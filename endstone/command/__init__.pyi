"""
Classes relating to handling specialized non-chat player input.
"""

import collections.abc
import typing

from endstone import Server
from endstone.block import Block
from endstone.lang import Translatable
from endstone.permissions import Permissible

__all__ = [
    "BlockCommandSender",
    "Command",
    "CommandExecutor",
    "CommandMap",
    "CommandSender",
    "CommandSenderWrapper",
    "ConsoleCommandSender",
    "ProxiedCommandSender",
]

class CommandSender(Permissible):
    """
    Represents a command sender.
    """
    def send_message(self, message: str | Translatable) -> None:
        """
        Sends this sender a message.

        Args:
            message: Message to be displayed.
        """

    def send_error_message(self, message: str | Translatable) -> None:
        """
        Sends this sender a error message.

        Args:
            message: Error message to be displayed.
        """

    @property
    def server(self) -> Server:
        """
        The server instance that this command is running on.
        """

    @property
    def name(self) -> str:
        """
        The name of this command sender.
        """

class BlockCommandSender(CommandSender):
    """
    Represents a block command sender.
    """
    @property
    def block(self) -> Block:
        """
        The block this command sender belongs to.
        """

class CommandSenderWrapper(CommandSender):
    """
    Represents a wrapper that forwards commands to the wrapped `CommandSender` and captures its output.
    """
    def __init__(
        self,
        sender: CommandSender,
        on_message: collections.abc.Callable[[str | Translatable], None] | None = None,
        on_error: collections.abc.Callable[[str | Translatable], None] | None = None,
    ) -> None: ...

class ConsoleCommandSender(CommandSender):
    """
    Represents a console command sender.
    """

class ProxiedCommandSender(CommandSender):
    """
    Represents a command sender that is running a command on behalf of another one, as `/execute` does.

    Output and permissions belong to the caller, while the name and the execution context belong to the callee.
    """
    @property
    def caller(self) -> CommandSender:
        """
        The `CommandSender` which triggered this proxied command.
        """

    @property
    def callee(self) -> CommandSender:
        """
        The `CommandSender` which is being used to call the command.
        """

class Command:
    """
    Represents a Command, which executes various tasks upon user input.
    """
    def __init__(
        self,
        name: str,
        description: str | None = None,
        usages: list[str] | None = None,
        aliases: list[str] | None = None,
        permissions: list[str] | None = None,
        *args: typing.Any,
        **kwargs: typing.Any,
    ) -> None: ...
    def execute(self, sender: CommandSender, args: list[str]) -> bool:
        """
        Executes the command, returning its success.

        Args:
            sender: Source of the command.
            args: Arguments passed to the command.

        Returns:
            `True` if the execution was successful, `False` otherwise.
        """

    def test_permission(self, target: CommandSender) -> bool:
        """
        Tests the given `CommandSender` to see if they can perform this command.

        If they do not have permission, they will be informed that they cannot do this.

        Args:
            target: User to test.

        Returns:
            `True` if they can use it, `False` otherwise.
        """

    def test_permission_silently(self, target: CommandSender) -> bool:
        """
        Tests the given `CommandSender` to see if they can perform this command.

        No error is sent to the sender.

        Args:
            target: User to test.

        Returns:
            `True` if they can use it, `False` otherwise.
        """

    @property
    def name(self) -> str:
        """
        The name of this command.
        """

    @name.setter
    def name(self, arg1: str) -> None: ...
    @property
    def description(self) -> str:
        """
        A brief description of this command.
        """

    @description.setter
    def description(self, arg1: str) -> None: ...
    @property
    def aliases(self) -> list[str]:
        """
        A list of aliases of this command.
        """

    @aliases.setter
    def aliases(self, arg1: list[str]) -> None: ...
    @property
    def usages(self) -> list[str]:
        """
        A list of usages of this command.
        """

    @usages.setter
    def usages(self, arg1: list[str]) -> None: ...
    @property
    def permissions(self) -> list[str]:
        """
        The permissions required by users to be able to perform this command.
        """

    @permissions.setter
    def permissions(self, arg1: list[str]) -> None: ...
    @property
    def is_registered(self) -> bool:
        """
        The current registered state of this command.
        """

class CommandMap:
    """
    Represents a command map that manages all commands of the Server.
    """
    def register_command(self, command: Command) -> bool:
        """
        Registers a command.

        Args:
            command: The command to register.

        Returns:
            `True` on success, `False` if a command with the same name is already registered.
        """

    def dispatch(self, sender: CommandSender, command_line: str) -> bool:
        """
        Looks for the requested command and executes it if found.

        Args:
            sender: The command's sender.
            command_line: The command and its arguments, e.g. "/test abc 123".

        Returns:
            `True` if the execution was successful, `False` otherwise.
        """

    def clear_commands(self) -> None:
        """
        Clears all registered commands.
        """

    def get_command(self, name: str) -> Command | None:
        """
        Gets the command registered to the specified name.

        Args:
            name: The name of the command to retrieve.

        Returns:
            The command with the specified name, `None` if a command with that label doesn't exist.
        """

class CommandExecutor:
    """
    Represents a class which contains a single method for executing commands.
    """
    def __init__(self) -> None: ...
    def on_command(self, sender: CommandSender, command: Command, args: list[str]) -> bool:
        """
        Executes the given command, returning its success.

        Args:
            sender: Source of the command.
            command: Command which was executed.
            args: Passed command arguments.

        Returns:
            `True` if the execution is successful, `False` otherwise.
        """
