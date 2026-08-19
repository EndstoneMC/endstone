import math

import pytest
from endstone import Identifier, Player
from endstone.attribute import Attribute, AttributeInstance, AttributeModifier


def test_health_attribute_matches_properties(player: Player) -> None:
    # `health` / `max_health` are backed by the same `minecraft:health` attribute through a different code path
    # (Mob.getHealth / getMaxHealth, which ceil the float value), so the two must agree.
    health = player.get_attribute(Attribute.HEALTH)
    assert health is not None
    assert math.ceil(health.value) == player.health
    assert math.ceil(health.max_value) == player.max_health


def test_set_max_value(player: Player) -> None:
    # Setting the max of `minecraft:health` raises the player's maximum health, visible through the separate
    # max_health property.
    health = player.get_attribute(Attribute.HEALTH)
    assert health is not None
    original = health.max_value
    try:
        health.max_value = 40.0
        assert health.max_value == 40.0
        assert player.max_health == 40
    finally:
        health.max_value = original
    assert player.max_health == math.ceil(original)


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

    modifier = AttributeModifier(
        "test:by_id", 1.0, AttributeModifier.Operation.MULTIPLY_BASE
    )
    attr.add_transient_modifier(modifier)
    assert len(attr.modifiers) == 1

    attr.remove_modifier(modifier.id)
    assert len(attr.modifiers) == 0


def test_attributes_present(player: Player) -> None:
    # A player is a mob, so it exposes its full set of attribute instances.
    assert len(player.attributes) > 0


# =============================================================================
# Section: The v0.12 AttributeInstance surface
# =============================================================================


def test_type_is_an_identifier(player: Player) -> None:
    """Verify AttributeInstance.type is an Identifier, not an Attribute object."""
    health = player.get_attribute(Attribute.HEALTH)
    assert isinstance(health.type, Identifier)
    assert health.type == "minecraft:health"


def test_base_min_and_max_are_distinct(player: Player) -> None:
    """Verify the base, minimum, maximum and effective values are separate members."""
    health = player.get_attribute(Attribute.HEALTH)
    assert isinstance(health.base_value, float)
    assert isinstance(health.min_value, float)
    assert isinstance(health.max_value, float)
    assert isinstance(health.value, float)
    assert health.min_value <= health.value <= health.max_value


def test_base_value_is_writable(player: Player) -> None:
    """Verify the base value round-trips."""
    health = player.get_attribute(Attribute.HEALTH)
    original = health.base_value
    try:
        health.base_value = 15.0
        assert abs(health.base_value - 15.0) <= 0.00001
    finally:
        health.base_value = original


def test_min_value_is_writable(player: Player) -> None:
    """Verify the Bedrock-specific minimum round-trips."""
    health = player.get_attribute(Attribute.HEALTH)
    original = health.min_value
    try:
        health.min_value = 1.0
        assert abs(health.min_value - 1.0) <= 0.00001
    finally:
        health.min_value = original


def test_value_is_read_only(player: Player) -> None:
    """Verify the effective value cannot be assigned directly."""
    with pytest.raises(AttributeError):
        player.get_attribute(Attribute.HEALTH).value = 1.0


def test_an_unknown_attribute(player: Player) -> None:
    """Verify an unknown name answers has_attribute False and makes get_attribute raise."""
    assert player.has_attribute("minecraft:not_an_attribute") is False
    with pytest.raises(RuntimeError):
        player.get_attribute("minecraft:not_an_attribute")


def test_attributes_are_attribute_instances(player: Player) -> None:
    """Verify the attribute list is made of AttributeInstance objects."""
    assert len(player.attributes) > 0
    for attribute in player.attributes:
        assert isinstance(attribute, AttributeInstance)
        assert isinstance(attribute.type, Identifier)


# =============================================================================
# Section: Modifiers
# =============================================================================


@pytest.fixture
def attack_damage(player: Player) -> AttributeInstance:
    """Strip any modifier this test added, before and after."""
    instance = player.get_attribute(Attribute.ATTACK_DAMAGE)
    for modifier in list(instance.modifiers):
        if modifier.id == "endstone:test_modifier":
            instance.remove_modifier(modifier)

    yield instance

    for modifier in list(instance.modifiers):
        if modifier.id == "endstone:test_modifier":
            instance.remove_modifier(modifier)
    assert instance.get_modifier("endstone:test_modifier") is None


@pytest.mark.parametrize(
    "operation",
    [
        AttributeModifier.Operation.ADD,
        AttributeModifier.Operation.MULTIPLY_BASE,
        AttributeModifier.Operation.MULTIPLY,
        AttributeModifier.Operation.CAP,
    ],
    ids=lambda o: o.name,
)
def test_every_operation_applies(
    attack_damage: AttributeInstance, operation: AttributeModifier.Operation
) -> None:
    """Verify a modifier of each operation, including Bedrock's CAP, can be added."""
    modifier = AttributeModifier("endstone:test_modifier", 2.0, operation)
    attack_damage.add_modifier(modifier)

    applied = attack_damage.get_modifier("endstone:test_modifier")
    assert applied is not None
    assert applied.operation == operation


def test_remove_modifier_by_object(attack_damage: AttributeInstance) -> None:
    """Verify a modifier can be removed by handing back the object."""
    modifier = AttributeModifier("endstone:test_modifier", 2.0, AttributeModifier.ADD)
    attack_damage.add_modifier(modifier)
    assert attack_damage.get_modifier("endstone:test_modifier") is not None

    attack_damage.remove_modifier(modifier)
    assert attack_damage.get_modifier("endstone:test_modifier") is None


def test_get_modifier_for_an_absent_id(attack_damage: AttributeInstance) -> None:
    """Verify an unknown modifier id reads back as None."""
    assert attack_damage.get_modifier("endstone:not_a_modifier") is None


def test_transient_modifier_applies(attack_damage: AttributeInstance) -> None:
    """Verify a transient modifier takes effect the same way a durable one does."""
    modifier = AttributeModifier("endstone:test_modifier", 3.0, AttributeModifier.ADD)
    attack_damage.add_transient_modifier(modifier)
    assert attack_damage.get_modifier("endstone:test_modifier") is not None
