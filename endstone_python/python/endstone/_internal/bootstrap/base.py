import errno
import hashlib
import logging
import os
import platform
import zipfile
from pathlib import Path
from tempfile import TemporaryFile
from typing import Union

import aioconsole
import asyncio
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
        self._install_path = Path(install_path)
        self._version = version
        self._remote = remote
        self._logger = logging.getLogger(self.name)
        self._process: asyncio.subprocess.Process

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
        assert platform.system() == self.target_system, f"{platform.system()} is not supported by this boostrap."

    def download(self, dst: Union[str, os.PathLike], url: str, sha256: str) -> None:
        """
        Downloads and extracts the server binaries from a given URL.

        Args:
            dst (Union[str, os.PathLike]): The destination path where the server will be extracted.
            url (str): The URL from which the server binaries will be downloaded.
            sha256 (str): The SHA256 checksum of the file to verify its integrity.

        Raises:
            AssertionError: If the SHA256 checksum does not match.
        """

        with TemporaryFile() as f:
            response = requests.get(url, stream=True)
            response.raise_for_status()
            total_size = int(response.headers.get("Content-Length", 0))
            self._logger.info(f"Downloading {url}...")
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

            self._logger.info(f"Verifying...")
            assert m.hexdigest() == sha256, "SHA256 mismatch: the downloaded file may be corrupted or tampered with."

            self._logger.info(f"Unzipping to {dst}...")
            dst = Path(dst)
            dst.mkdir(parents=True, exist_ok=True)
            with zipfile.ZipFile(f) as zip_ref:
                zip_ref.extractall(str(dst))

    def install(self) -> None:
        """
        Installs the server if not already installed.
        """

        if self.executable_path.exists():
            return

        self.server_path.mkdir(parents=True, exist_ok=True)

        self._logger.info("Loading index from the remote server...")
        response = requests.get(self._remote)
        response.raise_for_status()
        server_data = response.json()

        assert self._version in server_data["binary"], f"Version v{self._version} is not found in the remote server."
        self.download(self.server_path, **server_data["binary"][self._version][self.target_system.lower()])

    def run(self) -> int:
        """
        Runs the server application.

        Returns:
            int: The exit code from the server process.
        """
        self.plugin_path.mkdir(parents=True, exist_ok=True)
        return asyncio.run(self._run_server())

    async def _run_server(self) -> int:
        """
        Core routine to manage the server process.

        This method includes pre-creation, creation, post-creation processes, and starts the server loop.

        Returns:
            int: The exit code from the server process.
        """

        await self._pre_create_process()
        assert self.executable_path.exists(), FileNotFoundError(
            errno.ENOENT, os.strerror(errno.ENOENT), str(self.executable_path)
        )
        await self._create_process()
        await self._post_create_process()
        await self._start_server_loop()
        return await self._server_terminated()

    async def _create_process(self, *args, **kwargs):
        """
        Creates an asynchronous subprocess for the server.
        """
        self._process = await asyncio.create_subprocess_exec(
            str(self.executable_path.absolute()),
            stdin=asyncio.subprocess.PIPE,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.STDOUT,
            *args,
            **kwargs,
        )

    async def _pre_create_process(self) -> None:
        """
        Lifecycle method to be run before creating the server process.

        This method can be overridden in subclasses to perform tasks before the creation of server process.
        """
        pass

    async def _post_create_process(self) -> None:
        """
        Lifecycle method to be run after creating the server process.

        This method can be overridden in subclasses to perform tasks after the creation of server process.
        """
        pass

    async def _start_server_loop(self) -> None:
        """
        Manages the server loop, handling input and output.

        This method reads output from the server and sends input to it, managing asynchronous tasks for each.
        """

        async def read_output(process: asyncio.subprocess.Process) -> None:
            while True:
                line = await process.stdout.readline()
                if not line:
                    send_task.cancel()
                    break

                print(line.decode().rstrip("\n"))

        async def send_input(process: asyncio.subprocess.Process) -> None:
            while True:
                line = await aioconsole.ainput()
                if not line:
                    break

                line += "\n"
                process.stdin.write(line.encode())
                await process.stdin.drain()

        read_task = asyncio.create_task(read_output(self._process))
        send_task = asyncio.create_task(send_input(self._process))
        await asyncio.gather(read_task, send_task, return_exceptions=True)

    async def _server_terminated(self) -> int:
        """
        Handles the server process termination.

        Returns:
            int: The exit code of the server process. Returns -1 if the process is not created.
        """

        if self._process is None:
            return -1

        return await self._process.wait()
