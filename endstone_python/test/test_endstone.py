import importlib


def test_import_endstone():
    # import endstone
    module = importlib.import_module("endstone")
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


def test_construct_plugin_description():
    module = importlib.import_module("endstone.plugin")
    PluginDescription = getattr(module, "PluginDescription")

    # construct with required arguments
    desc = PluginDescription("test", "1.0.0")
    assert desc.name == "test"
    assert desc.version == "1.0.0"
    assert desc.description is None
    assert desc.authors is None
    assert desc.prefix is None

    # construct with positional arguments
    desc = PluginDescription("test", "1.0.0", "this is a description")
    assert desc.name == "test"
    assert desc.version == "1.0.0"
    assert desc.description == "this is a description"
    assert desc.authors is None
    assert desc.prefix is None

    # construct with keyword arguments
    desc = PluginDescription(name="test", version="1.0.0", description="this is a description")
    assert desc.name == "test"
    assert desc.version == "1.0.0"
    assert desc.description == "this is a description"
    assert desc.authors is None
    assert desc.prefix is None

    # construct with positional and keyword arguments
    desc = PluginDescription("test", "1.0.0", prefix="Python", description="this is also a description")
    assert desc.name == "test"
    assert desc.version == "1.0.0"
    assert desc.description == "this is also a description"
    assert desc.authors is None
    assert desc.prefix == "Python"

    # construct with dictionary
    desc = PluginDescription(
        **{
            "name": "test",
            "version": "1.0.0",
            "description": "this is another description",
            "extra": "this is an extra argument",
            "redundant": "this is a redundant argument",
            "authors": ["Alice", "Bob"],
        }
    )
    assert desc.name == "test"
    assert desc.version == "1.0.0"
    assert desc.description == "this is another description"
    assert "Alice" in desc.authors
    assert "Bob" in desc.authors
    assert desc.prefix is None
