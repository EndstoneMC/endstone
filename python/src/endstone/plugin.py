import typing

from endstone._internal.endstone_python import PluginDescription, PluginLoader, PluginManager
from endstone._internal.endstone_python import Plugin as _Plugin
from endstone._internal.endstone_python import PluginCommand as _PluginCommand
from endstone.command import Command, CommandExecutor

__all__ = [
    "Plugin",
    "PluginCommand",
    "PluginDescription",
    "PluginLoader",
    "PluginManager",
]


class PluginCommand(_PluginCommand):
    def __init__(self, command: Command, owner: _Plugin):
        _PluginCommand.__init__(self, command, owner)
        self._executor: CommandExecutor = owner  # keep it alive

    def _get_executor(self):
        return self._executor

    def _set_executor(self, executor):
        self._executor = executor

    @property
    def executor(self) -> CommandExecutor:
        """
        The CommandExecutor to run when parsing this command
        """
        return self._get_executor()

    @executor.setter
    def executor(self, executor: CommandExecutor) -> None:
        self._set_executor(executor)


class Plugin(_Plugin):
    def __init__(self):
        _Plugin.__init__(self)
        self._description: PluginDescription | None = None
        self._plugin_commands: list[PluginCommand] = []  # keep them alive

    def register_command(self, command_type: typing.Type[Command]) -> PluginCommand:
        command = PluginCommand(command_type(), self)
        self._plugin_commands.append(command)
        return self.server.register_plugin_command(command)

    def _get_description(self) -> PluginDescription:
        return self._description

    @property
    def description(self) -> PluginDescription:
        """
        Returns the details of this plugin
        """
        # noinspection PyUnresolvedReferences
        return self._get_description()
