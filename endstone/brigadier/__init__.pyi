"""
Classes of Mojang's Brigadier command library.
"""

import collections.abc
import typing

from endstone.command import CommandSourceStack

__all__ = [
    "ArgumentCommandNode",
    "ArgumentType",
    "BoolArgumentType",
    "CommandContext",
    "CommandDispatcher",
    "CommandNode",
    "CommandSyntaxException",
    "DoubleArgumentType",
    "FloatArgumentType",
    "IntegerArgumentType",
    "LiteralArgumentBuilder",
    "LiteralCommandNode",
    "RequiredArgumentBuilder",
    "RootCommandNode",
    "StringArgumentType",
]

class CommandSyntaxException(Exception): ...

class ArgumentType:
    """
    Turns the text of an argument into its value.
    """
    @property
    def examples(self) -> list[str]:
        """
        Examples of valid input.
        """

class StringArgumentType(ArgumentType):
    """
    An argument whose value is a string.
    """
    @staticmethod
    def word() -> StringArgumentType:
        """
        A single word of letters, digits and `_-.+`.
        """

    @staticmethod
    def string() -> StringArgumentType:
        """
        A single word, or a phrase in quotes.
        """

    @staticmethod
    def greedy_string() -> StringArgumentType:
        """
        The rest of the input, which must be the last argument.
        """

class BoolArgumentType(ArgumentType):
    """
    An argument whose value is `True` or `False`.
    """
    @staticmethod
    def bool() -> BoolArgumentType:
        """
        Either `true` or `false`.
        """

class IntegerArgumentType(ArgumentType):
    """
    An argument whose value is an integer.
    """
    @staticmethod
    def integer(min: int = -2147483648, max: int = 2147483647) -> IntegerArgumentType:
        """
        An integer between `min` and `max`, both included.

        Args:
            min: The smallest value accepted.
            max: The largest value accepted.

        Returns:
            The argument type.
        """

    @property
    def minimum(self) -> int:
        """
        The smallest value accepted.
        """

    @property
    def maximum(self) -> int:
        """
        The largest value accepted.
        """

    def __eq__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...

class FloatArgumentType(ArgumentType):
    """
    An argument whose value is a float.
    """
    @staticmethod
    def float_arg(min: float = ..., max: float = ...) -> FloatArgumentType:
        """
        A float between `min` and `max`, both included.

        Args:
            min: The smallest value accepted.
            max: The largest value accepted.

        Returns:
            The argument type.
        """

    @property
    def minimum(self) -> float:
        """
        The smallest value accepted.
        """

    @property
    def maximum(self) -> float:
        """
        The largest value accepted.
        """

    def __eq__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...

class DoubleArgumentType(ArgumentType):
    """
    An argument whose value is a double.
    """
    @staticmethod
    def double_arg(min: float = ..., max: float = ...) -> DoubleArgumentType:
        """
        A double between `min` and `max`, both included.

        Args:
            min: The smallest value accepted.
            max: The largest value accepted.

        Returns:
            The argument type.
        """

    @property
    def minimum(self) -> float:
        """
        The smallest value accepted.
        """

    @property
    def maximum(self) -> float:
        """
        The largest value accepted.
        """

    def __eq__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...

class CommandContext:
    """
    What a command receives: its source and its parsed arguments.
    """
    @property
    def source(self) -> CommandSourceStack:
        """
        The source the command runs with.
        """

    @property
    def input(self) -> str:
        """
        The input of the command.
        """

    def get_argument(self, name: str) -> object:
        """
        Gets the value of an argument.

        Args:
            name: The name of the argument.

        Returns:
            The value of the argument.

        Raises:
            ValueError: If the command has no argument with that name.
        """

class CommandNode:
    """
    A node of a command tree.
    """
    @property
    def name(self) -> str:
        """
        The name of this node.
        """

    @property
    def children(self) -> list[CommandNode]:
        """
        The nodes below this node.
        """

    @property
    def command(self) -> collections.abc.Callable[[CommandContext], int] | None:
        """
        The code that runs when this node ends the input, or `None`.
        """

    def can_use(self, source: CommandSourceStack) -> bool:
        """
        Checks whether a source passes the requirement of this node.

        Args:
            source: The source of the command.

        Returns:
            `True` if the source can use this node.
        """

class LiteralCommandNode(CommandNode):
    """
    A node that matches a keyword.
    """
    @property
    def literal(self) -> str:
        """
        The keyword.
        """

class ArgumentCommandNode(CommandNode):
    """
    A node that reads an argument.
    """
    @property
    def type(self) -> ArgumentType:
        """
        The type of the argument.
        """

class RootCommandNode(CommandNode):
    """
    The root node of a dispatcher.
    """

class LiteralArgumentBuilder:
    """
    Builds a literal node.
    """
    @typing.overload
    def then(self, argument: LiteralArgumentBuilder) -> LiteralArgumentBuilder:
        """
        Adds a node below this one.

        Args:
            argument: The builder of the node to add.

        Returns:
            This builder.
        """

    @typing.overload
    def then(self, argument: RequiredArgumentBuilder) -> LiteralArgumentBuilder: ...
    @typing.overload
    def then(self, argument: CommandNode) -> LiteralArgumentBuilder: ...
    def executes(self, command: collections.abc.Callable[[CommandContext], object]) -> LiteralArgumentBuilder:
        """
        Sets the code that runs when the input ends at this node.

        Args:
            command: Takes the `CommandContext` and returns a result above 0 for success, or `None` for success. A `CommandSyntaxException` it raises is sent to the sender.

        Returns:
            This builder.
        """

    def requires(self, requirement: collections.abc.Callable[[CommandSourceStack], bool]) -> LiteralArgumentBuilder:
        """
        Sets the requirement a source must pass to see and use this node.

        Args:
            requirement: Takes the `CommandSourceStack` and returns whether it can use this node.

        Returns:
            This builder.
        """

    @property
    def literal(self) -> str:
        """
        The keyword of the node.
        """

    def build(self) -> LiteralCommandNode:
        """
        Builds the node.

        Returns:
            The node.
        """

class RequiredArgumentBuilder:
    """
    Builds an argument node.
    """
    @typing.overload
    def then(self, argument: LiteralArgumentBuilder) -> RequiredArgumentBuilder:
        """
        Adds a node below this one.

        Args:
            argument: The builder of the node to add.

        Returns:
            This builder.
        """

    @typing.overload
    def then(self, argument: RequiredArgumentBuilder) -> RequiredArgumentBuilder: ...
    @typing.overload
    def then(self, argument: CommandNode) -> RequiredArgumentBuilder: ...
    def executes(self, command: collections.abc.Callable[[CommandContext], object]) -> RequiredArgumentBuilder:
        """
        Sets the code that runs when the input ends at this node.

        Args:
            command: Takes the `CommandContext` and returns a result above 0 for success, or `None` for success. A `CommandSyntaxException` it raises is sent to the sender.

        Returns:
            This builder.
        """

    def requires(self, requirement: collections.abc.Callable[[CommandSourceStack], bool]) -> RequiredArgumentBuilder:
        """
        Sets the requirement a source must pass to see and use this node.

        Args:
            requirement: Takes the `CommandSourceStack` and returns whether it can use this node.

        Returns:
            This builder.
        """

    @property
    def name(self) -> str:
        """
        The name of the argument.
        """

    @property
    def type(self) -> ArgumentType:
        """
        The type of the argument.
        """

    def build(self) -> ArgumentCommandNode:
        """
        Builds the node.

        Returns:
            The node.
        """

class CommandDispatcher:
    """
    Parses and runs input against command trees.
    """
    def __init__(self) -> None: ...
    def register(self, command: LiteralArgumentBuilder) -> LiteralCommandNode:
        """
        Adds a command tree to this dispatcher.

        Args:
            command: The builder of the root node.

        Returns:
            The root node of the command.
        """

    def execute(self, input: str, source: CommandSourceStack) -> int:
        """
        Parses and runs a command.

        Args:
            input: The command, without a leading slash.
            source: The source to run it with.

        Returns:
            The result of the command.

        Raises:
            CommandSyntaxException: If the input does not parse or the command fails.
        """

    @property
    def root(self) -> RootCommandNode:
        """
        The root node.
        """
