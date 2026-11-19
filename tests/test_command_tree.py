import pytest

from endstone.actor import ActorType
from endstone.block import BlockType
from endstone.brigadier import (
    ArgumentCommandNode,
    ArgumentType,
    BoolArgumentType,
    CommandContext,
    DoubleArgumentType,
    FloatArgumentType,
    IntegerArgumentType,
    LiteralCommandNode,
    StringArgumentType,
)
from endstone.command import (
    ActorArgumentType,
    ActorSelectorArgumentResolver,
    ArgumentTypes,
    BasicCommand,
    BlockPositionArgumentType,
    BlockPositionResolver,
    Commands,
    CommandSourceStack,
    DimensionArgumentType,
    FinePositionArgumentType,
    FinePositionResolver,
    PlayerArgumentType,
    PlayerSelectorArgumentResolver,
    ResourceArgumentType,
    SoftEnumArgumentType,
)
from endstone.inventory import ItemType
from endstone.plugin import Plugin

# ==== Builders ====


def test_builders_produce_the_expected_tree() -> None:
    """Verify literal and argument builders nest into the tree they describe."""

    def run(context: CommandContext) -> int:
        return 1

    root = (
        Commands.literal("warp")
        .then(Commands.literal("set").then(Commands.argument("name", StringArgumentType.word()).executes(run)))
        .then(Commands.argument("target", ArgumentTypes.soft_enum("WarpNames")).executes(run))
        .requires(lambda source: True)
        .build()
    )

    assert isinstance(root, LiteralCommandNode)
    assert root.literal == "warp"
    assert root.command is None
    assert [child.name for child in root.children] == ["set", "target"]

    keyword, target = root.children
    assert isinstance(keyword, LiteralCommandNode)
    assert isinstance(target, ArgumentCommandNode)
    assert isinstance(target.type, SoftEnumArgumentType)
    assert target.type.name == "WarpNames"
    assert target.command is not None

    (name,) = keyword.children
    assert isinstance(name, ArgumentCommandNode)
    assert isinstance(name.type, StringArgumentType)
    assert name.command is not None


def test_command_source_stack_cannot_be_constructed() -> None:
    """Verify CommandSourceStack has no constructor, since only the server creates sources."""
    with pytest.raises(TypeError):
        CommandSourceStack()


@pytest.mark.parametrize("name", ["word", "string", "greedy_string"])
def test_string_argument_types_are_bound(name: str) -> None:
    """Verify each string argument type constructs from Python."""
    assert isinstance(getattr(StringArgumentType, name)(), StringArgumentType)


# ==== Primitive argument types ====


@pytest.mark.parametrize(
    ("argument", "text"),
    [
        (BoolArgumentType.bool(), "bool()"),
        (IntegerArgumentType.integer(), "integer()"),
        (IntegerArgumentType.integer(-100), "integer(-100)"),
        (IntegerArgumentType.integer(-100, 100), "integer(-100, 100)"),
        (FloatArgumentType.float_arg(), "float()"),
        (FloatArgumentType.float_arg(-100), "float(-100.0)"),
        (FloatArgumentType.float_arg(-100, 100), "float(-100.0, 100.0)"),
        (DoubleArgumentType.double_arg(), "double()"),
        (DoubleArgumentType.double_arg(-100), "double(-100.0)"),
        (DoubleArgumentType.double_arg(-100, 100), "double(-100.0, 100.0)"),
    ],
)
def test_primitive_argument_types_describe_their_range(argument: ArgumentType, text: str) -> None:
    """Verify each primitive argument type prints as Brigadier's does."""
    assert isinstance(argument, ArgumentType)
    assert str(argument) == text


def test_numeric_argument_types_expose_their_bounds() -> None:
    """Verify the numeric argument types keep the bounds they were built with."""
    integer = IntegerArgumentType.integer(1, 64)
    assert (integer.minimum, integer.maximum) == (1, 64)
    assert IntegerArgumentType.integer().minimum == -(2**31)
    assert IntegerArgumentType.integer().maximum == 2**31 - 1
    assert (FloatArgumentType.float_arg(0, 1).minimum, FloatArgumentType.float_arg(0, 1).maximum) == (0.0, 1.0)
    assert (DoubleArgumentType.double_arg(-2.5, 2.5).minimum, DoubleArgumentType.double_arg(-2.5, 2.5).maximum) == (
        -2.5,
        2.5,
    )


def test_numeric_argument_types_compare_by_bounds() -> None:
    """Verify numeric argument types are equal exactly when their bounds are."""
    assert IntegerArgumentType.integer(-100, 100) == IntegerArgumentType.integer(-100, 100)
    assert IntegerArgumentType.integer(-100, 100) != IntegerArgumentType.integer(-100, 50)
    assert FloatArgumentType.float_arg(-50, 100) == FloatArgumentType.float_arg(-50, 100)
    assert DoubleArgumentType.double_arg() != DoubleArgumentType.double_arg(0)


@pytest.mark.parametrize(
    ("argument", "expected"),
    [
        (IntegerArgumentType.integer(-100, 100), -3000),
        (IntegerArgumentType.integer(1, 64), 95),
        (FloatArgumentType.float_arg(-50, 100), -1450),
        (FloatArgumentType.float_arg(), -(2**31)),
        (DoubleArgumentType.double_arg(0.5, 2.25), 17),
        (DoubleArgumentType.double_arg(), -(2**31)),
    ],
)
def test_numeric_argument_types_hash_as_brigadier_does(argument: ArgumentType, expected: int) -> None:
    """Verify numeric argument types hash to Brigadier's hashCode."""
    assert hash(argument) == expected


def test_equal_numeric_argument_types_share_a_set_entry() -> None:
    """Verify equal numeric argument types collapse to one set entry."""
    assert len({IntegerArgumentType.integer(1, 64), IntegerArgumentType.integer(1, 64)}) == 1
    assert len({FloatArgumentType.float_arg(0, 1), FloatArgumentType.float_arg(0, 1)}) == 1
    assert (
        len({DoubleArgumentType.double_arg(), DoubleArgumentType.double_arg(), DoubleArgumentType.double_arg(1)}) == 2
    )


def test_primitive_argument_types_build_into_a_tree() -> None:
    """Verify the primitive argument types can be used as tree arguments."""

    def run(context: CommandContext) -> int:
        return 1

    root = (
        Commands.literal("give")
        .then(Commands.argument("count", IntegerArgumentType.integer(1, 64)).executes(run))
        .then(Commands.argument("loud", BoolArgumentType.bool()).executes(run))
        .build()
    )
    count, loud = root.children
    assert isinstance(count.type, IntegerArgumentType)
    assert isinstance(loud.type, BoolArgumentType)
    assert count.type.examples == ["0", "123", "-123"]


# ==== BasicCommand ====


def test_basic_command_permission_defaults_to_none() -> None:
    """Verify a BasicCommand subclass needs no permission unless it returns one."""

    class Plain(BasicCommand):
        def execute(self, source, args) -> None:
            pass

    class Guarded(Plain):
        def permission(self) -> str | None:
            return "warp.use"

    assert Plain().permission() is None
    assert Guarded().permission() == "warp.use"


def test_plugin_registers_both_forms() -> None:
    """Verify Plugin binds register_command for trees and for basic commands."""
    assert callable(Plugin.register_command)
    assert "node" in Plugin.register_command.__doc__
    assert "label" in Plugin.register_command.__doc__


def test_executes_accepts_a_handler_that_returns_none() -> None:
    """Verify executes takes a handler without a return value, which counts as success when it runs."""

    def run(context: CommandContext) -> None:
        pass

    root = Commands.literal("quiet").executes(run).build()
    assert root.command is not None
    assert "`None` for success" in Commands.literal("quiet").executes.__doc__


# ==== Selector, position, dimension and resource arguments ====


@pytest.mark.parametrize(
    ("factory", "cls", "text"),
    [
        ("player", PlayerArgumentType, "player()"),
        ("players", PlayerArgumentType, "players()"),
        ("actor", ActorArgumentType, "actor()"),
        ("actors", ActorArgumentType, "actors()"),
    ],
)
def test_selector_argument_types(factory: str, cls: type, text: str) -> None:
    """Verify each selector argument type constructs as its class."""
    argument = getattr(ArgumentTypes, factory)()
    assert isinstance(argument, cls)
    assert isinstance(argument, ArgumentType)
    assert str(argument) == text


@pytest.mark.parametrize(
    ("factory", "cls", "text"),
    [
        ("block_position", BlockPositionArgumentType, "blockPosition()"),
        ("fine_position", FinePositionArgumentType, "finePosition()"),
        ("dimension", DimensionArgumentType, "dimension()"),
    ],
)
def test_position_and_dimension_argument_types(factory: str, cls: type, text: str) -> None:
    """Verify the position and dimension argument types construct as their classes."""
    argument = getattr(ArgumentTypes, factory)()
    assert isinstance(argument, cls)
    assert str(argument) == text


@pytest.mark.parametrize("registry_type", [BlockType, ItemType, ActorType])
def test_resource_argument_type(registry_type: type) -> None:
    """Verify resource takes each registry type it supports."""
    argument = ArgumentTypes.resource(registry_type)
    assert isinstance(argument, ResourceArgumentType)
    assert str(argument) == "resource()"


@pytest.mark.parametrize("registry_type", [int, str, CommandSourceStack])
def test_resource_argument_type_refuses_other_types(registry_type: type) -> None:
    """Verify resource raises ValueError for a type that is not a supported registry type."""
    with pytest.raises(ValueError, match="BlockType, ItemType or ActorType"):
        ArgumentTypes.resource(registry_type)


@pytest.mark.parametrize(
    "resolver",
    [PlayerSelectorArgumentResolver, ActorSelectorArgumentResolver, BlockPositionResolver, FinePositionResolver],
)
def test_resolvers_are_bound_without_a_constructor(resolver: type) -> None:
    """Verify each resolver has resolve(source) and only the server creates one."""
    assert callable(resolver.resolve)
    assert "source" in resolver.resolve.__doc__
    with pytest.raises(TypeError):
        resolver()


def test_builders_take_the_new_argument_types() -> None:
    """Verify argument nodes keep the selector, position, dimension and resource types they were built with."""

    def run(context: CommandContext) -> int:
        return 1

    root = (
        Commands.literal("t")
        .then(Commands.literal("p").then(Commands.argument("v", ArgumentTypes.player()).executes(run)))
        .then(Commands.literal("b").then(Commands.argument("v", ArgumentTypes.block_position()).executes(run)))
        .then(Commands.literal("d").then(Commands.argument("v", ArgumentTypes.dimension()).executes(run)))
        .then(Commands.literal("r").then(Commands.argument("v", ArgumentTypes.resource(BlockType)).executes(run)))
        .build()
    )
    types = [keyword.children[0].type for keyword in root.children]
    assert isinstance(types[0], PlayerArgumentType)
    assert isinstance(types[1], BlockPositionArgumentType)
    assert isinstance(types[2], DimensionArgumentType)
    assert str(types[3]) == "resource()"
