import importlib.util
import io
import sys
import zipimport
from importlib.machinery import ModuleSpec
from pathlib import Path
from types import ModuleType

# noinspection PyProtectedMember
from endstone._bindings import PluginLoader

from .plugin import Plugin
from .plugin_description import PluginDescriptionFile


class BasePluginLoader(PluginLoader):
    _file_filters = []

    @property
    def plugin_file_filters(self) -> list[str]:
        return self._file_filters.copy()

    def _get_plugin_file_filters(self) -> list[str]:
        return self.plugin_file_filters

    @staticmethod
    def _parse_main_class(description):
        main = description.main
        parts = main.split(".")
        class_name = parts.pop()
        module_name = ".".join(parts)
        return module_name, class_name

    @staticmethod
    def _create_plugin(module, class_name, description):
        plugin = getattr(module, class_name)()
        assert isinstance(plugin, Plugin), f"Main class {class_name} does not extend endstone.plugin.Plugin"
        plugin._description = description  # noinspection PyProtectedMember
        return plugin


class ZipPluginLoader(BasePluginLoader):
    _file_filters = [r"\.whl$", r"\.zip$"]

    def load_plugin(self, file: str) -> Plugin:
        assert file is not None, "File cannot be None"

        importer = zipimport.zipimporter(file)
        # noinspection PyTypeChecker
        fp = io.BytesIO(importer.get_data("plugin.toml"))
        description = PluginDescriptionFile(fp)

        module_name, class_name = self._parse_main_class(description)
        module = importer.load_module(module_name)

        return self._create_plugin(module, class_name, description)


class SourcePluginLoader(BasePluginLoader):
    _file_filters = [r"plugin\.toml$"]

    def load_plugin(self, file: str) -> Plugin:
        assert file is not None, "File cannot be None"

        file = Path(file)
        dir_name = file.resolve().parent

        with open(file, "rb") as f:
            description = PluginDescriptionFile(f)
        module_name, class_name = self._parse_main_class(description)

        spec = self._get_module_spec(module_name, dir_name)
        if not spec:
            raise ModuleNotFoundError(module_name)

        module = self._load_module_from_spec(spec)

        return self._create_plugin(module, class_name, description)

    @staticmethod
    def _load_module_from_spec(spec: ModuleSpec) -> ModuleType:
        if spec.name in sys.modules:
            return sys.modules[spec.name]
        else:
            module = importlib.util.module_from_spec(spec)
            sys.modules[spec.name] = module
            spec.loader.exec_module(module)
            return module

    @staticmethod
    def _get_module_spec(module_name: str, path: Path) -> ModuleSpec:
        module_parts = module_name.split(".")

        # Check for package
        package_location = path.joinpath(*module_parts, "__init__.py")
        if package_location.exists():
            return importlib.util.spec_from_file_location(module_name, package_location)

        # Check for module file
        module_file = module_parts.pop() + ".py"
        module_location = path.joinpath(*module_parts, module_file)
        if module_location.exists():
            return importlib.util.spec_from_file_location(module_name, module_location)

        raise ModuleNotFoundError(module_name)
