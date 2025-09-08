"""
Classes relating to the specialized enhancements to ItemStacks.
"""

from __future__ import annotations

import endstone._python.inventory

__all__: list[str] = ["Enchantment"]

class Enchantment:
    def can_enchant_item(self, item: endstone._python.inventory.ItemStack) -> bool:
        """
        Checks if this Enchantment may be applied to the given ItemStack.

        This does not check if it conflicts with any enchantments already applied to the item.
        """

    def conflicts_with(self, other: Enchantment) -> bool:
        """
        Check if this enchantment conflicts with another enchantment.
        """

    @property
    def id(self) -> str:
        """
        Return the identifier for this enchantment.
        """

    @property
    def max_level(self) -> int:
        """
        Gets the maximum level that this Enchantment may become.
        """

    @property
    def start_level(self) -> int:
        """
        Gets the level that this Enchantment should start at (also known as minimum level).
        """

    @property
    def translation_key(self) -> str:
        """
        Get the translation key, suitable for use in a translation component.
        """
