"""
Classes relevant to attributes.
"""

import enum
import uuid

__all__ = ["Attribute", "AttributeInstance", "AttributeModifier"]

class Attribute:
    """
    All attribute types.
    """

    HEALTH = "minecraft:health"
    FOLLOW_RANGE = "minecraft:follow_range"
    KNOCKBACK_RESISTANCE = "minecraft:knockback_resistance"
    MOVEMENT_SPEED = "minecraft:movement"
    UNDERWATER_MOVEMENT_SPEED = "minecraft:underwater_movement"
    LAVA_MOVEMENT_SPEED = "minecraft:lava_movement"
    ATTACK_DAMAGE = "minecraft:attack_damage"
    ABSORPTION = "minecraft:absorption"
    LUCK = "minecraft:luck"
    JUMP_STRENGTH = "minecraft:jump_strength"
    PLAYER_HUNGER = "minecraft:player.hunger"
    PLAYER_SATURATION = "minecraft:player.saturation"
    PLAYER_EXHAUSTION = "minecraft:player.exhaustion"
    PLAYER_LEVEL = "minecraft:player.level"
    PLAYER_EXPERIENCE = "minecraft:player.experience"
    ZOMBIE_SPAWN_REINFORCEMENTS = "minecraft:zombie.spawn_reinforcements"

class AttributeModifier:
    """
    Represents an attribute modifier.
    """
    def __init__(self, name: str, amount: float, operation: Operation, operand: Operand = Operand.VALUE) -> None: ...
    class Operand(enum.Enum):
        """
        Value on which operation to be applied.
        """

        VALUE = 0
        MAX_VALUE = 1
        MIN_VALUE = 2

    VALUE = Operand.VALUE
    MAX_VALUE = Operand.MAX_VALUE
    MIN_VALUE = Operand.MIN_VALUE
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

    ADD = Operation.ADD
    MULTIPLY_BASE = Operation.MULTIPLY_BASE
    MULTIPLY = Operation.MULTIPLY
    @property
    def unique_id(self) -> uuid.UUID:
        """
        Get the unique ID for this modifier.
        """
        ...
    @property
    def name(self) -> str:
        """
        Get the name of this modifier.
        """
        ...
    @property
    def amount(self) -> float:
        """
        Get the amount by which this modifier will apply the operation.
        """
        ...
    @property
    def operand(self) -> Operand:
        """
        Get the operand this modifier will apply.
        """
        ...
    @property
    def operation(self) -> Operation:
        """
        Get the operation this modifier will apply.
        """
        ...

class AttributeInstance:
    """
    Represents a mutable instance of an attribute and its associated modifiers and values.
    """
    @property
    def type(self) -> str:
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
    def value(self) -> float:
        """
        Get the value of this instance after all associated modifiers have been applied.
        """
        ...
    @property
    def modifiers(self) -> list[AttributeModifier]:
        """
        Get all modifiers present on this instance.
        """
        ...
    def add_modifier(self, modifier: AttributeModifier) -> None:
        """
        Add a modifier to this instance.
        """
        ...
    def remove_modifier(self, modifier: AttributeModifier) -> None:
        """
        Remove a modifier from this instance.
        """
        ...
