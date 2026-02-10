from typing import Any, Generator

import pytest
from endstone import Player
from endstone.plugin import Plugin


@pytest.fixture()
def permissible(player: Player) -> Generator[Player, Any, None]:
    is_op = player.is_op
    player.is_op = False
    yield player
    player.is_op = is_op


def test_non_op_permissions(permissible: Player):
    assert not permissible.is_op, "Player should not be op by default"
    assert permissible.has_permission("minecraft.command.me") is True
    assert not permissible.has_permission("minecraft.command.kick")
    assert not permissible.has_permission("endstone.command.ban")


def test_op_permissions(permissible: Player):
    permissible.is_op = True
    assert permissible.is_op, "Player should be op"
    assert permissible.has_permission("minecraft.command.me")
    assert permissible.has_permission("minecraft.command.kick")
    assert permissible.has_permission("endstone.command.ban")


def test_explicit_grant_non_op(permissible: Player, plugin: Plugin):
    attachment = permissible.add_attachment(plugin, "minecraft.command.kick", True)
    assert not permissible.is_op, "Player remains non-op"
    assert permissible.has_permission("minecraft.command.kick"), (
        "Explicit grant should allow the permission"
    )
    permissible.remove_attachment(attachment)
    assert not permissible.has_permission("minecraft.command.kick")


def test_explicit_deny_op(permissible: Player, plugin: Plugin):
    permissible.is_op = True
    attachment = permissible.add_attachment(plugin, "minecraft.command.kick", False)
    assert permissible.is_op, "Player is op"
    assert not permissible.has_permission("minecraft.command.kick"), (
        "Explicit deny should override op status"
    )
    permissible.remove_attachment(attachment)
    assert permissible.has_permission("minecraft.command.kick")
