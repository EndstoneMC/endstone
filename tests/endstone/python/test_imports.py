import importlib


def test_import_endstone():
    module = importlib.import_module("endstone")
    getattr(module, "Server")
    getattr(module, "Logger")


def test_import_plugin():
    module = importlib.import_module("endstone.plugin")
    getattr(module, "Plugin")
    getattr(module, "PluginDescription")
    getattr(module, "PluginLoader")
