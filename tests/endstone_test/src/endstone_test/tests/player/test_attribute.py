import math

from endstone import Player
from endstone.attribute import Attribute, AttributeModifier


def test_health_attribute_matches_properties(player: Player) -> None:
    # `health` / `max_health` are backed by the same `minecraft:health` attribute through a different code path
    # (Mob.getHealth / getMaxHealth, which ceil the float value), so the two must agree.
    health = player.get_attribute(Attribute.HEALTH)
    assert health is not None
    assert math.ceil(health.value) == player.health
    assert math.ceil(health.max_value) == player.max_health


def test_attribute(player: Player) -> None:
    attr = player.get_attribute(Attribute.HEALTH)
    assert attr is not None
    assert attr.type == "minecraft:health"
    assert player.has_attribute(Attribute.HEALTH)

    modifier = AttributeModifier("test:test", 2.0, AttributeModifier.Operation.ADD)
    attr.add_modifier(modifier)
    assert len(attr.modifiers) == 1
    assert attr.get_modifier(modifier.id) is not None

    attr.remove_modifier(modifier)
    assert len(attr.modifiers) == 0
    assert attr.get_modifier(modifier.id) is None


def test_attribute_remove_by_id(player: Player) -> None:
    attr = player.get_attribute(Attribute.ATTACK_DAMAGE)
    assert attr is not None

    modifier = AttributeModifier("test:by_id", 1.0, AttributeModifier.Operation.MULTIPLY_BASE)
    attr.add_transient_modifier(modifier)
    assert len(attr.modifiers) == 1

    attr.remove_modifier(modifier.id)
    assert len(attr.modifiers) == 0


def test_attributes_present(player: Player) -> None:
    # A player is a mob, so it exposes its full set of attribute instances.
    assert len(player.attributes) > 0
