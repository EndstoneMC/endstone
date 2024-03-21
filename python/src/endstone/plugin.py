import typing

from endstone._internal.endstone_python import Plugin as _Plugin
from endstone._internal.endstone_python import PluginCommand as _PluginCommand
from endstone._internal.endstone_python import (
    Command,
    CommandSender,
    PluginDescription,
    PluginLoader,
    PluginLoadOrder,
    PluginManager,
)
from endstone.command import CommandExecutor

__all__ = [
    "Plugin",
    "PluginCommand",
    "PluginDescription",
    "PluginLoader",
    "PluginLoadOrder",
    "PluginManager",
]


class PluginCommand(Command):
    def __init__(self, impl: _PluginCommand) -> None:
        Command.__init__(self, impl.name)
        self._impl = impl
        self._executor: CommandExecutor | None = None

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
    def plugin(self) -> _Plugin:
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


class Plugin(_Plugin):
    # Metadata
    name = None
    version = None

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
        self._plugin_commands: dict[_PluginCommand, PluginCommand] = {}

    def _get_description(self) -> PluginDescription:
        return self._description

    def get_command(self, name) -> typing.Optional[PluginCommand]:
        command = _Plugin.get_command(self, name)
        if command is None:
            return None

        if command in self._plugin_commands:
            return self._plugin_commands[command]

        wrapped_command = PluginCommand(command)
        self._plugin_commands[command] = wrapped_command
        return wrapped_command
