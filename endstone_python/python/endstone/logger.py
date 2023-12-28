from importlib import import_module

for name in (__all__ := ["Logger"]):
    module = import_module("endstone._internal.endstone_python")
    globals()[f"_{name}"] = module.__dict__[name]
    del module

Logger = globals()["_Logger"]
