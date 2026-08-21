from typing import Annotated

import pytest

from endstone import Player
from endstone.command import CommandContext, command
from endstone.command import arguments as arg
from endstone.command._tree import build_command_trees


class Warps:
    """Stands in for a plugin: the decorators only need an object to hang methods off."""

    @command(description="Manage warps.", aliases=["w"], permission="my_plugin.command.warp")
    def warp(self, ctx: CommandContext) -> None: ...

    @warp.subcommand()
    def add(self, ctx: CommandContext, name: str) -> None: ...

    @warp.subcommand(permission="my_plugin.command.warp.others")
    def tp(
        self,
        ctx: CommandContext,
        name: Annotated[str, arg.soft_enum("my_plugin:warps")],
        amount: Annotated[int, arg.integer(1, 64)] = 1,
        who: list[Player] | None = None,
    ) -> None: ...


def shape(node) -> dict:
    return {
        "name": node.name,
        "executable": node.is_executable,
        "permissions": list(node.permissions),
        "children": [shape(child) for child in node.children],
    }


@pytest.fixture
def root():
    trees = build_command_trees(Warps())
    assert len(trees) == 1
    return trees[0].build()


class TestCommandDecorator:
    def test_root_carries_the_command_metadata(self, root) -> None:
        assert root.name == "warp"
        assert root.description == "Manage warps."
        assert list(root.aliases) == ["w"]
        assert list(root.permissions) == ["my_plugin.command.warp"]

    def test_a_handler_with_no_arguments_makes_the_literal_executable(self, root) -> None:
        assert root.is_executable

    def test_subcommands_become_children_named_after_the_method(self, root) -> None:
        assert [child.name for child in root.children] == ["add", "tp"]

    def test_a_mandatory_argument_moves_the_handler_off_the_literal(self, root) -> None:
        add = next(child for child in root.children if child.name == "add")
        assert not add.is_executable

        name = add.children[0]
        assert name.name == "name"
        assert name.is_executable

    def test_every_optional_argument_can_end_the_command(self, root) -> None:
        tp = next(child for child in root.children if child.name == "tp")
        assert not tp.is_executable

        # /warp tp <name> [amount] [who] - each optional step is its own executable node.
        name = tp.children[0]
        amount = name.children[0]
        who = amount.children[0]
        assert [name.name, amount.name, who.name] == ["name", "amount", "who"]
        assert all(node.is_executable for node in (name, amount, who))
        assert who.children == []

    def test_a_subcommand_keeps_its_own_permission(self, root) -> None:
        tp = next(child for child in root.children if child.name == "tp")
        assert list(tp.permissions) == ["my_plugin.command.warp.others"]


class TestArgumentInference:
    def test_an_unannotated_argument_is_rejected(self) -> None:
        class Bad:
            @command()
            def broken(self, ctx: CommandContext, name) -> None: ...

        with pytest.raises(TypeError, match="needs an annotation"):
            build_command_trees(Bad())

    def test_an_unmappable_annotation_is_rejected(self) -> None:
        class Bad:
            @command()
            def broken(self, ctx: CommandContext, when: complex) -> None: ...

        with pytest.raises(TypeError, match="has no argument type"):
            build_command_trees(Bad())

    def test_a_bare_annotation_picks_the_matching_argument_type(self) -> None:
        class Plain:
            @command()
            def echo(self, ctx: CommandContext, count: int, message: str) -> None: ...

        root = build_command_trees(Plain())[0].build()
        count = root.children[0]
        assert [count.name, count.children[0].name] == ["count", "message"]
