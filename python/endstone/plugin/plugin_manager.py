import os.path

from endstone._plugin import Plugin
from endstone._plugin_manager import PluginManager
from endstone._server import Server


class DummyPlugin(Plugin):

    def on_enable(self):
        print("Enabled")

    def on_disable(self):
        print("Disabled")


class EndstonePluginManager(PluginManager):

    def __init__(self, server: Server):
        print("Creating EndstonePluginManager")
        PluginManager.__init__(self, server)
        self._server = server
        self._plugins = []

    def load_plugin(self, path: str) -> Plugin:
        return DummyPlugin()

    def load_plugins(self, directory: str) -> list[Plugin]:
        loaded = []
        plugin = self.load_plugin(os.path.join(directory, "sample_plugin"))
        loaded.append(plugin)
        self._plugins.append(plugin)
        return loaded

    def enable_plugins(self):
        for plugin in self._plugins:
            plugin.on_enable()
