import importlib


def test_import_endstone():
    # import endstone
    module = importlib.import_module("endstone")
    assert getattr(module, "__version__") is not None
    assert getattr(module, "__minecraft_version__") is not None


def test_import_server():
    # from endstone import Server
    getattr(importlib.import_module("endstone"), "Server")
