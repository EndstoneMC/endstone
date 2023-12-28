import importlib.util
import sys
from importlib.machinery import ModuleSpec
from pathlib import Path
from types import ModuleType

from endstone._internal.plugin_description_file import PluginDescriptionFile
from endstone.plugin import PluginDescription, PluginLoader, Plugin


def _create_plugin(module: ModuleType, class_name: str, description: PluginDescription) -> Plugin:
    plugin = getattr(module, class_name)()
    assert isinstance(plugin, Plugin), f"Main class {class_name} does not extend endstone.plugin.Plugin"
    plugin._description = description  # noinspection PyProtectedMember
    return plugin


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


def _load_module_from_spec(spec: ModuleSpec) -> ModuleType:
    if spec.name in sys.modules:
        return sys.modules[spec.name]
    else:
        module = importlib.util.module_from_spec(spec)
        sys.modules[spec.name] = module
        spec.loader.exec_module(module)
        return module


class SourcePluginLoader(PluginLoader):
    # noinspection PyMethodMayBeStatic
    def get_plugin_file_filters(self) -> list[str]:
        return [r"plugin\.toml$"]

    def load_plugin(self, file) -> Plugin:
        assert file is not None, "File cannot be None"

        file = Path(file)
        dir_name = file.resolve().parent / "src"

        with open(file, "rb") as f:
            description = PluginDescriptionFile(f)

        module_name, class_name = description.main.split(":")

        spec = _get_module_spec(module_name, dir_name)
        if not spec:
            raise ModuleNotFoundError(module_name)

        module = _load_module_from_spec(spec)

        return _create_plugin(module, class_name, description)
