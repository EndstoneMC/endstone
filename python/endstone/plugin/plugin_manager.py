import importlib
import os.path
import sys
from pathlib import Path

from endstone.plugin.plugin_description import PluginDescriptionFile

if sys.version_info >= (3, 11):
    pass
else:
    pass

from endstone._logger import Logger
from endstone._server import Server

from endstone.plugin import Plugin


class PluginManager:
    def __init__(self, server: Server):
        self._server = server
        self._plugins = []
        self._logger = Logger.get_logger(self.__class__.__name__)

    def load_plugin(self, path: str) -> Plugin:
        try:
            with open(os.path.join(path, "plugin.toml"), "rb") as f:
                description = PluginDescriptionFile(f)
        except Exception as e:
            raise RuntimeError(f"Unable to load 'plugin.toml': {e}")

        try:
            main = description.get_main()
            pos = main.rfind(".")
            module_name = main[:pos]
            class_name = main[pos + 1 :]

            module = importlib.import_module(module_name)
            plugin = getattr(module, class_name)()

            assert isinstance(plugin, Plugin), f"Main class {main} does not extend endstone.plugin.Plugin"

            # noinspection PyProtectedMember
            plugin._init(description)
            return plugin
        except Exception as e:
            raise RuntimeError(f"Unable to load plugin {description.get_fullname()}: {e}")

    def load_plugins(self, directory: str) -> list[Plugin]:
        assert directory is not None, "Directory cannot be None"
        assert os.path.isdir(directory), f"Path {directory} is not a directory"

        results = []
        for entry in Path(directory).iterdir():
            entry = entry.absolute()

            if not entry.is_dir():
                continue

            if not (entry / "plugin.toml").exists():
                continue

            try:
                plugin = self.load_plugin(str(entry))
                results.append(plugin)
                self._plugins.append(plugin)
                plugin.on_load()
            except RuntimeError as e:
                self._logger.error(f"{e}")

        return results

    def enable_plugin(self, plugin: Plugin) -> None:
        if not plugin.is_enabled():
            plugin.logger.info(f"Enabling {plugin.get_description().get_fullname()}")
            # noinspection PyProtectedMember
            plugin._set_enabled(True)

    def enable_plugins(self) -> None:
        for plugin in self._plugins:
            self.enable_plugin(plugin)

    def disable_plugin(self, plugin: Plugin):
        if plugin.is_enabled():
            plugin.logger.info(f"Disabling {plugin.get_description().get_fullname()}")
            # noinspection PyProtectedMember
            plugin._set_enabled(False)

    def disable_plugins(self):
        for plugin in self._plugins:
            self.disable_plugin(plugin)

    def clear_plugins(self):
        self.disable_plugins()
        self._plugins.clear()
