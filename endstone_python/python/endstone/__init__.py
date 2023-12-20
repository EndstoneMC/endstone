from importlib import import_module

module = import_module(f"{__name__}._api")
globals().update(module.__dict__)
del module
