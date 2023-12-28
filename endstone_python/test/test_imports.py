import importlib


def test_import_endstone():
    module = importlib.import_module("endstone")


def test_import_server():
    module = importlib.import_module("endstone.server")
    getattr(module, "Server")


def test_import_logger():
    module = importlib.import_module("endstone.logger")
    getattr(module, "Logger")


def test_import_plugin():
    module = importlib.import_module("endstone.plugin")
    getattr(module, "Plugin")
    getattr(module, "PluginDescription")
    getattr(module, "PluginLoader")
