import importlib.metadata

from packaging.version import Version

__version__ = importlib.metadata.version(__package__)
__minecraft__version__ = (lambda v: f"{v[0]}.{v[1]}.{v[2]}.{v[3]:02}")([int(a) for a in Version(__version__).release])

from .chat_color import ChatColor
from .logger import Logger
