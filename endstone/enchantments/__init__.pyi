"""
Classes relating to the specialized enhancements to ItemStacks.
"""

from endstone.inventory import ItemStack

__all__ = ["Enchantment"]

class Enchantment:
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
