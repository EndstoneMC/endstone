#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "click",
#     "requests",
#     "tomlkit",
#     "tqdm",
# ]
# ///
"""
Download a Bedrock Dedicated Server (BDS) release and extract its server binary
into the bedrock-symbols tree so `idat.exe` can build an IDA database from it.

This is the download/extract half of the build-ida-db skill. It reuses the same
source and SHA256-keyed cache as scripts/dump_symbols.py:

    https://raw.githubusercontent.com/EndstoneMC/bedrock-server-data/v2
    cache: ~/.bedrock_server/<platform>/<zip>

The version defaults to the `version` key of scripts/configs/<platform>.toml, so
in normal use you just pass `--platform`. It prints the extracted binary's path
on the last stdout line (consume that to launch idat).

    uv run --script fetch_bds.py --platform windows
    uv run --script fetch_bds.py --platform linux --version 1.26.30
"""

import hashlib
import os
from pathlib import Path
from zipfile import ZipFile

import click
import requests
import tomlkit

BASE_RAW_URL = "https://raw.githubusercontent.com/EndstoneMC/bedrock-server-data/v2"
CACHE_DIR = Path.home() / ".bedrock_server"
HEADERS = {
    "User-Agent": (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
        "(KHTML, like Gecko) Chrome/129.0.0.0 Safari/537.36 Edg/129.0.0.0"
    )
}

# repo root = .../endstone ; bedrock-symbols sits beside it (.../Endstone/bedrock-symbols)
REPO_ROOT = Path(__file__).resolve().parents[3]
SYMBOLS_ROOT = REPO_ROOT.parent / "bedrock-symbols"

# Per-platform: IDA-db arch folder + the binary's name inside the zip.
PLATFORMS = {
    "windows": {"arch": "win32_server_x64", "binary": "bedrock_server.exe"},
    "linux": {"arch": "linux_server_x64", "binary": "bedrock_server"},
}


def echo(msg: str) -> None:
    click.echo(msg, err=True)  # keep stdout clean for the final path


def rel(path: Path) -> str:
    """POSIX-style path relative to the cwd (repo root in normal use)."""
    try:
        return Path(os.path.relpath(path)).as_posix()
    except ValueError:
        return path.as_posix()  # different drive: keep absolute, still forward-slash


def compute_sha256(path: Path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(8192), b""):
            h.update(chunk)
    return h.hexdigest()


def download_server(version: str, platform: str) -> Path:
    """Download (or reuse a cached, hash-verified copy of) the BDS zip."""
    url = f"{BASE_RAW_URL}/release/{version}/metadata.json"
    resp = requests.get(url, headers=HEADERS)
    resp.raise_for_status()
    info = resp.json().get("binary", {}).get(platform)
    if not info or "url" not in info or "sha256" not in info:
        raise KeyError(f"No {platform} binary info for version {version}")

    download_url = info["url"]
    expected = info["sha256"].lower()
    dest = CACHE_DIR / platform / os.path.basename(download_url)

    if dest.exists() and compute_sha256(dest).lower() == expected:
        echo(f"Using cached {platform} zip: {dest}")
        return dest

    echo(f"Downloading {platform} zip: {download_url}")
    dest.parent.mkdir(parents=True, exist_ok=True)
    with requests.get(download_url, headers=HEADERS, stream=True) as r:
        r.raise_for_status()
        total = int(r.headers.get("content-length", 0))
        from tqdm import tqdm

        with open(dest, "wb") as f, tqdm(
            total=total, unit="B", unit_scale=True, desc=dest.name, disable=not total
        ) as bar:
            for chunk in r.iter_content(chunk_size=8192):
                f.write(chunk)
                bar.update(len(chunk))
    return dest


def version_from_config(platform: str) -> str:
    cfg = REPO_ROOT / "scripts" / "configs" / f"{platform}.toml"
    with cfg.open("r") as f:
        return str(tomlkit.load(f)["version"])


@click.command()
@click.option("--platform", required=True, type=click.Choice(["windows", "linux"]))
@click.option("--version", "version", default=None, help="Defaults to the config's version key.")
@click.option(
    "--dest",
    type=click.Path(file_okay=False, path_type=Path),
    default=None,
    help="Output dir. Defaults to bedrock-symbols/<arch>/<version>/.",
)
def main(platform: str, version: str | None, dest: Path | None) -> None:
    version = version or version_from_config(platform)
    spec = PLATFORMS[platform]
    dest = dest or (SYMBOLS_ROOT / spec["arch"] / version)
    dest.mkdir(parents=True, exist_ok=True)

    binary = dest / spec["binary"]
    if binary.exists():
        echo(f"Binary already extracted: {binary}")
        click.echo(rel(binary))
        return

    zip_path = download_server(version, platform)
    echo(f"Extracting {spec['binary']} -> {dest}")
    with ZipFile(zip_path) as zf:
        with zf.open(spec["binary"]) as src, open(binary, "wb") as out:
            out.write(src.read())

    echo(f"Ready: {binary}")
    click.echo(rel(binary))  # final stdout line = the (repo-relative) path to feed idat


if __name__ == "__main__":
    main()
