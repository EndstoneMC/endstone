import errno
import fnmatch
import hashlib
import json
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
import tomlkit
from packaging.version import Version
from rich.progress import BarColumn, DownloadColumn, Progress, TextColumn, TimeRemainingColumn

from endstone import __minecraft_version__
from endstone.cli import _properties

# server.properties entries where Endstone's default differs from Mojang's.
_SERVER_PROPERTY_OVERRIDES = {
    "server-name": "Endstone Server",
    "client-side-chunk-generation-enabled": False,
}

# NetworkStackLatencyPacket, left unbounded by the shipped packetlimitconfig.json.
_PING_PACKET_ID = 115


class Bootstrap:
    def __init__(self, server_folder: str, no_confirm: bool, remote: str, interactive: bool) -> None:
        self._server_path = Path(server_folder).absolute()
        self._no_confirm = no_confirm
        self._remote = remote
        self._logger = logging.getLogger(self.name)
        self._process: subprocess.Popen[str]
        self._interactive = interactive

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

    @property
    def minecraft_version(self) -> Version:
        return Version(__minecraft_version__)

    def _validate(self) -> None:
        if platform.system().lower() != self.target_system:
            raise NotImplementedError(f"{platform.system()} is not supported by this bootstrap.")
        if not self.executable_path.exists():
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), str(self.executable_path))
        if not self._endstone_runtime_path.exists():
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), str(self._endstone_runtime_path))

    def _download(self, dst: Union[str, os.PathLike[str]]) -> None:
        dst = Path(dst)

        self._logger.info("Loading index from the remote server...")
        channel = "preview" if self.minecraft_version.is_prerelease else "release"
        version = f"1.{self.minecraft_version}" if self.minecraft_version.major > 1 else str(self.minecraft_version)
        metadata_url = "/".join([self._remote, channel, version, "metadata.json"])
        response = requests.get(metadata_url, timeout=10)
        response.raise_for_status()
        metadata = response.json()

        if version != metadata["version"]:
            raise ValueError(f"Version mismatch, expect: {version}, actual: {metadata['version']}")

        default_properties: Union[str, None] = None

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
                            if file == "server.properties":
                                default_properties = zip_ref.read(file).decode("utf-8")
                            self._logger.info(f"{dest_path} already exists, skipping.")
                            continue

                    zip_ref.extract(file, dst)

        self._update_server_properties(dst / "server.properties", default_properties)

        version_file = dst / "version.txt"
        with version_file.open("w", encoding="utf-8") as f:
            f.writelines(str(self.minecraft_version))

    def _update_server_properties(self, path: Path, defaults: Union[str, None]) -> None:
        """
        Applies the Endstone defaults to a freshly extracted server.properties, or, when the server already had one,
        appends the entries this version of the Bedrock Dedicated Server added, leaving the user's values alone.
        """
        if not path.exists():
            return

        with path.open("r", encoding="utf-8", newline="") as file:
            props = _properties.load(file)

        if defaults is None:
            for key, value in _SERVER_PROPERTY_OVERRIDES.items():
                if key in props:
                    props[key] = value
        else:
            added = self._merge_server_properties(_properties.loads(defaults), props)
            if not added:
                return
            self._logger.info(f"Added {len(added)} new entries to server.properties: {', '.join(added)}")

        with path.open("w", encoding="utf-8", newline="") as file:
            _properties.dump(props, file)

    @staticmethod
    def _merge_server_properties(defaults: _properties.Properties, props: _properties.Properties) -> list[str]:
        """
        Appends every property in defaults that props lacks, along with the comments documenting it, which the Bedrock
        Dedicated Server writes below the property rather than above it.
        """
        added = []
        body = defaults.body
        for i, item in enumerate(body):
            if not isinstance(item, _properties.Property) or item.key in props:
                continue

            if item.key in _SERVER_PROPERTY_OVERRIDES:
                item.value = _SERVER_PROPERTY_OVERRIDES[item.key]

            if props.body and not isinstance(props.body[-1], _properties.Whitespace):
                props.add_blank()

            props.append(item)
            for trailing in body[i + 1 :]:
                if not isinstance(trailing, _properties.Comment):
                    break
                props.append(trailing)

            added.append(item.key)

        return added

    def _prepare(self) -> None:
        # ensure the plugin folder exists
        self.plugin_path.mkdir(parents=True, exist_ok=True)

        # create or update the config file
        ref = importlib_resources.files("endstone") / "config" / "endstone.default.toml"
        if not self.config_path.exists():
            with importlib_resources.as_file(ref) as path:
                shutil.copy(path, self.config_path)
        else:
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
                            migrate_config(val, to_doc[key])  # type: ignore[arg-type]

            migrate_config(default_config, config)
            with open(self.config_path, "w", encoding="utf-8") as f:
                tomlkit.dump(config, f)

        self._update_packet_limit_config()

    def _update_packet_limit_config(self) -> None:
        path = self.server_path / "packetlimitconfig.json"

        config: dict = {}
        if path.exists():
            try:
                with path.open("r", encoding="utf-8") as f:
                    config = json.load(f)
            except (OSError, ValueError):
                return

        if not isinstance(config, dict):
            return

        groups = config.setdefault("limitGroups", [])
        if not isinstance(groups, list):
            return

        for group in groups:
            ids = group.get("minecraftPacketIds") if isinstance(group, dict) else None
            if isinstance(ids, list) and _PING_PACKET_ID in ids:
                return

        groups.append(
            {
                "minecraftPacketIds": [_PING_PACKET_ID],
                "algorithm": {
                    "name": "BucketPacketLimitAlgorithm",
                    "params": {"drainRatePerSec": 5, "maxBucketSize": 20},
                },
            }
        )

        with path.open("w", encoding="utf-8") as f:
            json.dump(config, f, indent=2)

        self._logger.info(f"Added a rate limit for packet {_PING_PACKET_ID} to packetlimitconfig.json.")

    def _install(self) -> None:
        """
        Installs the server if not already installed.
        """

        if self.executable_path.exists():
            self._update()
            return

        if not self._no_confirm:
            download = click.confirm(
                f"Bedrock Dedicated Server (v{self.minecraft_version}) "
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
        supported_version = self.minecraft_version

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
                f"Would you like to update to v{self.minecraft_version} now?",
                default=True,
            )
        else:
            update = True

        if not update:
            sys.exit(1)

        self._logger.info(f"Updating server from v{current_version} to v{self.minecraft_version}...")
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
        if self._interactive:
            env["ENDSTONE_USE_INTERACTIVE_CONSOLE"] = "1"
        return env

    def _run(self, *args, **kwargs) -> int:  # type: ignore[no-untyped-def]
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
