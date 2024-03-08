from typing import Optional

from endstone._internal.endstone_python import Plugin as _Plugin
from endstone._internal.endstone_python import PluginCommand as _PluginCommand
from endstone._internal.endstone_python import PluginDescription, PluginLoader, PluginManager
from endstone.command import CommandExecutor

__all__ = [
    "Plugin",
    "PluginCommand",
    "PluginDescription",
    "PluginLoader",
    "PluginManager",
]


class PluginCommand(_PluginCommand):
    def __init__(
        self,
        plugin: _Plugin,
        name: str,
        description: Optional[str] = None,
        usages: list[str] = None,
        aliases: list[str] = None,
    ) -> None:
        _PluginCommand.__init__(self, plugin, name, description, usages, aliases)
        self._executor: CommandExecutor | None = None

    def _get_executor(self):
        if self._executor:
            return self._executor
        else:
            return self.plugin

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

    def register_command(
        self, name: str, description: str = "", usages: list[str] = None, aliases: list[str] = None
    ) -> PluginCommand:
        """
        Registers a new PluginCommand.
        """
        command = PluginCommand(self, name, description, usages, aliases)
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
