import importlib


def test_import_endstone():
    # import endstone
    module = importlib.import_module("endstone")

    # from endstone import __version__, __minecraft_version__
    getattr(module, "__version__")
    getattr(module, "__minecraft_version__")

    # from endstone import Logger, Server
    getattr(module, "Server")
    getattr(module, "Logger")


def test_import_plugin():
    # import endstone.plugin
    module = importlib.import_module("endstone.plugin")

    # from endstone.plugin import Plugin, PluginDescription, PluginLoader
    getattr(module, "Plugin")
    getattr(module, "PluginDescription")
    getattr(module, "PluginLoader")
