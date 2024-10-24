import errno
import hashlib
import logging
import os
import platform
import subprocess
import sys
import tempfile
import zipfile
from pathlib import Path
from typing import Union

import click
import requests
from packaging.version import Version
from rich.progress import BarColumn, DownloadColumn, Progress, TextColumn, TimeRemainingColumn

from endstone import __minecraft_version__ as minecraft_version
from endstone import __version__ as endstone_version


class Bootstrap:
    def __init__(self, server_folder: str, no_confirm: bool, remote: str) -> None:
        self._server_path = Path(server_folder).absolute()
        self._no_confirm = no_confirm
        self._remote = remote
        self._logger = logging.getLogger(self.name)
        self._process: subprocess.Popen

    @property
    def name(self) -> str:
        return __name__

    @property
    def target_system(self) -> str:
        raise NotImplementedError

    @property
    def executable_filename(self) -> str:
        raise NotImplementedError

    @property
    def server_path(self) -> Path:
        return self._server_path

    @property
    def executable_path(self) -> Path:
        return self.server_path / self.executable_filename

    @property
    def plugin_path(self) -> Path:
        return self.server_path / "plugins"

    @property
    def user_agent(self) -> str:
        return f"Endstone/{endstone_version} (Minecraft/{minecraft_version})"

    def _validate(self) -> None:
        if platform.system().lower() != self.target_system:
            raise NotImplementedError(f"{platform.system()} is not supported by this bootstrap.")
        if not self.executable_path.exists():
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), str(self.executable_path))
        if not self._endstone_runtime_path.exists():
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), str(self._endstone_runtime_path))

    def _download(self, dst: Union[str, os.PathLike]) -> None:
        dst = Path(dst)

        self._logger.info("Loading index from the remote server...")
        response = requests.get(self._remote)
        response.raise_for_status()
        server_data = response.json()

        if minecraft_version not in server_data["binary"]:
            raise ValueError(f"Version v{minecraft_version} is not found in the remote server.")

        should_modify_server_properties = True

        with tempfile.TemporaryFile(dir=dst) as f:
            metadata = server_data["binary"][minecraft_version][self.target_system.lower()]
            url = metadata["url"]
            response = requests.get(url, stream=True, headers={"User-Agent": self.user_agent})
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

            self._logger.info("Download complete. Verifying integrity...")
            if m.hexdigest() != metadata["sha256"]:
                raise ValueError("SHA256 mismatch: the downloaded file may be corrupted or tampered with.")

            self._logger.info(f"Integrity check passed. Extracting to {dst}...")
            dst.mkdir(parents=True, exist_ok=True)
            with zipfile.ZipFile(f) as zip_ref:
                for file in zip_ref.namelist():
                    if file in ["allowlist.json", "permissions.json", "server.properties"] and (dst / file).exists():
                        self._logger.info(f"{file} already exists, skipping.")
                        should_modify_server_properties = False
                        continue

                    zip_ref.extract(file, dst)

        if should_modify_server_properties:
            properties = dst / "server.properties"
            with properties.open("r", encoding="utf-8") as file:
                in_lines = file.readlines()

            out_lines = []
            for line in in_lines:
                if line.strip() == "server-name=Dedicated Server":
                    out_lines.append("server-name=Endstone Server\n")
                elif line.strip() == "client-side-chunk-generation-enabled=true":
                    out_lines.append("client-side-chunk-generation-enabled=false\n")
                else:
                    out_lines.append(line)

            with properties.open("w", encoding="utf-8") as file:
                file.writelines(out_lines)

        version_file = dst / "version.txt"
        with version_file.open("w", encoding="utf-8") as file:
            file.writelines(minecraft_version)

    def _prepare(self) -> None:
        self.plugin_path.mkdir(parents=True, exist_ok=True)

    def _install(self) -> None:
        """
        Installs the server if not already installed.
        """

        if self.executable_path.exists():
            self._update()
            return

        if not self._no_confirm:
            download = click.confirm(
                f"Bedrock Dedicated Server (v{minecraft_version}) "
                f"is not found in {str(self.executable_path.parent)}. "
                f"Would you like to download it now?",
                default=True,
            )
        else:
            download = True

        if not download:
            sys.exit(1)

        self.server_path.mkdir(parents=True, exist_ok=True)
        self._download(self.server_path)

    def _update(self) -> None:
        current_version = Version("0.0.0")
        supported_version = Version(minecraft_version)

        version_file = self.server_path / "version.txt"
        if version_file.exists():
            with version_file.open("r", encoding="utf-8") as file:
                current_version = Version(file.readline())

        if current_version == supported_version:
            return

        if current_version > supported_version:
            raise RuntimeError(
                f"A newer version of Bedrock Dedicated Server (v{current_version}) "
                f"is found in {str(self.executable_path.parent)}. Please update your Endstone server."
            )

        if not self._no_confirm:
            update = click.confirm(
                f"An older version of Bedrock Dedicated Server (v{current_version}) "
                f"is found in {str(self.executable_path.parent)}. "
                f"Would you like to update to v{minecraft_version} now?",
                default=True,
            )
        else:
            update = True

        if not update:
            sys.exit(1)

        self._logger.info(f"Updating server from v{current_version} to v{minecraft_version}...")
        self._download(self.server_path)

    def run(self) -> int:
        self._install()
        self._validate()
        self._prepare()
        self._create_process()
        return self._wait_for_server()

    @property
    def _endstone_runtime_filename(self) -> str:
        raise NotImplementedError

    @property
    def _endstone_runtime_path(self) -> Path:
        p = Path(__file__).parent.parent / self._endstone_runtime_filename
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
        env["PYTHONIOENCODING"] = "UTF-8"
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
