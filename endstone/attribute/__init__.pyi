"""
Classes relevant to attributes.
"""


__all__ = ["Attribute", "AttributeInstance"]

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
    def base_max_value(self) -> float:
        """
        Base max value of this instance before modifiers are applied.
        """
        ...
    @base_max_value.setter
    def base_max_value(self, arg1: float) -> None: ...
    @property
    def base_min_value(self) -> float:
        """
        Base min value of this instance before modifiers are applied.
        """
        ...
    @base_min_value.setter
    def base_min_value(self, arg1: float) -> None: ...
    @property
    def value(self) -> float:
        """
        Get the value of this instance after all associated modifiers have been applied.
        """
        ...
    @property
    def max_value(self) -> float:
        """
        Get the max value of this instance after all associated modifiers have been applied.
        """
        ...
    @property
    def min_value(self) -> float:
        """
        Get the min value of this instance after all associated modifiers have been applied.
        """
        ...
