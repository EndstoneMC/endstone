import errno
import functools
import hashlib
import logging
import os
import platform
import sys
import zipfile
from pathlib import Path
from tempfile import TemporaryFile

import click
import requests
from rich.progress import Progress, BarColumn, TimeRemainingColumn, DownloadColumn, TextColumn

from endstone._internal.version import __version__, __version_tuple__

logging.basicConfig(level=logging.INFO, format="[%(asctime)s %(levelname)s] [%(name)s] %(message)s")
logger = logging.getLogger(__name__)


def catch_exceptions(func):
    """Decorator to catch and log exceptions."""

    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except Exception as e:
            logger.error(e)
            sys.exit(1)

    return wrapper


@click.command(help="Starts an endstone server.")
@click.option(
    "-if",
    "--install-folder",
    default="bedrock_server",
    help="Specify the folder for installing the bedrock server. Defaults to 'bedrock_server'.",
)
@click.option(
    "-i",
    "--install",
    default=False,
    is_flag=True,
    show_default=True,
    help="Install the bedrock server if one is not already installed under the install folder",
)
@click.option(
    "-r",
    "--remote",
    default="https://raw.githubusercontent.com/EndstoneMC/bedrock-server-data/main/bedrock_server_data.json",
    help="The remote URL to retrieve bedrock server data from.",
)
@click.version_option(__version__)
@catch_exceptions
def cli(install_folder: str, install: bool, remote: str) -> None:
    """
    Starts an endstone server.

    Args:
        install_folder (str): The directory path where the bedrock server files will be installed.
        install (bool): Whether to install the bedrock server if not already installed.
        remote (str): The remote URL to retrieve bedrock server data from.

    Raises:
        NotImplementedError: If the operating system is not supported.
        FileNotFoundError: If the server executable is not found and install flag is not set to True.
    """
    version = ".".join(map(str, __version_tuple__[:3]))
    install_folder = Path(install_folder) / version

    system = platform.system()
    if system == "Windows":
        executable_name = "bedrock_server.exe"
    elif system == "Linux":
        executable_name = "bedrock_server"
    else:
        raise NotImplementedError(f"{system} is not supported.")

    executable_path = (install_folder / executable_name).absolute()
    if not executable_path.exists():
        if not install:
            download = click.confirm(
                f"Bedrock Dedicated Server (v{version}) can not be found in {str(install_folder)}. "
                f"Would you like to download it from remote now?",
                default=True,
            )
        else:
            download = True

        if not download:
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), str(executable_path))

        install_folder.mkdir(parents=True, exist_ok=True)

        logger.info("Loading index from the remote server...")
        response = requests.get(remote)
        response.raise_for_status()
        server_data = response.json()

        assert version in server_data["binary"], f"Version v{version} is not found in the remote server."

        url = server_data["binary"][version][system.lower()]["url"]
        sha256 = server_data["binary"][version][system.lower()]["sha256"]

        with TemporaryFile() as f:
            response = requests.get(url, stream=True)
            response.raise_for_status()
            total_size = int(response.headers.get("Content-Length", 0))
            logger.info(f"Downloading {url}...")
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

            logger.info(f"Verifying...")
            assert m.hexdigest() == sha256, "SHA256 mismatch: the downloaded file may be corrupted or tampered with."

            logger.info(f"Unzipping to {install_folder}...")
            with zipfile.ZipFile(f) as zip_ref:
                zip_ref.extractall(install_folder)

    plugins_dir = install_folder / "plugins"
    plugins_dir.mkdir(exist_ok=True)
    # sys.path.append(str(plugins_dir.absolute()))
    #
    # os.environ["PYTHONHOME"] = sys.base_exec_prefix
    # os.environ["PYTHONPATH"] = os.pathsep.join(sys.path)
    # os.chdir(install_folder)

    # TODO: start server


if __name__ == "__main__":
    cli()
