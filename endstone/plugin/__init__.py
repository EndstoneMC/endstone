import inspect
import os
import shutil
from pathlib import Path

import lazy_loader as lazy
import tomlkit
from importlib_resources import as_file, files

from endstone._python.plugin import Plugin as _Plugin
from endstone._python.plugin import PluginDescription
from endstone.event import Event


class Plugin(_Plugin):
    # Metadata
    version = None
    api_version = None

    # Optional metadata
    description = None
    load = None
    authors = None
    contributors = None
    website = None
    prefix = None

    # Dependencies
    provides = None
    depend = None
    soft_depend = None
    load_before = None

    # Command
    commands = None

    # Permissions
    default_permission = None
    permissions = None

    def __init__(self):
        _Plugin.__init__(self)
        self._description: PluginDescription | None = None
        self._config = None
        self._listeners = []

    def _get_description(self) -> PluginDescription:
        return self._description

    def register_events(self, listener: object) -> None:
        """Registers all events defined in the given listener instance.

        Args:
            listener (object): The listener object containing event handler methods
                to be registered.
        """
        if not self.is_enabled:
            raise RuntimeError(f"Plugin {self.name} attempted to register events while not enabled")

        if listener is None:
            raise ValueError("Listener cannot be None")

        self._listeners.append(listener)

        for attr_name in dir(listener):
            if attr_name == "config":
                continue

            func = getattr(listener, attr_name)
            if not callable(func) or not getattr(func, "_is_event_handler", False):
                continue

            sig = inspect.signature(func)
            params = list(sig.parameters.values())
            if (
                len(params) != 1
                or not inspect.isclass(params[0].annotation)
                or not issubclass(params[0].annotation, Event)
            ):
                self.logger.error(
                    f"Plugin {self.name} attempted to register an invalid event handler signature: {attr_name}: {sig}"
                )
                continue

            event_cls = params[0].annotation
            priority = getattr(func, "_priority")
            ignore_cancelled = getattr(func, "_ignore_cancelled")
            self.server.plugin_manager.register_event(
                getattr(event_cls, "NAME", event_cls.__name__), func, priority, self, ignore_cancelled
            )

    @property
    def config(self) -> dict:
        """
        Returns the plugin's configuration loaded from config.toml.

        Loads and returns the plugin’s configuration data from ``config.toml``.
        If the configuration has not been loaded yet, it is automatically
        reloaded using ``reload_config()``.

        Returns:
            dict: The plugin's configuration data.
        """
        if self._config is None:
            self._config = self.reload_config()

        return self._config

    def reload_config(self) -> dict:
        """
        Returns the plugin's configuration loaded from config.toml.

        Loads and returns the plugin’s configuration data from ``config.toml``.
        If a default configuration file is packaged with the plugin, its values
        are used as defaults.

        Returns:
            dict: The plugin's configuration data.
        """
        with (Path(self.data_folder) / "config.toml").open("r", encoding="utf-8") as f:
            self._config = tomlkit.load(f)

        return self._config

    def save_config(self) -> None:
        """
        Saves the current configuration to the config.toml file.
        """
        if self._config is None:
            return

        with (Path(self.data_folder) / "config.toml").open("w", encoding="utf-8") as f:
            tomlkit.dump(self._config, f)

    def save_default_config(self) -> None:
        """
        Saves the default config.toml file to the plugin's data folder.

        If ``config.toml`` does not already exist in the plugin’s data folder,
        this method copies the default version from the plugin’s packaged
        resources. If the file already exists, the method does nothing and
        fails silently.
        """
        if not (Path(self.data_folder) / "config.toml").exists():
            self.save_resources("config.toml", False)

    def save_resources(self, path: str, replace: bool = False) -> None:
        """
        Saves a packaged resource from the plugin module to the data folder.

        The method locates a resource bundled within the plugin’s package and
        copies it to the plugin’s data folder, preserving its relative directory
        structure. If the target file already exists, it will only be replaced
        if ``replace`` is set to True.

        Args:
            path (str): The relative path to the resource inside the plugin’s package.
                Directory separators are normalized to forward slashes.
            replace (bool, optional): Whether to overwrite the existing file if it
                already exists. Defaults to False.

        Raises:
            FileNotFoundError: If the specified resource cannot be found in the package.
            OSError: If an error occurs while copying or creating directories.
        """
        path = path.replace(os.pathsep, "/")
        out_path = Path(self.data_folder) / path
        if not out_path.exists() or replace:
            out_path.parent.mkdir(exist_ok=True)
            resource = files(self.__class__.__module__).joinpath(path)
            with as_file(resource) as f:
                shutil.copy(f, out_path)
        else:
            self.logger.warning(f"Could not save {out_path.name} to {out_path}: file already exists.")


__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "plugin": [
            "PluginCommand",
            "PluginLoadOrder",
            "PluginLoader",
            "PluginManager",
            "Service",
            "ServiceManager",
            "ServicePriority",
        ]
    },
)

__all__.extend(["Plugin", "PluginDescription"])
