import logging
import os
import platform
import sys
import urllib.parse
import zipfile
from pathlib import Path

import click
import requests
import tqdm

from endstone import __minecraft__version__

logger = logging.getLogger("main")


@click.command
@click.option("-p", "--path", default="minecraft_server")
@click.option("-y", "--yes", default=False, is_flag=True, show_default=True)
def cli(path: str, yes: bool):
    minecraft_version = __minecraft__version__
    path = Path(path) / minecraft_version

    system = platform.system()
    if system == "Windows":
        filename = "bedrock_server.exe"
        base_url = f"https://minecraft.azureedge.net/bin-win/"
        from .windows import start_server
    else:
        raise NotImplementedError(f"{system} is not currently supported.")

    exec_path = (path / filename).absolute()
    if not exec_path.exists():
        if not yes:
            download = click.confirm(
                f"Bedrock Dedicated Server (v{minecraft_version}) can not be found in {str(path)}. "
                f"Would you like to download it now?",
                default=True,
            )
        else:
            download = True

        if download:
            path.mkdir(parents=True, exist_ok=True)
            zip_filename = f"bedrock-server-{minecraft_version}.zip"

            response = requests.get(urllib.parse.urljoin(base_url, zip_filename), stream=True)
            assert response.status_code == 200, f"Error while downloading from {response.url}"

            total_size = int(response.headers.get("Content-Length", 0))
            logger.info(f"Downloading {zip_filename}...")
            with tqdm.tqdm(total=total_size, unit="iB", unit_scale=True) as progress_bar:
                with open(path / zip_filename, "wb") as file:
                    for data in response.iter_content(chunk_size=1024):
                        progress_bar.update(len(data))
                        file.write(data)

            # TODO: checksum
            logger.info(f"Unzipping {zip_filename}...")
            with zipfile.ZipFile(path / zip_filename, "r") as zf:
                zf.extractall(path)

            logger.info(f"Cleaning {zip_filename}...")
            os.remove(path / zip_filename)

        else:
            raise FileNotFoundError(f"{exec_path}")

    plugins_dir = path / "plugins"
    plugins_dir.mkdir(exist_ok=True)
    sys.path.append(str(plugins_dir.absolute()))

    os.environ["PYTHONHOME"] = sys.base_exec_prefix
    os.environ["PYTHONPATH"] = os.pathsep.join(sys.path)
    os.chdir(path)

    start_server(exec_path)


if __name__ == "__main__":
    cli()
