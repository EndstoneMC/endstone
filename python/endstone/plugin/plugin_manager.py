import importlib
import os.path
from pathlib import Path

import toml
from endstone._logger import Logger
from endstone._plugin_manager import PluginManager as IPluginManager
from endstone._server import Server

from endstone.plugin import Plugin


class PluginManager(IPluginManager):

    def __init__(self, server: Server):
        IPluginManager.__init__(self, server)
        self._server = server
        self._plugins = []
        self._logger = Logger.get_logger(self.__class__.__name__)

    def load_plugin(self, path: str) -> Plugin:

        description = toml.load(os.path.join(path, "plugin.toml"))
        main = description["main"]
        name = description["name"]

        self._logger.info(f"Loading plugin: {name}")

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
                self._logger.error(f"Could not load plugin in {entry}: {e}")

        return results

    def enable_plugin(self, plugin: Plugin) -> None:
        if not plugin.is_enabled():
            # noinspection PyProtectedMember
            plugin._set_enabled(True)

    def enable_plugins(self) -> None:
        for plugin in self._plugins:
            self.enable_plugin(plugin)

    def disable_plugin(self, plugin: Plugin):
        if plugin.is_enabled():
            # noinspection PyProtectedMember
            plugin._set_enabled(False)

    def disable_plugins(self):
        for plugin in self._plugins:
            self.disable_plugin(plugin)

    def clear_plugins(self):
        self.disable_plugins()
        self._plugins.clear()
