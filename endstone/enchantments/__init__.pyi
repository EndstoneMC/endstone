"""
Classes relating to the specialized enhancements to ItemStacks.
"""

from endstone import Identifier
from endstone.inventory import ItemStack

__all__ = [
    "Enchantment",
]

class Enchantment:
    """
    Represents an enchantment that may be added to armor, weapons, tools or other items.
    """

    PROTECTION: Identifier[Enchantment] = "minecraft:protection"
    FIRE_PROTECTION: Identifier[Enchantment] = "minecraft:fire_protection"
    FEATHER_FALLING: Identifier[Enchantment] = "minecraft:feather_falling"
    BLAST_PROTECTION: Identifier[Enchantment] = "minecraft:blast_protection"
    PROJECTILE_PROTECTION: Identifier[Enchantment] = "minecraft:projectile_protection"
    THORNS: Identifier[Enchantment] = "minecraft:thorns"
    RESPIRATION: Identifier[Enchantment] = "minecraft:respiration"
    DEPTH_STRIDER: Identifier[Enchantment] = "minecraft:depth_strider"
    AQUA_AFFINITY: Identifier[Enchantment] = "minecraft:aqua_affinity"
    SHARPNESS: Identifier[Enchantment] = "minecraft:sharpness"
    SMITE: Identifier[Enchantment] = "minecraft:smite"
    BANE_OF_ARTHROPODS: Identifier[Enchantment] = "minecraft:bane_of_arthropods"
    KNOCKBACK: Identifier[Enchantment] = "minecraft:knockback"
    FIRE_ASPECT: Identifier[Enchantment] = "minecraft:fire_aspect"
    LOOTING: Identifier[Enchantment] = "minecraft:looting"
    EFFICIENCY: Identifier[Enchantment] = "minecraft:efficiency"
    SILK_TOUCH: Identifier[Enchantment] = "minecraft:silk_touch"
    UNBREAKING: Identifier[Enchantment] = "minecraft:unbreaking"
    POWER: Identifier[Enchantment] = "minecraft:power"
    PUNCH: Identifier[Enchantment] = "minecraft:punch"
    FLAME: Identifier[Enchantment] = "minecraft:flame"
    INFINITY: Identifier[Enchantment] = "minecraft:infinity"
    LUCK_OF_THE_SEA: Identifier[Enchantment] = "minecraft:luck_of_the_sea"
    LURE: Identifier[Enchantment] = "minecraft:lure"
    FROST_WALKER: Identifier[Enchantment] = "minecraft:frost_walker"
    MENDING: Identifier[Enchantment] = "minecraft:mending"
    CURSE_OF_BINDING: Identifier[Enchantment] = "minecraft:binding"
    CURSE_OF_VANISHING: Identifier[Enchantment] = "minecraft:vanishing"
    IMPALING: Identifier[Enchantment] = "minecraft:impaling"
    RIPTIDE: Identifier[Enchantment] = "minecraft:riptide"
    LOYALTY: Identifier[Enchantment] = "minecraft:loyalty"
    CHANNELING: Identifier[Enchantment] = "minecraft:channeling"
    MULTISHOT: Identifier[Enchantment] = "minecraft:multishot"
    PIERCING: Identifier[Enchantment] = "minecraft:piercing"
    QUICK_CHARGE: Identifier[Enchantment] = "minecraft:quick_charge"
    SOUL_SPEED: Identifier[Enchantment] = "minecraft:soul_speed"
    SWIFT_SNEAK: Identifier[Enchantment] = "minecraft:swift_sneak"
    WIND_BURST: Identifier[Enchantment] = "minecraft:wind_burst"
    DENSITY: Identifier[Enchantment] = "minecraft:density"
    BREACH: Identifier[Enchantment] = "minecraft:breach"
    LUNGE: Identifier[Enchantment] = "minecraft:lunge"
    @property
    def id(self) -> Identifier[Enchantment]:
        """
        The identifier of this enchantment.
        """
        ...
    @property
    def translation_key(self) -> str:
        """
        The translation key, suitable for use in a translation component.
        """
        ...
    @property
    def max_level(self) -> int:
        """
        The maximum level that this `Enchantment` may become.
        """
        ...
    @property
    def start_level(self) -> int:
        """
        The level that this `Enchantment` should start at (also known as minimum level).
        """
        ...
    def conflicts_with(self, other: Enchantment) -> bool:
        """
        Check if this enchantment conflicts with another enchantment.

        Args:
            other: The enchantment to check against.

        Returns:
            `True` if there is a conflict.
        """
        ...
    def can_enchant_item(self, item: ItemStack) -> bool:
        """
        Checks if this `Enchantment` may be applied to the given `ItemStack`.

        This does not check if it conflicts with any enchantments already applied to the item.

        Args:
            item: Item to test.

        Returns:
            `True` if the enchantment may be applied, `False` otherwise.
        """
        ...
    @staticmethod
    def get(name: Identifier[Enchantment] | str) -> Enchantment:
        """
        Look up a registry entry by identifier.

        Args:
            name: Identifier to look up.

        Returns:
            The `Enchantment`, or `None` if not found.
        """
        ...
    def __str__(self) -> str: ...
    def __hash__(self) -> int: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
