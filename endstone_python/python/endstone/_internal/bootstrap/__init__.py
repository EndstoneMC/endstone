import errno
import functools
import logging
import os
import platform
import sys

import click

from endstone._internal.version import __version__, __version_tuple__

logging.basicConfig(level=logging.INFO, format="[%(asctime)s %(levelname)s] [%(name)s] %(message)s")
logger = logging.getLogger(__name__)

__all__ = ["cli"]


def catch_exceptions(func):
    """Decorator to catch and log exceptions."""

    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except Exception as e:
            logger.exception(e)
            sys.exit(-1)

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
def cli(install_folder: str, install: bool, remote: str) -> int:
    """
    Starts an endstone server.

    Args:
        install_folder (str): The directory where the bedrock server files will be installed.
        install (bool): Whether to install the bedrock server if not already installed.
        remote (str): The remote URL to retrieve bedrock server data from.

    Raises:
        NotImplementedError: If the operating system is not supported.
        FileNotFoundError: If the server executable is not found and install is set to False.
    """
    server_version = ".".join(map(str, __version_tuple__[:3]))

    system = platform.system()
    if system == "Windows":
        from endstone._internal.bootstrap.windows import WindowsBootstrap

        cls = WindowsBootstrap

    elif system == "Linux":
        from endstone._internal.bootstrap.linux import LinuxBootstrap

        cls = LinuxBootstrap
    else:
        raise NotImplementedError(f"{system} is not supported.")

    bootstrap = cls(install_path=install_folder, version=server_version, remote=remote)

    if not bootstrap.executable_path.exists():
        if not install:
            download = click.confirm(
                f"Bedrock Dedicated Server (v{server_version}) "
                f"is not found in {str(bootstrap.executable_path.parent)}. "
                f"Would you like to download it now?",
                default=True,
            )
        else:
            download = True

        if not download:
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), str(bootstrap.executable_path))

        bootstrap.install()

    exit_code = bootstrap.run()
    sys.exit(exit_code)


if __name__ == "__main__":
    cli()
