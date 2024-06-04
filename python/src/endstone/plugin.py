import inspect
import os
import shutil
import typing
from importlib_resources import files, as_file
from pathlib import Path
import tomlkit
from endstone._internal.endstone_python import (
    PluginDescription,
    PluginLoader,
    PluginLoadOrder,
    PluginManager,
)
from endstone._internal import endstone_python
from endstone.command import Command, CommandExecutor, CommandSender
from endstone.event import Event

__all__ = [
    "Plugin",
    "PluginCommand",
    "PluginDescription",
    "PluginLoader",
    "PluginLoadOrder",
    "PluginManager",
]


class PluginCommand(Command):
    def __init__(self, impl: endstone_python.PluginCommand) -> None:
        Command.__init__(self, impl.name)
        self._impl = impl
        self._executor: typing.Optional[CommandExecutor] = None

    @property
    def executor(self) -> CommandExecutor:
        """
        Gets the CommandExecutor to run when parsing this command
        """
        return self._impl.executor

    @executor.setter
    def executor(self, executor: CommandExecutor) -> None:
        """
        Sets the CommandExecutor to run when parsing this command
        """
        self._executor = executor
        self._impl.executor = executor

    @property
    def plugin(self) -> endstone_python.Plugin:
        """
        The owner of this PluginCommand
        """
        return self._impl.plugin

    def execute(self, sender: CommandSender, args: list[str]) -> bool:
        return self._impl.execute(sender, args)

    def test_permission(self, target: CommandSender) -> bool:
        return self._impl.test_permission(target)

    def test_permission_silently(self, target: CommandSender) -> bool:
        return self._impl.test_permission_silently(target)

    @property
    def aliases(self) -> list[str]:
        return self._impl.aliases

    @aliases.setter
    def aliases(self, aliases: list[str]) -> None:
        self._impl.aliases = aliases

    @property
    def description(self) -> str:
        return self._impl.description

    @description.setter
    def description(self, description: str) -> None:
        self._impl.description = description

    @property
    def name(self) -> str:
        return self._impl.name

    @name.setter
    def name(self, name: str) -> None:
        self._impl.name = name

    @property
    def permissions(self) -> list[str]:
        return self._impl.permissions

    @permissions.setter
    def permissions(self, permissions: list[str]) -> None:
        self._impl.permissions = permissions

    @property
    def registered(self) -> bool:
        return self._impl.registered

    @property
    def usages(self) -> list[str]:
        return self._impl.usages

    @usages.setter
    def usages(self, usages: list[str]) -> None:
        self._impl.usages = usages


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
        self._plugin_commands: dict[endstone_python.PluginCommand, PluginCommand] = {}
        self._config = None

    def _get_description(self) -> PluginDescription:
        return self._description

    def get_command(self, name) -> typing.Optional[PluginCommand]:
        command = endstone_python.Plugin.get_command(self, name)
        if command is None:
            return None

        if command in self._plugin_commands:
            return self._plugin_commands[command]

        wrapped_command = PluginCommand(command)
        self._plugin_commands[command] = wrapped_command
        return wrapped_command

    def register_events(self, listener: object) -> None:
        if not self.enabled:
            raise RuntimeError(f"Plugin {self.name} attempted to register events while not enabled")

        if listener is None:
            raise ValueError("Listener cannot be None")

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
                    f"Plugin {self.name} attempted to register an invalid "
                    f"event handler signature: {attr_name}: {sig}"
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
        with (Path(self.data_folder) / "config.toml").open("r") as f:
            self._config = tomlkit.load(f)

        return self._config

    def save_config(self) -> None:
        if self._config is None:
            return

        with (Path(self.data_folder) / "config.toml").open("w") as f:
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
