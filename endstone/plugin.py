import inspect
import os
import shutil
import typing
from pathlib import Path

import tomlkit
from importlib_resources import as_file, files

from endstone._internal import endstone_python
from endstone._internal.endstone_python import (
    PluginCommand,
    PluginDescription,
    PluginLoader,
    PluginLoadOrder,
    PluginManager,
    Service,
    ServiceManager,
    ServicePriority,
)
from endstone.event import Event

__all__ = [
    "Plugin",
    "PluginCommand",
    "PluginDescription",
    "PluginLoader",
    "PluginLoadOrder",
    "PluginManager",
    "Service",
    "ServiceManager",
    "ServicePriority",
]


class Plugin(endstone_python.Plugin):
    # Metadata
    name = None
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
        endstone_python.Plugin.__init__(self)
        self._description: typing.Optional[PluginDescription] = None
        self._config = None
        self._listeners = []

    def _get_description(self) -> PluginDescription:
        return self._description

    def register_events(self, listener: object) -> None:
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
        if self._config is None:
            self._config = self.reload_config()

        return self._config

    def reload_config(self) -> dict:
        with (Path(self.data_folder) / "config.toml").open("r", encoding="utf-8") as f:
            self._config = tomlkit.load(f)

        return self._config

    def save_config(self) -> None:
        if self._config is None:
            return

        with (Path(self.data_folder) / "config.toml").open("w", encoding="utf-8") as f:
            tomlkit.dump(self._config, f)

    def save_default_config(self) -> None:
        if not (Path(self.data_folder) / "config.toml").exists():
            self.save_resources("config.toml", False)

    def save_resources(self, path: str, replace: bool = False) -> None:
        path = path.replace(os.pathsep, "/")
        out_path = Path(self.data_folder) / path
        if not out_path.exists() or replace:
            out_path.parent.mkdir(exist_ok=True)
            resource = files(self.__class__.__module__).joinpath(path)
            with as_file(resource) as f:
                shutil.copy(f, out_path)
        else:
            self.logger.warning(f"Could not save {out_path.name} to {out_path}: file already exists.")
            return
