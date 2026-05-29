"""
Classes relating to player inventories and item interactions.
"""

import enum
import typing

from endstone import Identifier
from endstone.enchantments import Enchantment
from endstone.map import MapView
from endstone.nbt import CompoundTag

__all__ = [
    "BookMeta",
    "BookMetaGeneration",
    "CrossbowMeta",
    "EquipmentSlot",
    "Inventory",
    "ItemFactory",
    "ItemMeta",
    "ItemStack",
    "ItemType",
    "MapMeta",
    "PlayerInventory",
    "WritableBookMeta",
]

class ItemStack:
    """
    Represents a stack of items.
    """
    def __init__(self, type: Identifier[ItemType] | str, amount: int = 1, data: int = 0) -> None: ...
    @property
    def type(self) -> ItemType:
        """
        The type of this item.
        """
        ...
    @type.setter
    def type(self, arg1: str) -> None: ...
    @property
    def amount(self) -> int:
        """
        The amount of items in this stack.
        """
        ...
    @amount.setter
    def amount(self, arg1: int) -> None: ...
    @property
    def data(self) -> int:
        """
        The data for this stack of items.
        """
        ...
    @data.setter
    def data(self, arg1: int) -> None: ...
    @property
    def translation_key(self) -> str:
        """
        The translation key for this item.
        """
        ...
    @property
    def max_stack_size(self) -> int:
        """
        The maximum stack size for this item.
        """
        ...
    def is_similar(self, other: ItemStack) -> bool:
        """
        Checks if the two stacks are equal, but does not consider stack size (amount).

        Args:
            other: The item stack to compare to.

        Returns:
            True if the two stacks are equal, ignoring the amount.
        """
        ...
    @property
    def item_meta(self) -> ItemMeta:
        """
        A copy of the ItemMeta of this ItemStack.
        """
        ...
    def set_item_meta(self, meta: ItemMeta) -> bool:
        """
        Set the ItemMeta of this ItemStack.

        Args:
            meta: New ItemMeta, or None to clear the metadata.

        Returns:
            True if successfully applied ItemMeta.
        """
        ...
    @property
    def nbt(self) -> CompoundTag:
        """
        The NBT compound tag of this item stack.
        """
        ...
    @nbt.setter
    def nbt(self, arg1: CompoundTag) -> None: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __str__(self) -> str: ...

class EquipmentSlot(enum.Enum):
    HAND = 0
    OFF_HAND = 1
    FEET = 2
    LEGS = 3
    CHEST = 4
    HEAD = 5
    BODY = 6
    """
    Only for certain entities such as horses and wolves.
    """

class ItemType:
    """
    Represents an item type.
    """
    @property
    def id(self) -> Identifier[ItemType]:
        """
        The identifier of this item type.
        """
        ...
    @property
    def translation_key(self) -> str:
        """
        The translation key, suitable for use in a translation component.
        """
        ...
    def get_translation_key(self, data: int = 0) -> str:
        """
        Get the translation key, suitable for use in a translation component.

        Args:
            data: Data for this item type.

        Returns:
            The translation key.
        """
        ...
    @property
    def max_stack_size(self) -> int:
        """
        The maximum amount of this item type that can be held in a stack.
        """
        ...
    @property
    def max_durability(self) -> int:
        """
        The maximum durability of this item type.
        """
        ...
    def create_item_stack(self, amount: int = 1) -> ItemStack:
        """
        Constructs a new ItemStack with this item type.

        Args:
            amount: The amount in the stack.

        Returns:
            An ItemStack of this item type.
        """
        ...
    @staticmethod
    def get(name: Identifier[ItemType] | str) -> ItemType:
        """
        Attempts to get the ItemType with the given name.
        """
        ...
    def __str__(self) -> str: ...
    def __hash__(self) -> int: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...

class ItemMeta:
    """
    Represents the metadata of a generic item.
    """
    def clone(self) -> ItemMeta:
        """
        Creates a clone of the current metadata.

        Returns:
            A copy of the metadata containing the same state as the original.
        """
        ...
    @property
    def has_display_name(self) -> bool:
        """
        Whether this has a display name.
        """
        ...
    @property
    def display_name(self) -> str:
        """
        The display name that is set.
        """
        ...
    @display_name.setter
    def display_name(self, arg1: str | None) -> None: ...
    @property
    def has_lore(self) -> bool:
        """
        Whether this has lore.
        """
        ...
    @property
    def lore(self) -> list[str]:
        """
        The lore for this item.
        """
        ...
    @lore.setter
    def lore(self, arg1: list[str] | None) -> None: ...
    @property
    def has_damage(self) -> bool:
        """
        Whether this item has damage.
        """
        ...
    @property
    def damage(self) -> int:
        """
        The damage.
        """
        ...
    @damage.setter
    def damage(self, arg1: int) -> None: ...
    @property
    def has_enchants(self) -> bool:
        """
        Whether an enchantment exists on this meta.
        """
        ...
    def has_enchant(self, id: Identifier[Enchantment] | str) -> bool:
        """
        Checks for existence of the specified enchantment.

        Args:
            id: Enchantment id to check.

        Returns:
            True if this enchantment exists for this meta.
        """
        ...
    def get_enchant_level(self, id: Identifier[Enchantment] | str) -> int:
        """
        Checks for the level of the specified enchantment.

        Args:
            id: Enchantment id to check.

        Returns:
            The level that the specified enchantment has, or 0 if none.
        """
        ...
    @property
    def enchants(self) -> dict[Enchantment, int]:
        """
        Returns a copy of the enchantments in this ItemMeta.

        Returns an empty map if none.
        """
        ...
    def add_enchant(self, id: Identifier[Enchantment] | str, level: int, force: bool = False) -> bool:
        """
        Adds the specified enchantment to this item meta.

        Args:
            id: Enchantment id to add.
            level: Level for the enchantment.
            force: This indicates the enchantment should be applied, ignoring the level limit.

        Returns:
            True if the item meta changed as a result of this call, False otherwise.
        """
        ...
    def remove_enchant(self, id: Identifier[Enchantment] | str) -> bool:
        """
        Removes the specified enchantment from this item meta.

        Args:
            id: Enchantment id to remove.

        Returns:
            True if the item meta changed as a result of this call, False otherwise.
        """
        ...
    def remove_enchants(self) -> None:
        """
        Removes all enchantments from this item meta.
        """
        ...
    @property
    def has_repair_cost(self) -> bool:
        """
        Whether this item has a repair penalty.
        """
        ...
    @property
    def repair_cost(self) -> int:
        """
        The repair penalty.
        """
        ...
    @repair_cost.setter
    def repair_cost(self, arg1: int) -> None: ...
    @property
    def is_unbreakable(self) -> bool:
        """
        The unbreakable tag. An unbreakable item will not lose durability.
        """
        ...
    @is_unbreakable.setter
    def is_unbreakable(self, arg1: bool) -> None: ...

class MapMeta(ItemMeta):
    """
    Represents the metadata for a map item.
    """
    @property
    def has_map_id(self) -> bool:
        """
        Whether this has a map ID number.
        """
        ...
    @property
    def map_id(self) -> int:
        """
        The map ID. This is used to determine what map is displayed.
        """
        ...
    @map_id.setter
    def map_id(self, arg1: int) -> None: ...
    @property
    def has_map_view(self) -> bool:
        """
        Whether this item has an associated map.
        """
        ...
    @property
    def map_view(self) -> MapView:
        """
        The map view associated with this map item.
        """
        ...
    @map_view.setter
    def map_view(self, arg1: MapView) -> None: ...

class WritableBookMeta(ItemMeta):
    """
    Represents the meta for a writable book that can have pages.
    """
    @property
    def has_pages(self) -> bool:
        """
        Whether the book has pages.
        """
        ...
    def get_page(self, page: int) -> str:
        """
        Gets the specified page in the book. The given page must exist.

        Note:
            Pages are 1-indexed.

        Args:
            page: The page number to get, in range [1, page_count].

        Returns:
            The page from the book.
        """
        ...
    def set_page(self, page: int, data: str) -> None:
        """
        Sets the specified page in the book. Pages of the book must be contiguous.

        Note:
            The data can be up to 1024 characters in length, additional characters are truncated.

        Pages are 1-indexed.

        Args:
            page: The page number to set, in range [1, page_count].
            data: The data to set for that page.
        """
        ...
    @property
    def pages(self) -> list[str]:
        """
        All the pages in the book.
        """
        ...
    @pages.setter
    def pages(self, arg1: list[str]) -> None: ...
    def add_page(self, *args: str) -> None:
        """
        Adds new pages to the end of the book.

        Note:
            Up to a maximum of 50 pages with 256 characters per page.

        Args:
            pages: A list of strings, each being a page.
        """
        ...
    @property
    def page_count(self) -> int:
        """
        The number of pages in the book.
        """
        ...

class BookMetaGeneration(enum.Enum):
    ORIGINAL = 0
    COPY_OF_ORIGINAL = 1
    COPY_OF_COPY = 2

class BookMeta(WritableBookMeta):
    """
    Represents the meta for a written book that can have a title, an author, and pages.
    """
    @property
    def has_title(self) -> bool:
        """
        Whether the book has a title.
        """
        ...
    @property
    def title(self) -> str:
        """
        The title of the book.
        """
        ...
    @title.setter
    def title(self, arg1: str | None) -> None: ...
    @property
    def has_author(self) -> bool:
        """
        Whether the book has an author.
        """
        ...
    @property
    def author(self) -> str:
        """
        The author of the book.
        """
        ...
    @author.setter
    def author(self, arg1: str | None) -> None: ...
    @property
    def has_generation(self) -> bool:
        """
        Whether the book has a generation level.
        """
        ...
    @property
    def generation(self) -> BookMetaGeneration | None:
        """
        The generation of the book.
        """
        ...
    @generation.setter
    def generation(self, arg1: BookMetaGeneration | None) -> None: ...

class CrossbowMeta(ItemMeta):
    """
    Represents the meta for a crossbow that can have a charged projectile.
    """
    @property
    def has_charged_projectile(self) -> bool:
        """
        Whether the crossbow has a charged projectile.
        """
        ...
    @property
    def charged_projectile(self) -> ItemStack | None:
        """
        The charged projectile, or None if none.
        """
        ...
    @charged_projectile.setter
    def charged_projectile(self, arg1: ItemStack | None) -> None: ...

class ItemFactory:
    def get_item_meta(self, type: Identifier[ItemType] | str) -> ItemMeta:
        """
        This creates a new item meta for the item type.

        Args:
            type: The item type to consider as base for the meta.

        Returns:
            A new ItemMeta that could be applied to an item stack of the specified item type.
        """
        ...
    def is_applicable(self, meta: ItemMeta, type: Identifier[ItemType] | str) -> bool:
        """
        This method checks the item meta to confirm that it is applicable (no data lost if applied) to the specified ItemStack.

        Args:
            meta: Meta to check.
            type: The item type that meta will be applied to.

        Returns:
            True if the meta can be applied without losing data, False otherwise.
        """
        ...
    def equals(self, meta1: ItemMeta, meta2: ItemMeta) -> bool:
        """
        This method is used to compare two ItemMeta objects.

        Args:
            meta1: First meta to compare; may be None to indicate no data.
            meta2: Second meta to compare; may be None to indicate no data.

        Returns:
            False if one of the meta has data the other does not, otherwise True.
        """
        ...
    def as_meta_for(self, meta: ItemMeta, type: Identifier[ItemType] | str) -> ItemMeta:
        """
        Returns an appropriate item meta for the specified item type.

        The item meta returned will always be a valid meta for a given ItemStack of the specified item type. It may be a
        more or less specific meta, and could also be the same meta or meta type as the parameter. The item meta returned
        will also always be the most appropriate meta.

        Args:
            meta: The meta to convert.
            type: The item type to convert the meta for.

        Returns:
            An appropriate item meta for the specified item type.
        """
        ...

class Inventory:
    """
    Interface to the various inventories.
    """
    @property
    def size(self) -> int:
        """
        The size of the inventory.
        """
        ...
    @property
    def max_stack_size(self) -> int:
        """
        The maximum stack size for an ItemStack in this inventory.
        """
        ...
    def get_item(self, index: int) -> ItemStack | None:
        """
        Returns the ItemStack found in the slot at the given index.

        Args:
            index: The index of the Slot's ItemStack to return.

        Returns:
            The ItemStack in the slot.
        """
        ...
    def set_item(self, index: int, item: ItemStack | None) -> None:
        """
        Stores the ItemStack at the given index of the inventory.

        Args:
            index: The index where to put the ItemStack.
            item: The ItemStack to set.
        """
        ...
    def add_item(self, *args) -> dict[int, ItemStack]:
        """
        Stores the given ItemStacks in the inventory.

        Note:
            This will try to fill existing stacks and empty slots as well as it can.

        Note:
            The returned map contains what it couldn't store, where the key is the index, and the value is the
            ItemStack. If all items are stored, it will return an empty dict.

        Returns:
            A map containing items that couldn't be added.
        """
        ...
    def remove_item(self, *args) -> dict[int, ItemStack]:
        """
        Removes the given ItemStacks from the inventory.

        Note:
            It will try to remove 'as much as possible' from the types and amounts you give as arguments.

        Note:
            The returned map contains what it couldn't remove, where the key is the index, and the value is the
            ItemStack. If all the given ItemStacks are removed, it will return an empty dict.

        Returns:
            A map containing items that couldn't be removed.
        """
        ...
    @property
    def contents(self) -> list[ItemStack | None]:
        """
        All ItemStacks from the inventory. Empty slots are represented as None.
        """
        ...
    @contents.setter
    def contents(self, arg1: list[ItemStack | None]) -> None: ...
    @typing.overload
    def contains(self, item: ItemStack, amount: int) -> bool:
        """
        Checks if the inventory contains at least the minimum amount specified of exactly matching ItemStacks.

        Note:
            An ItemStack only counts if both the type and the amount of the stack match.

        Args:
            item: The ItemStack to match against.
            amount: How many identical stacks to check for.

        Returns:
            True if amount less than 1 or if amount of exactly matching ItemStacks were found, False otherwise.
        """
        ...
    @typing.overload
    def contains(self, item: ItemStack) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemStack.

        Note:
            This will only return True if both the type and the amount of the stack match.

        Args:
            item: The ItemStack to match against.

        Returns:
            True if any exactly matching ItemStacks were found, False otherwise.
        """
        ...
    @typing.overload
    def contains(self, type: str) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemType.

        Args:
            type: The item type to check for.

        Returns:
            True if an ItemStack is found with the given ItemType.
        """
        ...
    @typing.overload
    def contains_at_least(self, item: ItemStack, amount: int) -> bool:
        """
        Checks if the inventory contains ItemStacks matching the given ItemStack whose amounts sum to at least the
        minimum amount specified.

        Args:
            item: The ItemStack to match against.
            amount: The minimum amount.

        Returns:
            True if amount less than 1 or enough ItemStacks were found to add to the given amount, False otherwise.
        """
        ...
    @typing.overload
    def contains_at_least(self, type: str, amount: int) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemType, adding to at least the minimum
        amount specified.

        Args:
            type: The ItemType to check for.
            amount: The minimum amount.

        Returns:
            True if amount is less than 1, True if enough ItemStacks were found to add to the given amount.
        """
        ...
    @typing.overload
    def all(self, item: ItemStack) -> dict[int, ItemStack]:
        """
        Finds all slots in the inventory containing any ItemStacks with the given ItemStack.

        Note:
            This will only match slots if both the type and the amount of the stack match.

        The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot.
        If no matching ItemStack is found, an empty map is returned.

        Args:
            item: The ItemStack to match against.

        Returns:
            A map from slot indexes to item at index.
        """
        ...
    @typing.overload
    def all(self, type: str) -> dict[int, ItemStack]:
        """
        Finds all slots in the inventory containing any ItemStacks with the given ItemType.

        The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot.
        If no matching ItemStack is found, an empty map is returned.

        Args:
            type: The ItemType to match against.

        Returns:
            A map from slot indexes to item at index.
        """
        ...
    @typing.overload
    def first(self, item: ItemStack) -> int:
        """
        Returns the first slot in the inventory containing an ItemStack with the given stack.

        Args:
            item: The ItemStack to match against.

        Returns:
            The slot index of the given ItemStack or -1 if not found.
        """
        ...
    @typing.overload
    def first(self, type: str) -> int:
        """
        Finds the first slot in the inventory containing an ItemStack with the given ItemType.

        Args:
            type: The ItemType to look for.

        Returns:
            The slot index of the given ItemType or -1 if not found.
        """
        ...
    @property
    def first_empty(self) -> int:
        """
        The first empty Slot found, or -1 if no empty slots.
        """
        ...
    @property
    def is_empty(self) -> bool:
        """
        Whether this inventory is empty.

        An inventory is considered to be empty if there are no ItemStacks in any slot of this inventory.
        """
        ...
    @typing.overload
    def remove(self, item: ItemStack) -> None:
        """
        Removes all stacks in the inventory matching the given stack.

        Note:
            This will only match a slot if both the type and the amount of the stack match.

        Args:
            item: The ItemStack to match against.
        """
        ...
    @typing.overload
    def remove(self, type: str) -> None:
        """
        Removes all stacks in the inventory matching the given ItemType.

        Args:
            type: The ItemType to remove.
        """
        ...
    @typing.overload
    def clear(self, index: int) -> None:
        """
        Clears out a particular slot in the index.

        Args:
            index: The index to empty.
        """
        ...
    @typing.overload
    def clear(self) -> None:
        """
        Clears out the whole Inventory.
        """
        ...
    def __len__(self) -> int:
        """
        Returns the size of the inventory.
        """
        ...
    def __getitem__(self, index: int) -> ItemStack | None:
        """
        Returns the ItemStack found in the slot at the given index.
        """
        ...
    def __setitem__(self, index: int, item: ItemStack | None) -> None:
        """
        Stores the ItemStack at the given index of the inventory.
        """
        ...
    @typing.overload
    def __contains__(self, item: ItemStack) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemStack.
        """
        ...
    @typing.overload
    def __contains__(self, type: str) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemType.
        """
        ...

class PlayerInventory(Inventory):
    """
    Interface to the inventory of a Player, including the four armor slots and any extra slots.
    """
    @property
    def helmet(self) -> ItemStack | None:
        """
        The ItemStack in the helmet slot.
        """
        ...
    @helmet.setter
    def helmet(self, arg1: ItemStack | None) -> None: ...
    @property
    def chestplate(self) -> ItemStack | None:
        """
        The ItemStack in the chestplate slot.
        """
        ...
    @chestplate.setter
    def chestplate(self, arg1: ItemStack | None) -> None: ...
    @property
    def leggings(self) -> ItemStack | None:
        """
        The ItemStack in the leg slot.
        """
        ...
    @leggings.setter
    def leggings(self, arg1: ItemStack | None) -> None: ...
    @property
    def boots(self) -> ItemStack | None:
        """
        The ItemStack in the boots slot.
        """
        ...
    @boots.setter
    def boots(self, arg1: ItemStack | None) -> None: ...
    @property
    def item_in_main_hand(self) -> ItemStack | None:
        """
        The item the player is currently holding in their main hand.
        """
        ...
    @item_in_main_hand.setter
    def item_in_main_hand(self, arg1: ItemStack | None) -> None: ...
    @property
    def item_in_off_hand(self) -> ItemStack | None:
        """
        The item the player is currently holding in their off hand.
        """
        ...
    @item_in_off_hand.setter
    def item_in_off_hand(self, arg1: ItemStack | None) -> None: ...
    @property
    def held_item_slot(self) -> int:
        """
        The slot number of the currently held item.
        """
        ...
    @held_item_slot.setter
    def held_item_slot(self, arg1: int) -> None: ...
