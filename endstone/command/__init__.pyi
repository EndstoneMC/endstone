"""
Classes relating to handling specialized non-chat player input.
"""

import collections.abc
import typing

from endstone import Player, Server
from endstone.actor import Actor
from endstone.block import Block
from endstone.brigadier import ArgumentType, LiteralArgumentBuilder, RequiredArgumentBuilder
from endstone.lang import Translatable
from endstone.level import Location
from endstone.permissions import Permissible
from endstone.util import Vector

__all__ = [
    "ActorArgumentType",
    "ActorSelectorArgumentResolver",
    "ArgumentTypes",
    "BasicCommand",
    "BlockCommandSender",
    "BlockPositionArgumentType",
    "BlockPositionResolver",
    "Command",
    "CommandExecutor",
    "CommandMap",
    "CommandSender",
    "CommandSenderWrapper",
    "CommandSourceStack",
    "Commands",
    "ConsoleCommandSender",
    "DimensionArgumentType",
    "FinePositionArgumentType",
    "FinePositionResolver",
    "PlayerArgumentType",
    "PlayerSelectorArgumentResolver",
    "ProxiedCommandSender",
    "ResourceArgumentType",
    "SoftEnum",
    "SoftEnumArgumentType",
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

class CommandSourceStack:
    """
    The source of a command run through a Brigadier command tree, carrying who ran it and where.
    """
    @property
    def sender(self) -> CommandSender:
        """
        The command sender that triggered the command.
        """

    @property
    def executor(self) -> Actor | None:
        """
        The actor that executes the command, which differs from the sender after `/execute as`, or `None`.
        """

    @property
    def location(self) -> Location:
        """
        A copy of the location where the command is executed.
        """

class PlayerSelectorArgumentResolver:
    """
    Resolves a player selector argument to the players it selects.
    """
    def resolve(self, source: CommandSourceStack) -> list[Player]:
        """
        Resolves the argument with the given command source stack.

        Args:
            source: The source of the command.

        Returns:
            The players the selector selects.

        Raises:
            CommandSyntaxException: If the argument takes one player and the selector selects none or more than one.
            ValueError: If the source was not created by the server.
        """

class ActorSelectorArgumentResolver:
    """
    Resolves an actor selector argument to the actors it selects.
    """
    def resolve(self, source: CommandSourceStack) -> list[Actor]:
        """
        Resolves the argument with the given command source stack.

        Args:
            source: The source of the command.

        Returns:
            The actors the selector selects.

        Raises:
            CommandSyntaxException: If the argument takes one actor and the selector selects none or more than one.
            ValueError: If the source was not created by the server.
        """

class BlockPositionResolver:
    """
    Resolves a block position argument, with relative and local coordinates taken from the source of the command.
    """
    def resolve(self, source: CommandSourceStack) -> Vector:
        """
        Resolves the argument with the given command source stack.

        Args:
            source: The source of the command.

        Returns:
            The block position.

        Raises:
            ValueError: If the source was not created by the server.
        """

class FinePositionResolver:
    """
    Resolves a fine position argument, with relative and local coordinates taken from the source of the command.
    """
    def resolve(self, source: CommandSourceStack) -> Vector:
        """
        Resolves the argument with the given command source stack.

        Args:
            source: The source of the command.

        Returns:
            The position.

        Raises:
            ValueError: If the source was not created by the server.
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

    def register_to(self, command_map: CommandMap) -> bool:
        """
        Registers this command to a `CommandMap`.

        Args:
            command_map: The `CommandMap` to register to.

        Returns:
            `True` if the registration was successful, `False` otherwise.
        """

    def unregister_from(self, command_map: CommandMap) -> bool:
        """
        Unregisters this command from a `CommandMap`.

        Args:
            command_map: The `CommandMap` to unregister from.

        Returns:
            `True` if the unregistration was successful, `False` otherwise.
        """

class SoftEnum:
    """
    Represents a soft enum, a set of command completions that can change while the server runs.
    """
    @property
    def name(self) -> str:
        """
        The identifier the client sees, unique across the server.
        """

    @property
    def values(self) -> list[str]:
        """
        The completions this soft enum offers.
        """

    def set_values(self, values: list[str]) -> None:
        """
        Replaces the values of this soft enum and updates every client's completion at once.

        Args:
            values: The new values.

        Raises:
            ValueError: If this soft enum is read-only or a value is empty or holds whitespace.
            RuntimeError: If the plugin that owned this soft enum has removed it.
        """

    def add_values(self, values: list[str]) -> None:
        """
        Adds values to this soft enum and updates every client's completion at once.

        Args:
            values: The values to add.

        Raises:
            ValueError: If this soft enum is read-only or a value is empty or holds whitespace.
            RuntimeError: If the plugin that owned this soft enum has removed it.
        """

    def remove_values(self, values: list[str]) -> None:
        """
        Removes values from this soft enum and updates every client's completion at once.

        Args:
            values: The values to remove.

        Raises:
            ValueError: If this soft enum is read-only or a value is empty or holds whitespace.
            RuntimeError: If the plugin that owned this soft enum has removed it.
        """

class BasicCommand:
    """
    A command that takes its arguments as words, registered through `Commands.register_command`.
    """
    def __init__(self) -> None: ...
    def execute(self, source: CommandSourceStack, args: list[str]) -> None:
        """
        Executes the command with the given source and arguments.

        A `CommandSyntaxException` raised here is sent to the sender as an error.

        Args:
            source: The source of the command.
            args: The words typed after the label.
        """

    def can_use(self, sender: CommandSender) -> bool:
        """
        Checks whether a command sender can receive and run the root command.

        Args:
            sender: The command sender.

        Returns:
            `True` if the sender has `permission()` or no permission is set.
        """

    def permission(self) -> str | None:
        """
        Returns the permission for the root command used in `can_use` by default.

        Returns:
            The permission, or `None` if none is required.
        """

class SoftEnumArgumentType(ArgumentType):
    """
    An argument completed from a soft enum, whose value is the word typed.
    """
    @property
    def name(self) -> str:
        """
        The name of the soft enum this argument completes from.
        """

class PlayerArgumentType(ArgumentType):
    """
    A player selector or a player name, resolved through a `PlayerSelectorArgumentResolver`.
    """

class ActorArgumentType(ArgumentType):
    """
    An actor selector or a player name, resolved through an `ActorSelectorArgumentResolver`.
    """

class BlockPositionArgumentType(ArgumentType):
    """
    A block position of three integer, relative or local coordinates, resolved through a `BlockPositionResolver`.
    """

class FinePositionArgumentType(ArgumentType):
    """
    A position of three decimal, relative or local coordinates, resolved through a `FinePositionResolver`.
    """

class DimensionArgumentType(ArgumentType):
    """
    A dimension of the level, completed from the server's dimension names, including those plugins create.
    """

class ResourceArgumentType(ArgumentType):
    """
    An entry of a registry, completed from the server's names for it.
    """

class ArgumentTypes:
    """
    The argument types Endstone adds to Brigadier's own.
    """
    @staticmethod
    def player() -> PlayerArgumentType:
        """
        A selector or player name for exactly one player, resolved through a `PlayerSelectorArgumentResolver`.

        Returns:
            The argument type.
        """

    @staticmethod
    def players() -> PlayerArgumentType:
        """
        A selector or player name for any number of players, resolved through a `PlayerSelectorArgumentResolver`.

        Returns:
            The argument type.
        """

    @staticmethod
    def actor() -> ActorArgumentType:
        """
        A selector or player name for exactly one actor, resolved through an `ActorSelectorArgumentResolver`.

        Returns:
            The argument type.
        """

    @staticmethod
    def actors() -> ActorArgumentType:
        """
        A selector or player name for any number of actors, resolved through an `ActorSelectorArgumentResolver`.

        Returns:
            The argument type.
        """

    @staticmethod
    def block_position() -> BlockPositionArgumentType:
        """
        A block position, resolved through a `BlockPositionResolver`.

        Returns:
            The argument type.
        """

    @staticmethod
    def fine_position() -> FinePositionArgumentType:
        """
        A fine position, resolved through a `FinePositionResolver`.

        Returns:
            The argument type.
        """

    @staticmethod
    def dimension() -> DimensionArgumentType:
        """
        A dimension, whose value is a `Dimension`.

        Returns:
            The argument type.
        """

    @staticmethod
    def resource(type: type) -> ResourceArgumentType:
        """
        An entry of a registry, whose value is an instance of the registry type.

        Args:
            type: The registry type, which is `BlockType`, `ItemType` or `ActorType`.

        Returns:
            The argument type.

        Raises:
            ValueError: If the type is not `BlockType`, `ItemType` or `ActorType`.
        """

    @staticmethod
    def soft_enum(name: str) -> SoftEnumArgumentType:
        """
        An argument completed from a soft enum, whose value is the word typed as a string.

        The server accepts any word, so the command checks the value itself. The name must be a registered soft enum.
        Soft enums are named for their content in CamelCase, as the server names its own.

        Args:
            name: The name of a soft enum a plugin, Endstone or the server owns.

        Returns:
            The argument type.
        """

class Commands:
    """
    The registrar for a plugin's commands and soft enums.
    """
    @staticmethod
    def literal(name: str) -> LiteralArgumentBuilder:
        """
        Creates a literal node builder, the keyword a command or a subcommand starts with.

        Args:
            name: The keyword.

        Returns:
            The builder.
        """

    @staticmethod
    def argument(name: str, type: ArgumentType) -> RequiredArgumentBuilder:
        """
        Creates an argument node builder.

        Args:
            name: The name the command reads the argument back by.
            type: The type of the argument.

        Returns:
            The builder.
        """

class CommandMap:
    """
    Represents a command map that manages all commands of the Server.
    """
    @typing.overload
    def register_command(self, command: Command) -> bool:
        """
        Registers a command, using the lowercased plugin name as the fallback prefix for a `PluginCommand` and `endstone` otherwise.

        Args:
            command: The command to register.

        Returns:
            `True` if the command was registered under its own name, `False` if the name was already taken.
        """

    @typing.overload
    def register_command(self, fallback_prefix: str, command: Command) -> bool:
        """
        Registers a command under its name and under `fallback_prefix:name`.

        If the name is already taken, the command is only reachable as `fallback_prefix:name` and its name changes to that. Its aliases are registered the same way, and an alias that is already taken is dropped. A `fallback_prefix:name` that another command already holds is taken over. Built-in Endstone commands use the fallback prefix `endstone`.

        Args:
            fallback_prefix: A prefix which is prepended to the command name for a unique address.
            command: The command to register.

        Returns:
            `True` if the command was registered under its own name, `False` if the name was already taken.
        """

    def dispatch(self, sender: CommandSender, command_line: str) -> bool:
        """
        Looks for the requested command and executes it if found.

        Args:
            sender: The command's sender.
            command_line: The command and its arguments, e.g. "/test abc 123".

        Returns:
            `True` if the command was found and run, `False` if no such command exists.
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

    def get_soft_enum(self, name: str) -> SoftEnum | None:
        """
        Gets the soft enum registered to the specified name, whether a plugin, Endstone or the server owns it.

        The mutators of a soft enum the server or Endstone owns raise `ValueError`.

        Args:
            name: The name of the soft enum to retrieve.

        Returns:
            The soft enum with the specified name, `None` if no soft enum has that name.
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
