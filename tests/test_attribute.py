import pytest

from endstone import Identifier
from endstone.attribute import Attribute, AttributeModifier

ATTRIBUTES = {
    "HEALTH": "minecraft:health",
    "FOLLOW_RANGE": "minecraft:follow_range",
    "KNOCKBACK_RESISTANCE": "minecraft:knockback_resistance",
    "MOVEMENT_SPEED": "minecraft:movement",
    "UNDERWATER_MOVEMENT_SPEED": "minecraft:underwater_movement",
    "LAVA_MOVEMENT_SPEED": "minecraft:lava_movement",
    "ATTACK_DAMAGE": "minecraft:attack_damage",
    "ABSORPTION": "minecraft:absorption",
    "LUCK": "minecraft:luck",
    "JUMP_STRENGTH": "minecraft:horse.jump_strength",
    "PLAYER_HUNGER": "minecraft:player.hunger",
    "PLAYER_SATURATION": "minecraft:player.saturation",
    "PLAYER_EXHAUSTION": "minecraft:player.exhaustion",
    "PLAYER_LEVEL": "minecraft:player.level",
    "PLAYER_EXPERIENCE": "minecraft:player.experience",
    "ZOMBIE_SPAWN_REINFORCEMENTS": "minecraft:zombie.spawn_reinforcements",
}


@pytest.mark.parametrize("name,expected", sorted(ATTRIBUTES.items()))
def test_attribute_constants(name: str, expected: str) -> None:
    """Verify each attribute constant resolves to its documented identifier."""
    constant = getattr(Attribute, name)
    assert isinstance(constant, Identifier)
    assert constant == expected


def test_no_unexpected_attribute_constants() -> None:
    """Verify the attribute constants are exactly the ones we pin above."""
    declared = {name for name in dir(Attribute) if name.isupper()}
    assert declared == set(ATTRIBUTES)


def test_movement_attributes_drop_the_speed_suffix() -> None:
    """Verify the movement attributes use Bedrock's shorter ids, not Bukkit's."""
    assert Attribute.MOVEMENT_SPEED.key == "movement"
    assert Attribute.UNDERWATER_MOVEMENT_SPEED.key == "underwater_movement"
    assert Attribute.LAVA_MOVEMENT_SPEED.key == "lava_movement"


def test_player_attributes_use_a_dotted_key() -> None:
    """Verify the player attributes keep the dot in their key, not a second colon."""
    assert Attribute.PLAYER_HUNGER.namespace == "minecraft"
    assert Attribute.PLAYER_HUNGER.key == "player.hunger"


def test_operation_members() -> None:
    """Verify the operations, including the Bedrock-only CAP."""
    assert [member.name for member in AttributeModifier.Operation] == [
        "ADD",
        "MULTIPLY_BASE",
        "MULTIPLY",
        "CAP",
    ]


@pytest.mark.parametrize("name", ["ADD", "MULTIPLY_BASE", "MULTIPLY", "CAP"])
def test_operation_shorthand(name: str) -> None:
    """Verify each operation is also exported onto AttributeModifier itself."""
    assert getattr(AttributeModifier, name) is getattr(AttributeModifier.Operation, name)


def test_modifier_construction() -> None:
    """Verify a modifier keeps the key, amount and operation it was built with."""
    modifier = AttributeModifier("endstone:boost", 1.5, AttributeModifier.Operation.ADD)
    assert modifier.key == "endstone:boost"
    assert abs(modifier.amount - 1.5) <= 0.00001
    assert modifier.operation == AttributeModifier.Operation.ADD


def test_modifier_key_defaults_to_minecraft_namespace() -> None:
    """Verify a bare modifier key is placed in the minecraft namespace."""
    modifier = AttributeModifier("boost", 1.0, AttributeModifier.MULTIPLY_BASE)
    assert modifier.key.namespace == "minecraft"
    assert modifier.key.key == "boost"


def test_modifier_accepts_an_identifier() -> None:
    """Verify a modifier can be built from an Identifier as well as a string."""
    modifier = AttributeModifier(Identifier("endstone", "boost"), -2.0, AttributeModifier.CAP)
    assert modifier.key == "endstone:boost"
    assert abs(modifier.amount + 2.0) <= 0.00001


def test_modifier_key_survives_the_source_string() -> None:
    """Verify a modifier owns its key rather than viewing the caller's string."""
    name = "endstone:temporary"
    modifier = AttributeModifier(name, 1.0, AttributeModifier.ADD)
    del name
    assert modifier.key == "endstone:temporary"
