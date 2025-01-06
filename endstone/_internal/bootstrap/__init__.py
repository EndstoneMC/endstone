import functools
import logging
import platform
import sys

import click

from endstone._internal.version import __version__

logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s.%(msecs)03d %(levelname)s] [%(name)s] %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
)
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
    "-s",
    "--server-folder",
    default="bedrock_server",
    help="Specify the folder for the bedrock server. Defaults to 'bedrock_server'.",
)
@click.option(
    "-y",
    "--no-confirm",
    "--yes",
    default=False,
    is_flag=True,
    show_default=True,
    help="Assume yes as answer to all prompts",
)
@click.option(
    "-r",
    "--remote",
    default="https://raw.githubusercontent.com/EndstoneMC/bedrock-server-data/main/bedrock_server_data.json",
    help="The remote URL to retrieve bedrock server data from.",
)
@click.version_option(__version__)
@catch_exceptions
def cli(server_folder: str, no_confirm: bool, remote: str) -> None:
    system = platform.system()
    if system == "Windows":
        from endstone._internal.bootstrap.windows import WindowsBootstrap

        cls = WindowsBootstrap

    elif system == "Linux":
        from endstone._internal.bootstrap.linux import LinuxBootstrap

        cls = LinuxBootstrap
    else:
        raise NotImplementedError(f"{system} is not supported.")

    bootstrap = cls(server_folder=server_folder, no_confirm=no_confirm, remote=remote)
    exit_code = bootstrap.run()
    sys.exit(exit_code)
