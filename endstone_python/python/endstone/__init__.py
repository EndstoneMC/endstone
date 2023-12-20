from importlib import import_module

from packaging.version import Version

from ._version import __version__, __version_tuple__

__minecraft__version__ = (lambda v: f"{v[0]}.{v[1]}.{v[2]}.{v[3]:02}")([int(a) for a in Version(__version__).release])

module = import_module(f"{__name__}._api")
globals().update(module.__dict__)
del module
