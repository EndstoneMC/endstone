import importlib
import io
import zipimport
from pathlib import Path

from endstone._plugin import IPluginLoader

from .plugin import Plugin
from .plugin_description import PluginDescriptionFile

__all__ = ["ZipPluginLoader", "SourcePluginLoader"]


class PluginLoader(IPluginLoader):
    def __init__(self):
        IPluginLoader.__init__(self)

    def enable_plugin(self, plugin: Plugin) -> None:
        if not plugin.is_enabled():
            plugin.logger.info(f"Enabling {plugin.get_description().get_fullname()}")
            # noinspection PyProtectedMember
            plugin._set_enabled(True)

    def disable_plugin(self, plugin: Plugin) -> None:
        if plugin.is_enabled():
            plugin.logger.info(f"Disabling {plugin.get_description().get_fullname()}")
            # noinspection PyProtectedMember
            plugin._set_enabled(False)


class ZipPluginLoader(PluginLoader):
    _file_filters = [r"\.whl$", r"\.zip$"]

    def load_plugin(self, file: str) -> Plugin:
        assert file is not None, "File cannot be None"
        importer = zipimport.zipimporter(file)

        try:
            description = PluginDescriptionFile(io.BytesIO(importer.get_data("plugin.toml")))

        except Exception as e:
            raise RuntimeError(f"Unable to load 'plugin.toml': {e}")

        try:
            main = description.get_main()
            pos = main.rfind(".")
            module_name = main[:pos]
            class_name = main[pos + 1 :]

            module = importer.load_module(module_name)
            plugin = getattr(module, class_name)()

            assert isinstance(plugin, Plugin), f"Main class {main} does not extend endstone.plugin.Plugin"

            # noinspection PyProtectedMember
            plugin._init(description)
            return plugin
        except Exception as e:
            raise RuntimeError(f"Unable to load plugin {description.get_fullname()}: {e}")

    def get_plugin_filters(self) -> list[str]:
        return self._file_filters.copy()


class SourcePluginLoader(PluginLoader):
    _file_filters = [r"plugin\.toml$"]

    def load_plugin(self, file: str) -> Plugin:
        assert file is not None, "File cannot be None"
        file = Path(file)
        dir_name = file.parent.name

        try:
            with open(file, "rb") as f:
                description = PluginDescriptionFile(f)

        except Exception as e:
            raise RuntimeError(f"Unable to load 'plugin.toml': {e}")

        try:
            main = description.get_main()
            pos = main.rfind(".")
            module_name = main[:pos]
            class_name = main[pos + 1 :]

            module = importlib.import_module(f"{dir_name}.{module_name}")
            plugin = getattr(module, class_name)()

            assert isinstance(plugin, Plugin), f"Main class {main} does not extend endstone.plugin.Plugin"

            # noinspection PyProtectedMember
            plugin._init(description)
            return plugin
        except Exception as e:
            raise RuntimeError(f"Unable to load plugin {description.get_fullname()}: {e}")

    def get_plugin_filters(self) -> list[str]:
        return self._file_filters.copy()
