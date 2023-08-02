import importlib.metadata

__version__ = importlib.metadata.version('endstone')


def _get_minecraft_version():
    major, minor, patch, build = [int(a) for a in __version__.split('.')[:4]]
    return f"{major}.{minor}.{patch}.{build:02}"


__minecraft__version__ = _get_minecraft_version()
