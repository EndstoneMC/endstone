from endstone._api import Server
from endstone._version import __version__
from packaging.version import Version

__minecraft_version__ = (lambda v: f"{v[0]}.{v[1]}.{v[2]}.{v[3]:02}")([int(a) for a in Version(__version__).release])

__all__ = [
    "__version__",
    "__minecraft_version__",
    "Server",
]
