import importlib.metadata

from packaging.version import Version

__version__ = importlib.metadata.version("endstone")


def _get_minecraft_version():
    version = Version(__version__)
    major, minor, patch, build = [int(a) for a in version.release]
    return f"{major}.{minor}.{patch}.{build:02}"


__minecraft__version__ = _get_minecraft_version()
