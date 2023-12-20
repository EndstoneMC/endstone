from importlib import import_module

from _version import __version__, __version_tuple__

module = import_module(f"{__name__}._api")
globals().update(module.__dict__)
del module
