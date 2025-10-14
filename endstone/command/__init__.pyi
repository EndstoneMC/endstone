"""
Classes relating to handling specialized non-chat player input.
"""

import collections

from endstone import Server
from endstone.block import Block
from endstone.lang import Translatable
from endstone.permissions import Permissible

__all__ = [
    "BlockCommandSender",
    "Command",
    "CommandExecutor",
    "CommandSender",
    "CommandSenderWrapper",
    "ConsoleCommandSender",
]

class CommandSender(Permissible):
    """
    Represents a command sender.
    """
    def send_message(self, message: str | Translatable) -> None:
        """
        Sends this sender a message
        """
        ...
    def send_error_message(self, message: str | Translatable) -> None:
        """
        Sends this sender an error message
        """
        ...
    @property
    def server(self) -> Server:
        """
        Returns the server instance that this command is running on
        """
        ...
    @property
    def name(self) -> str:
        """
        Gets the name of this command sender
        """
        ...

class BlockCommandSender(CommandSender):
    """
    Represents a block command sender.
    """
    @property
    def block(self) -> Block:
        """
        Returns the block this command sender belongs to
        """
        ...

class CommandSenderWrapper(CommandSender):
    """
    Represents a wrapper that forwards commands to the wrapped CommandSender and captures its output
    """
    def __init__(
        self,
        sender: CommandSender,
        on_message: collections.abc.Callable[[str | Translatable], None] = None,
        on_error: collections.abc.Callable[[str | Translatable], None] = None,
    ) -> None: ...

class ConsoleCommandSender(CommandSender):
    """
    Represents a console command sender.
    """

class Command:
    """
    Represents a Command, which executes various tasks upon user input
    """
    def __init__(
        self,
        name: str,
        description: str | None = None,
        usages: list[str] | None = None,
        aliases: list[str] | None = None,
        permissions: list[str] | None = None,
        *args,
        **kwargs,
    ) -> None: ...
    def execute(self, sender: CommandSender, args: list[str]) -> bool:
        """
        Executes the command, returning its success
        """
        ...
    def test_permission(self, target: CommandSender) -> bool:
        """
        Tests the given CommandSender to see if they can perform this command.
        """
        ...
    def test_permission_silently(self, target: CommandSender) -> bool:
        """
        Tests the given CommandSender to see if they can perform this command. No error is sent to the sender.
        """
        ...
    @property
    def name(self) -> str:
        """
        Name of this command.
        """
        ...
    @name.setter
    def name(self, arg1: str) -> None: ...
    @property
    def description(self) -> str:
        """
        Brief description of this command
        """
        ...
    @description.setter
    def description(self, arg1: str) -> None: ...
    @property
    def aliases(self) -> list[str]:
        """
        List of aliases of this command
        """
        ...
    @aliases.setter
    def aliases(self, arg1: list[str]) -> None: ...
    @property
    def usages(self) -> list[str]:
        """
        List of usages of this command
        """
        ...
    @usages.setter
    def usages(self, arg1: list[str]) -> None: ...
    @property
    def permissions(self) -> list[str]:
        """
        The permissions required by users to be able to perform this command
        """
        ...
    @permissions.setter
    def permissions(self, arg1: list[str]) -> None: ...
    @property
    def is_registered(self) -> bool:
        """
        Returns the current registered state of this command
        """
        ...

class CommandExecutor:
    """
    Represents a class which contains a single method for executing commands
    """
    def __init__(self) -> None: ...
    def on_command(self, sender: CommandSender, command: Command, args: list[str]) -> bool:
        """
        Executes the given command, returning its success.
        """
        ...
