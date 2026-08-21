"""
Classes relating to handling specialized non-chat player input.
"""

import collections.abc
import typing

from endstone import Server
from endstone.block import Block
from endstone.lang import Translatable
from endstone.permissions import Permissible
from endstone.plugin import Plugin

__all__ = [
    "ArgumentCommandNode",
    "ArgumentKind",
    "ArgumentType",
    "BlockCommandSender",
    "BuiltinArgumentType",
    "Command",
    "CommandContext",
    "CommandError",
    "CommandExecutor",
    "CommandMap",
    "CommandNode",
    "CommandSender",
    "CommandSenderWrapper",
    "Commands",
    "ConsoleCommandSender",
    "LiteralArgumentBuilder",
    "LiteralCommandNode",
    "ProxiedCommandSender",
    "RequiredArgumentBuilder",
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

class CommandError(Exception): ...

class ArgumentKind:
    """
    The built-in argument types Endstone can register with the client.
    """
    def __init__(self, value: int) -> None: ...

    __entries = ...
    @property
    def name(self) -> str:
        """
        name(self: object, /) -> str
        """


    __members__ = ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __getstate__(self, /) -> int: ...
    def __hash__(self, /) -> int: ...
    @property
    def value(self) -> int: ...
    def __int__(self, /) -> int: ...
    def __index__(self, /) -> int: ...
    def __setstate__(self, state: int, /) -> None: ...

    CUSTOM: int = 0
    BOOLEAN: int = 1
    INTEGER: int = 2
    FLOAT: int = 3
    STRING: int = 4
    MESSAGE: int = 5
    RAW_TEXT: int = 6
    PLAYER: int = 7
    PLAYERS: int = 8
    ENTITY: int = 9
    ENTITIES: int = 10
    BLOCK_POSITION: int = 11
    POSITION: int = 12
    BLOCK_TYPE: int = 13
    BLOCK_STATES: int = 14
    ENTITY_TYPE: int = 15
    JSON: int = 16
    INTEGER_RANGE: int = 17
    ENUMERATION: int = 18
    SOFT_ENUM: int = 19

class ArgumentType:
    """
    Describes how a command argument is parsed.
    """
    @property
    def kind(self) -> ArgumentKind:
        """
        Which built-in type this is.
        """

class BuiltinArgumentType(ArgumentType):
    """
    An argument type provided by Endstone.
    """

class CommandNode:
    """
    Represents a node in a command tree.
    """
    @property
    def name(self) -> str:
        """
        The name of this node.
        """

    @property
    def children(self) -> list[CommandNode]:
        """
        The children of this node.
        """

    @property
    def is_executable(self) -> bool:
        """
        Whether a command may end at this node.
        """

    @property
    def permissions(self) -> list[str]:
        """
        The permissions that allow this branch to be used.
        """

class LiteralCommandNode(CommandNode):
    """
    A node matched by typing a fixed word.
    """
    @property
    def description(self) -> str:
        """
        A brief description of this command.
        """

    @property
    def aliases(self) -> list[str]:
        """
        The alternative names this command is registered under.
        """

class ArgumentCommandNode(CommandNode):
    """
    A node matched by parsing a value.
    """

class LiteralArgumentBuilder:
    """
    Builds a node matched by typing a fixed word.
    """
    def description(self, description: str) -> LiteralArgumentBuilder:
        """
        Sets a brief description of this command.
        """

    def aliases(self, aliases: list[str]) -> LiteralArgumentBuilder:
        """
        Adds alternative names this command may be typed as.
        """

    def register_to(self, command_map: CommandMap, owner: Plugin) -> bool:
        """
        Registers this command tree.

        Args:
            command_map: The `CommandMap` to register to.
            owner: The plugin the command belongs to.

        Returns:
            `True` on success, `False` if a command with the same name is already registered.
        """

    @typing.overload
    def then(self, child: LiteralArgumentBuilder) -> LiteralArgumentBuilder: ...
    @typing.overload
    def then(self, child: RequiredArgumentBuilder) -> LiteralArgumentBuilder:
        """
        Adds a child branch.
        """

    def executes(self, handler: collections.abc.Callable) -> LiteralArgumentBuilder:
        """
        Sets the handler run when a command ends at this node.

        Returning normally reports the command as successful; raise a `CommandError` to report a
        failure to the sender.
        """

    def permission(self, permission: str) -> LiteralArgumentBuilder:
        """
        Requires a permission to use this branch.

        A sender holding any one of the permissions added here passes. A sender without one is told
        they lack permission, and the branch is hidden from their client.
        """

    def requires(self, predicate: collections.abc.Callable) -> LiteralArgumentBuilder:
        """
        Requires a predicate to pass to use this branch.

        Unlike `permission`, a sender that fails is not told why, and the branch is simply absent.
        Keep the predicate cheap: it runs once per node per parse, and again for every node each time
        the command tree is sent to a player.
        """

    def build(self) -> LiteralCommandNode:
        """
        Returns the node this builder has shaped.
        """

class RequiredArgumentBuilder:
    """
    Builds a node matched by parsing a value.
    """
    @typing.overload
    def then(self, child: LiteralArgumentBuilder) -> RequiredArgumentBuilder: ...
    @typing.overload
    def then(self, child: RequiredArgumentBuilder) -> RequiredArgumentBuilder:
        """
        Adds a child branch.
        """

    def executes(self, handler: collections.abc.Callable) -> RequiredArgumentBuilder:
        """
        Sets the handler run when a command ends at this node.

        Returning normally reports the command as successful; raise a `CommandError` to report a
        failure to the sender.
        """

    def permission(self, permission: str) -> RequiredArgumentBuilder:
        """
        Requires a permission to use this branch.

        A sender holding any one of the permissions added here passes. A sender without one is told
        they lack permission, and the branch is hidden from their client.
        """

    def requires(self, predicate: collections.abc.Callable) -> RequiredArgumentBuilder:
        """
        Requires a predicate to pass to use this branch.

        Unlike `permission`, a sender that fails is not told why, and the branch is simply absent.
        Keep the predicate cheap: it runs once per node per parse, and again for every node each time
        the command tree is sent to a player.
        """

    def build(self) -> ArgumentCommandNode:
        """
        Returns the node this builder has shaped.
        """

class Commands:
    """
    Entry points for building a command tree.
    """
    @staticmethod
    def literal(name: str) -> LiteralArgumentBuilder:
        """
        Begins a branch matched by typing a fixed word.
        """

    @staticmethod
    def argument(name: str, type: ArgumentType) -> RequiredArgumentBuilder:
        """
        Begins a branch matched by parsing a value.
        """

class CommandContext:
    """
    The arguments and sender a command handler is invoked with.
    """
    @property
    def sender(self) -> CommandSender:
        """
        The source of this command.
        """

    @property
    def command(self) -> Command:
        """
        The command being executed.
        """

    @property
    def argument_names(self) -> list[str]:
        """
        The names of the arguments bound on the branch that ran.
        """

    def __contains__(self, name: str) -> bool: ...
    def __getitem__(self, name: str) -> object: ...
    def get(self, name: str, default: object | None = None) -> object:
        """
        Returns the value of the named argument, or the default if it was omitted.
        """
