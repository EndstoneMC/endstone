import importlib


def test_import_endstone():
    # import endstone
    importlib.import_module("endstone")


def test_import_server():
    # from endstone import Server
    getattr(importlib.import_module("endstone"), "Server")
