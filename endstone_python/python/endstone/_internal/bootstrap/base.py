import errno
import hashlib
import logging
import os
import platform
import subprocess
import sys
import zipfile
from pathlib import Path
from tempfile import TemporaryFile
from typing import Union

import requests
from rich.progress import Progress, TextColumn, BarColumn, DownloadColumn, TimeRemainingColumn


class Bootstrap:
    """
    Handles the setup and execution of a bedrock server.

    This class is responsible for managing the installation, validation, and execution of bedrock server.
    It supports downloading the server binaries, verifying their integrity, installing them, and running the server.
    The class is designed to be extended for specific platform, where methods like `target_system` and
    `executable_filename` are expected to be implemented.

    Methods:
    validate: Validates if the current platform is supported.
    download: Downloads and extracts the server binaries from a remote URL.
    install: Installs the server if not already installed.
    run: Runs the server application.
    """

    def __init__(self, install_path: Union[str, os.PathLike], version: str, remote: str) -> None:
        """
        Initializes the Bootstrap class with installation path, version, installation flag, and remote URL.

        Args:
            install_path (Union[str, os.PathLike]): The path where the server is to be installed.
            version (str): The version of the server.
            remote (str): The URL of the remote server for downloading server binaries.

        """
        self._install_path = Path(install_path).absolute()
        self._version = version
        self._remote = remote
        self._logger = logging.getLogger(self.name)
        self._process: subprocess.Popen

    @property
    def name(self) -> str:
        """
        Provides the name of the class.

        Returns:
            str: The name of the class.
        """
        return __name__

    @property
    def target_system(self) -> str:
        """
        Returns the target system for which the server is intended.

        This method should be implemented in a subclass.

        Returns:
            str: The name of the target system (e.g., 'Windows', 'Linux').
        """
        raise NotImplementedError

    @property
    def executable_filename(self) -> str:
        """
        Returns the filename of the server executable.

        This method should be implemented in a subclass.

        Returns:
            str: The filename of the server executable (e.g., 'bedrock_server').
        """
        raise NotImplementedError

    @property
    def install_path(self) -> Path:
        """
        The base path where the bedrock servers with different versions are installed.

        Returns:
            Path: The path to the installation directory.
        """
        return self._install_path

    @property
    def server_path(self) -> Path:
        """
        The path to the specific version of the server.

        Returns:
            Path: The path to the server's version-specific directory.
        """
        return self.install_path / self._version

    @property
    def executable_path(self) -> Path:
        """
        The path to the server executable.

        Returns:
            Path: The full path to the server's executable file.
        """
        return self.server_path / self.executable_filename

    @property
    def plugin_path(self) -> Path:
        """
        The path to the server's plugins directory.

        Returns:
            Path: The path to the server's plugins directory.
        """
        return self.server_path / "plugins"

    def validate(self) -> None:
        """
        Validate if the current platform is supported by this bootstrap, and if the necessary paths exist.

        Raises:
            NotImplementedError: If the current platform system is not supported by this bootstrap.
            FileNotFoundError: If either the executable_path or _endstone_runtime_path do not exist.
        """
        assert platform.system() == self.target_system, NotImplementedError(
            f"{platform.system()} is not supported by this bootstrap."
        )
        assert self.executable_path.exists(), FileNotFoundError(
            errno.ENOENT, os.strerror(errno.ENOENT), str(self.executable_path)
        )
        assert self._endstone_runtime_path.exists(), FileNotFoundError(
            errno.ENOENT, os.strerror(errno.ENOENT), str(self._endstone_runtime_path)
        )

    def _download(self, dst: Union[str, os.PathLike], url: str, sha256: str) -> None:
        """
        Downloads and extracts the server binaries from a given URL.

        This method downloads a file from the specified URL, computes its SHA256 checksum, and compares it
        against the provided checksum to ensure the file's integrity. If the checksum matches, the file is
        extracted to the specified destination.

        Args:
            dst (Union[str, os.PathLike]): The destination path for the extracted server.
            url (str): The URL from which to download the server binaries.
            sha256 (str): The expected SHA256 checksum of the downloaded file.

        Raises:
            ValueError: If the SHA256 checksum of the downloaded file does not match the expected checksum.
        """

        with TemporaryFile() as f:
            response = requests.get(url, stream=True)
            response.raise_for_status()
            total_size = int(response.headers.get("Content-Length", 0))
            self._logger.info(f"Downloading server from {url}...")
            m = hashlib.sha256()

            with Progress(
                    TextColumn("[progress.description]{task.description}"),
                    BarColumn(),
                    DownloadColumn(),
                    TimeRemainingColumn(),
            ) as progress:
                task = progress.add_task("[bold blue]Downloading...", total=total_size)
                for data in response.iter_content(chunk_size=1024):
                    progress.update(task, advance=len(data))
                    f.write(data)
                    m.update(data)

            self._logger.info(f"Download complete. Verifying integrity...")
            if m.hexdigest() != sha256:
                raise ValueError("SHA256 mismatch: the downloaded file may be corrupted or tampered with.")

            self._logger.info(f"Integrity check passed. Extracting to {dst}...")
            dst = Path(dst)
            dst.mkdir(parents=True, exist_ok=True)
            with zipfile.ZipFile(f) as zip_ref:
                zip_ref.extractall(str(dst))

        self._download_finished()

    def _download_finished(self) -> None:
        self.validate()

    def install(self) -> None:
        """
        Installs the server if not already installed.
        """

        if self.executable_path.exists():
            return

        if self.server_path.exists() and any(self.server_path.iterdir()):
            raise AssertionError(
                f"Server directory {self.server_path} exists and is not empty but the server executable "
                f"{self.executable_filename} for the current system can not be found. Are you using the same "
                f"directory for both Windows and Linux platform?")

        self.server_path.mkdir(parents=True, exist_ok=True)

        self._logger.info("Loading index from the remote server...")
        response = requests.get(self._remote)
        response.raise_for_status()
        server_data = response.json()

        assert self._version in server_data["binary"], f"Version v{self._version} is not found in the remote server."
        self._download(self.server_path, **server_data["binary"][self._version][self.target_system.lower()])

    def run(self) -> int:
        """
        Runs the server application and returns its exit code.

        Returns:
            int: The exit code of the server process. A zero value typically indicates successful termination.
        """
        self.validate()
        self.plugin_path.mkdir(parents=True, exist_ok=True)
        self._create_process()
        return self._wait_for_server()

    @property
    def _endstone_runtime_filename(self) -> str:
        raise NotImplementedError

    @property
    def _endstone_runtime_path(self) -> Path:
        p = Path(__file__).parent / ".." / self._endstone_runtime_filename
        return p.resolve().absolute()

    def _create_process(self, *args, **kwargs) -> None:
        """
        Creates a subprocess for running the server.

        This method initializes a subprocess.Popen object for the server executable. It sets up the necessary
        buffers and encodings for the process and specifies the working directory.

        Args:
            *args: Variable length argument list.
            **kwargs: Arbitrary keyword arguments.

        """
        env = kwargs.pop("env", os.environ.copy())
        env["PATH"] = os.pathsep.join(sys.path)
        env["PYTHONPATH"] = os.pathsep.join(sys.path)
        self._process = subprocess.Popen(
            [str(self.executable_path.absolute())],
            stdin=sys.stdin,
            stdout=sys.stdout,
            stderr=subprocess.STDOUT,
            text=True,
            encoding="utf-8",
            cwd=str(self.server_path.absolute()),
            env=env,
            *args,
            **kwargs,
        )

    def _wait_for_server(self) -> int:
        """
        Waits for the server process to terminate and returns its exit code.

        This method blocks until the server process created by _create_process terminates. It returns the
        exit code of the process, which can be used to determine if the server shut down successfully or if
        there were errors.

        Returns:
            int: The exit code of the server process. Returns -1 if the process is not created or still running.
        """

        if self._process is None:
            return -1

        return self._process.wait()
