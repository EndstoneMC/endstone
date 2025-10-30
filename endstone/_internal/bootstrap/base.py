import errno
import fnmatch
import hashlib
import logging
import os
import platform
import shutil
import subprocess
import sys
import tempfile
import zipfile
from pathlib import Path
from typing import Union

import click
import importlib_resources
import requests
import sentry_crashpad
import tomlkit
from packaging.version import Version
from rich.progress import BarColumn, DownloadColumn, Progress, TextColumn, TimeRemainingColumn

from endstone import __minecraft_version__ as minecraft_version


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
    def config_path(self) -> Path:
        return self.server_path / "endstone.toml"

    @property
    def plugin_path(self) -> Path:
        return self.server_path / "plugins"

    @property
    def user_agent(self) -> str:
        return "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36"

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
        channel = "preview" if Version(minecraft_version).is_prerelease else "release"
        metadata_url = "/".join([self._remote, channel, minecraft_version, "metadata.json"])
        response = requests.get(metadata_url, timeout=10)
        response.raise_for_status()
        metadata = response.json()

        if minecraft_version != metadata["version"]:
            raise ValueError(f"Version mismatch, expect: {minecraft_version}, actual: {metadata['version']}")

        should_modify_server_properties = True

        with tempfile.TemporaryFile(dir=dst) as f:
            url = metadata["binary"][self.target_system.lower()]["url"]
            self._logger.info(f"Downloading server from {url}...")
            response = requests.get(url, stream=True, headers={"User-Agent": self.user_agent})
            response.raise_for_status()
            total_size = int(response.headers.get("Content-Length", 0))
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
            if m.hexdigest() != metadata["binary"][self.target_system.lower()]["sha256"]:
                raise ValueError("SHA256 mismatch: the downloaded file may be corrupted or tampered with.")

            self._logger.info(f"Integrity check passed. Extracting to {dst}...")
            dst.mkdir(parents=True, exist_ok=True)
            override_patterns = [
                self.executable_filename,
                "behavior_packs/*",
                "definitions/*",
                "resource_packs/*",
                "bedrock_server_how_to.html",
                "profanity_filter.wlist",
                "release-notes.txt",
            ]
            with zipfile.ZipFile(f) as zip_ref:
                for file in zip_ref.namelist():
                    dest_path = dst / file
                    if dest_path.exists():
                        if not any(fnmatch.fnmatch(file, pattern) for pattern in override_patterns):
                            should_modify_server_properties = False
                            self._logger.info(f"{dest_path} already exists, skipping.")
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
        # ensure the plugin folder exists
        self.plugin_path.mkdir(parents=True, exist_ok=True)

        # prepare the crashpad handler
        src_dir, dst_dir = Path(sentry_crashpad._get_executable("crashpad_handler")).parent, self.server_path
        for s in src_dir.rglob("*"):
            if s.is_dir():
                continue
            rel = s.relative_to(src_dir)
            d = dst_dir / rel
            if d.exists() and d.is_dir():
                raise RuntimeError(f"Destination path is a directory: {d}")
            d.parent.mkdir(parents=True, exist_ok=True)
            if not d.exists():
                try:
                    shutil.copy2(s, d)
                except Exception as e:
                    raise RuntimeError(f"Failed to copy {s} -> {d}: {e}")

        # create or update the config file
        if not self.config_path.exists():
            ref = importlib_resources.files("endstone") / "config" / "endstone.toml"
            with importlib_resources.as_file(ref) as path:
                shutil.copy(path, self.config_path)
        else:
            ref = importlib_resources.files("endstone") / "config" / "endstone.toml"
            with importlib_resources.as_file(ref) as path:
                with open(path, "r", encoding="utf-8") as f:
                    default_config = tomlkit.load(f)

            with open(self.config_path, "r", encoding="utf-8") as f:
                config = tomlkit.load(f)

            def migrate_config(from_doc: tomlkit.TOMLDocument, to_doc: tomlkit.TOMLDocument) -> None:
                for key, val in from_doc.items():
                    if key not in to_doc:
                        # if the user hasn’t set it, copy it (with comments!)
                        to_doc[key] = val
                    else:
                        # if both are tables, dive deeper
                        if isinstance(val, tomlkit.TOMLDocument) and isinstance(to_doc[key], tomlkit.TOMLDocument):
                            migrate_config(val, to_doc[key])

            migrate_config(default_config, config)
            with open(self.config_path, "w", encoding="utf-8") as f:
                tomlkit.dump(config, f)

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
        return self._run()

    @property
    def _endstone_runtime_filename(self) -> str:
        raise NotImplementedError

    @property
    def _endstone_runtime_path(self) -> Path:
        p = Path(__file__).parent.parent / self._endstone_runtime_filename
        return p.resolve().absolute()

    @property
    def _endstone_runtime_env(self) -> dict[str, str]:
        env = os.environ.copy()
        env["PATH"] = os.pathsep.join(sys.path)
        env["PYTHONPATH"] = os.pathsep.join(sys.path)
        env["PYTHONIOENCODING"] = "UTF-8"
        env["ENDSTONE_PYTHON_EXECUTABLE"] = sys.executable
        return env

    def _run(self, *args, **kwargs) -> int:
        """
        Runs the server and returns its exit code.

        This method blocks until the server process terminates. It returns the exit code of the process, which can be
        used to determine if the server shut down successfully or if there were errors.

        Args:
            *args: Variable length argument list.
            **kwargs: Arbitrary keyword arguments.

        Returns:
            int: The exit code of the server process.
        """
        raise NotImplementedError
