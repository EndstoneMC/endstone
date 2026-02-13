"""
Classes relating to the specialized enhancements to ItemStacks.
"""

import typing

from endstone.inventory import ItemStack

__all__ = ["Enchantment"]

class Enchantment:
    PROTECTION = "minecraft:protection"
    FIRE_PROTECTION = "minecraft:fire_protection"
    FEATHER_FALLING = "minecraft:feather_falling"
    BLAST_PROTECTION = "minecraft:blast_protection"
    PROJECTILE_PROTECTION = "minecraft:projectile_protection"
    THORNS = "minecraft:thorns"
    RESPIRATION = "minecraft:respiration"
    DEPTH_STRIDER = "minecraft:depth_strider"
    AQUA_AFFINITY = "minecraft:aqua_affinity"
    SHARPNESS = "minecraft:sharpness"
    SMITE = "minecraft:smite"
    BANE_OF_ARTHROPODS = "minecraft:bane_of_arthropods"
    KNOCKBACK = "minecraft:knockback"
    FIRE_ASPECT = "minecraft:fire_aspect"
    LOOTING = "minecraft:looting"
    EFFICIENCY = "minecraft:efficiency"
    SILK_TOUCH = "minecraft:silk_touch"
    UNBREAKING = "minecraft:unbreaking"
    POWER = "minecraft:power"
    PUNCH = "minecraft:punch"
    FLAME = "minecraft:flame"
    INFINITY = "minecraft:infinity"
    LUCK_OF_THE_SEA = "minecraft:luck_of_the_sea"
    LURE = "minecraft:lure"
    FROST_WALKER = "minecraft:frost_walker"
    MENDING = "minecraft:mending"
    CURSE_OF_BINDING = "minecraft:binding"
    CURSE_OF_VANISHING = "minecraft:vanishing"
    IMPALING = "minecraft:impaling"
    RIPTIDE = "minecraft:riptide"
    LOYALTY = "minecraft:loyalty"
    CHANNELING = "minecraft:channeling"
    MULTISHOT = "minecraft:multishot"
    PIERCING = "minecraft:piercing"
    QUICK_CHARGE = "minecraft:quick_charge"
    SOUL_SPEED = "minecraft:soul_speed"
    SWIFT_SNEAK = "minecraft:swift_sneak"
    WIND_BURST = "minecraft:wind_burst"
    DENSITY = "minecraft:density"
    BREACH = "minecraft:breach"
    @property
    def id(self) -> str:
        """
        Return the identifier for this enchantment.
        """
        ...
    @property
    def translation_key(self) -> str:
        """
        Get the translation key, suitable for use in a translation component.
        """
        ...
    @property
    def max_level(self) -> int:
        """
        Gets the maximum level that this Enchantment may become.
        """
        ...
    @property
    def start_level(self) -> int:
        """
        Gets the level that this Enchantment should start at (also known as minimum level).
        """
        ...
    def conflicts_with(self, other: Enchantment) -> bool:
        """
        Check if this enchantment conflicts with another enchantment.
        """
        ...
    def can_enchant_item(self, item: ItemStack) -> bool:
        """
        Checks if this Enchantment may be applied to the given ItemStack.

        This does not check if it conflicts with any enchantments already applied to the item.
        """
        ...
    @staticmethod
    def get(name: str) -> Enchantment:
        """
        Attempts to get the Enchantment with the given name.
        """
        ...
    def __str__(self) -> str: ...
    def __hash__(self) -> int: ...
    @typing.overload
    def __eq__(self, arg0: Enchantment) -> bool: ...
    @typing.overload
    def __eq__(self, arg0: str) -> bool: ...
    @typing.overload
    def __ne__(self, arg0: Enchantment) -> bool: ...
    @typing.overload
    def __ne__(self, arg0: str) -> bool: ...
