import importlib
import os.path
from pathlib import Path

from endstone._plugin_manager import PluginManager as IPluginManager
import toml
from endstone._plugin import Plugin
from endstone._server import Server


class PluginManager(IPluginManager):

    def __init__(self, server: Server):
        IPluginManager.__init__(self, server)
        self._server = server
        self._plugins = []

    def load_plugin(self, path: str) -> Plugin:

        description = toml.load(os.path.join(path, "plugin.toml"))
        main = description["main"]
        name = description["name"]

        print(f"Loading plugin: {name}")

        pos = main.rfind('.')
        module_name = str(os.path.basename(path)) + "." + main[:pos]
        class_name = main[pos + 1:]

        module = importlib.import_module(module_name)
        plugin = getattr(module, class_name)()
        return plugin

    def load_plugins(self, directory: str) -> list[Plugin]:
        assert directory is not None, "Directory cannot be None"
        assert os.path.isdir(directory), f"Path {directory} is not a directory"

        results = []
        # print(f"Loading plugins in {directory}...")
        for entry in Path(directory).iterdir():
            if not entry.is_dir():
                continue

            if not (entry / "plugin.toml").exists():
                continue

            try:
                plugin = self.load_plugin(str(entry.absolute()))
                results.append(plugin)
                self._plugins.append(plugin)
                plugin.on_load()
            except Exception as e:
                print(f"Could not load plugin in {entry}: {e}")

        return results

    def enable_plugins(self):
        for plugin in self._plugins:
            plugin.on_enable()
