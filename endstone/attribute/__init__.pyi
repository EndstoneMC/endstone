"""
Classes relevant to attributes.
"""

import enum
import typing

from endstone import Identifier

__all__ = [
    "Attribute",
    "AttributeInstance",
    "AttributeModifier",
]

class Attribute:
    """
    All attribute types.
    """

    HEALTH: Identifier[Attribute] = "minecraft:health"
    FOLLOW_RANGE: Identifier[Attribute] = "minecraft:follow_range"
    KNOCKBACK_RESISTANCE: Identifier[Attribute] = "minecraft:knockback_resistance"
    MOVEMENT_SPEED: Identifier[Attribute] = "minecraft:movement"
    UNDERWATER_MOVEMENT_SPEED: Identifier[Attribute] = "minecraft:underwater_movement"
    LAVA_MOVEMENT_SPEED: Identifier[Attribute] = "minecraft:lava_movement"
    ATTACK_DAMAGE: Identifier[Attribute] = "minecraft:attack_damage"
    ABSORPTION: Identifier[Attribute] = "minecraft:absorption"
    LUCK: Identifier[Attribute] = "minecraft:luck"
    JUMP_STRENGTH: Identifier[Attribute] = "minecraft:jump_strength"
    PLAYER_HUNGER: Identifier[Attribute] = "minecraft:player.hunger"
    PLAYER_SATURATION: Identifier[Attribute] = "minecraft:player.saturation"
    PLAYER_EXHAUSTION: Identifier[Attribute] = "minecraft:player.exhaustion"
    PLAYER_LEVEL: Identifier[Attribute] = "minecraft:player.level"
    PLAYER_EXPERIENCE: Identifier[Attribute] = "minecraft:player.experience"
    ZOMBIE_SPAWN_REINFORCEMENTS: Identifier[Attribute] = "minecraft:zombie.spawn_reinforcements"

class AttributeModifier:
    """
    Represents an attribute modifier.
    """
    def __init__(self, id: Identifier[AttributeModifier] | str, amount: float, operation: Operation) -> None: ...
    class Operation(enum.Enum):
        """
        Operation to be applied.
        """

        ADD = 0
        """
        Adds (or subtracts) the specified amount to the base value.
        """
        MULTIPLY_BASE = 1
        """
        Multiplies the current value of the attribute by (1 + x), where x is the sum of the modifiers' amounts.
        """
        MULTIPLY = 2
        """
        For every modifier, multiplies the current value of the attribute by (1 + x), where x is the amount of the particular modifier.
        """
        CAP = 3
        """
        Caps the value of the attribute at the modifier's amount. Bedrock-specific operation with no Bukkit equivalent.
        """

    ADD = Operation.ADD
    MULTIPLY_BASE = Operation.MULTIPLY_BASE
    MULTIPLY = Operation.MULTIPLY
    CAP = Operation.CAP
    @property
    def id(self) -> Identifier[AttributeModifier]:
        """
        The id of this modifier.
        """
        ...
    @property
    def amount(self) -> float:
        """
        The amount by which this modifier will apply its operation.
        """
        ...
    @property
    def operation(self) -> Operation:
        """
        The operation this modifier will apply.
        """
        ...

class AttributeInstance:
    """
    Represents a mutable instance of an attribute and its associated modifiers and values.
    """
    @property
    def type(self) -> Identifier[Attribute]:
        """
        The attribute type pertaining to this instance.
        """
        ...
    @property
    def base_value(self) -> float:
        """
        Base value of this instance before modifiers are applied.
        """
        ...
    @base_value.setter
    def base_value(self, arg1: float) -> None: ...
    @property
    def min_value(self) -> float:
        """
        The minimum value this instance is allowed to take. Bedrock-specific.
        """
        ...
    @min_value.setter
    def min_value(self, arg1: float) -> None: ...
    @property
    def max_value(self) -> float:
        """
        The maximum value this instance is allowed to take. Bedrock-specific.
        """
        ...
    @max_value.setter
    def max_value(self, arg1: float) -> None: ...
    @property
    def value(self) -> float:
        """
        The value of this instance after all associated modifiers have been applied.
        """
        ...
    @property
    def modifiers(self) -> list[AttributeModifier]:
        """
        All modifiers present on this instance.
        """
        ...
    def get_modifier(self, id: Identifier[AttributeModifier] | str) -> AttributeModifier | None:
        """
        Gets the modifier with the corresponding id.

        Args:
            id: The id of the modifier.

        Returns:
            The modifier, or ``None`` if no modifier with the given id is present.
        """
        ...
    def add_modifier(self, modifier: AttributeModifier) -> None:
        """
        Add a modifier to this instance.

        Args:
            modifier: Modifier to add.
        """
        ...
    def add_transient_modifier(self, modifier: AttributeModifier) -> None:
        """
        Add a transient modifier to this instance. Transient modifiers are not persisted (saved with the NBT data).

        Args:
            modifier: Modifier to add.
        """
        ...
    @typing.overload
    def remove_modifier(self, modifier: AttributeModifier) -> None:
        """
        Remove a modifier from this instance.

        Args:
            modifier: Modifier to remove.
        """
        ...
    @typing.overload
    def remove_modifier(self, id: Identifier[AttributeModifier] | str) -> None:
        """
        Remove a modifier with the corresponding id from this instance.

        Args:
            id: The id of the modifier to remove.
        """
        ...
