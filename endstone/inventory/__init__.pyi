"""
Classes relating to player inventories and item interactions.
"""

import enum
import typing

from endstone.enchantments import Enchantment
from endstone.map import MapView
from endstone.nbt import CompoundTag

__all__ = [
    "EquipmentSlot",
    "Inventory",
    "ItemFactory",
    "ItemMeta",
    "ItemStack",
    "ItemType",
    "MapMeta",
    "PlayerInventory",
]

class ItemStack:
    """
    Represents a stack of items.
    """
    def __init__(self, type: str, amount: int = 1, data: int = 0) -> None: ...
    @property
    def type(self) -> ItemType:
        """
        Gets or sets the type of this item.
        """
        ...
    @type.setter
    def type(self, arg1: str) -> None: ...
    @property
    def amount(self) -> int:
        """
        Gets or sets the amount of items in this stack.
        """
        ...
    @amount.setter
    def amount(self, arg1: int) -> None: ...
    @property
    def data(self) -> int:
        """
        Gets or sets the data for this stack of items.
        """
        ...
    @data.setter
    def data(self, arg1: int) -> None: ...
    @property
    def translation_key(self) -> str:
        """
        Get the translation key for this item.
        """
        ...
    @property
    def max_stack_size(self) -> int:
        """
        Get the maximum stack size for this item.
        """
        ...
    def is_similar(self, other: ItemStack) -> bool:
        """
        Checks if the two stacks are equal, but does not consider stack size (amount).
        """
        ...
    @property
    def item_meta(self) -> ItemMeta:
        """
        Gets a copy of the ItemMeta of this ItemStack.
        """
        ...
    def set_item_meta(self, meta: ItemMeta) -> bool:
        """
        Set the ItemMeta of this ItemStack.
        """
        ...
    @property
    def nbt(self) -> CompoundTag:
        """
        Gets or sets the NBT compound tag of this item stack.
        """
        ...
    @nbt.setter
    def nbt(self, arg1: CompoundTag) -> None: ...
    def __eq__(self, arg0: ItemStack) -> bool: ...
    def __ne__(self, arg0: ItemStack) -> bool: ...
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
    def id(self) -> str:
        """
        Return the identifier of this item type.
        """
        ...
    @property
    def translation_key(self) -> str:
        """
        Get the translation key, suitable for use in a translation component.
        """
        ...
    def get_translation_key(self, data: int = 0) -> str:
        """
        Get the translation key, suitable for use in a translation component.
        """
        ...
    @property
    def max_stack_size(self) -> int:
        """
        Gets the maximum amount of this item type that can be held in a stack.
        """
        ...
    @property
    def max_durability(self) -> int:
        """
        Gets the maximum durability of this item type
        """
        ...
    def create_item_stack(self, amount: int = 1) -> ItemStack:
        """
        Constructs a new ItemStack with this item type.
        """
        ...
    @staticmethod
    def get(name: str) -> ItemType:
        """
        Attempts to get the ItemType with the given name.
        """
        ...
    def __str__(self) -> str: ...
    @typing.overload
    def __eq__(self, arg0: ItemType) -> bool: ...
    @typing.overload
    def __eq__(self, arg0: str) -> bool: ...
    @typing.overload
    def __ne__(self, arg0: ItemType) -> bool: ...
    @typing.overload
    def __ne__(self, arg0: str) -> bool: ...

class ItemMeta:
    """
    Represents the metadata of a generic item.
    """
    def clone(self) -> ItemMeta:
        """
        Creates a clone of the current metadata.
        """
        ...
    @property
    def has_display_name(self) -> bool:
        """
        Checks for existence of a display name.
        """
        ...
    @property
    def display_name(self) -> str:
        """
        Gets or sets the display name.
        """
        ...
    @display_name.setter
    def display_name(self, arg1: str | None) -> None: ...
    @property
    def has_lore(self) -> bool:
        """
        Checks for existence of lore.
        """
        ...
    @property
    def lore(self) -> list[str]:
        """
        Gets or sets the lore for this item.
        """
        ...
    @lore.setter
    def lore(self, arg1: list[str] | None) -> None: ...
    @property
    def has_damage(self) -> bool:
        """
        Checks to see if this item has damage.
        """
        ...
    @property
    def damage(self) -> int:
        """
        Gets or sets the damage.
        """
        ...
    @damage.setter
    def damage(self, arg1: int) -> None: ...
    @property
    def has_enchants(self) -> bool:
        """
        Checks for the existence of any enchantments.
        """
        ...
    def has_enchant(self, id: str) -> bool:
        """
        Checks for existence of the specified enchantment.
        """
        ...
    def get_enchant_level(self, id: str) -> int:
        """
        Checks for the level of the specified enchantment.
        """
        ...
    @property
    def enchants(self) -> dict[Enchantment, int]:
        """
        Returns a copy the enchantments in this ItemMeta.
        """
        ...
    def add_enchant(self, id: str, level: int, force: bool = False) -> bool:
        """
        Adds the specified enchantment to this item meta.
        """
        ...
    def remove_enchant(self, id: str) -> bool:
        """
        Removes the specified enchantment from this item meta.
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
        Checks to see if this item has a repair penalty.
        """
        ...
    @property
    def repair_cost(self) -> int:
        """
        Gets or sets the repair penalty.
        """
        ...
    @repair_cost.setter
    def repair_cost(self, arg1: int) -> None: ...
    @property
    def is_unbreakable(self) -> bool:
        """
        Gets or sets the unbreakable tag. An unbreakable item will not lose durability.
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
        Checks for existence of a map ID number.
        """
        ...
    @property
    def map_id(self) -> int:
        """
        Gets or sets the map ID. This is used to determine what map is displayed.
        """
        ...
    @map_id.setter
    def map_id(self, arg1: int) -> None: ...
    @property
    def has_map_view(self) -> bool:
        """
        Checks for existence of an associated map.
        """
        ...
    @property
    def map_view(self) -> MapView:
        """
        Gets or sets the map view that is associated with this map item.
        """
        ...
    @map_view.setter
    def map_view(self, arg1: MapView) -> None: ...

class ItemFactory:
    def get_item_meta(self, type: str) -> ItemMeta:
        """
        This creates a new item meta for the item type.
        """
        ...
    def is_applicable(self, meta: ItemMeta, type: str) -> bool:
        """
        This method checks the item meta to confirm that it is applicable (no data lost if applied) to the specified ItemStack
        """
        ...
    def equals(self, meta1: ItemMeta, meta2: ItemMeta) -> bool:
        """
        This method is used to compare two ItemMeta objects.
        """
        ...
    def as_meta_for(self, meta: ItemMeta, type: str) -> ItemMeta:
        """
        Returns an appropriate item meta for the specified item type.
        """
        ...

class Inventory:
    """
    Interface to the various inventories.
    """
    @property
    def size(self) -> int:
        """
        Returns the size of the inventory
        """
        ...
    @property
    def max_stack_size(self) -> int:
        """
        Returns the maximum stack size for an ItemStack in this inventory.
        """
        ...
    def get_item(self, index: int) -> ItemStack | None:
        """
        Returns the ItemStack found in the slot at the given index
        """
        ...
    def set_item(self, index: int, item: ItemStack | None) -> None:
        """
        Stores the ItemStack at the given index of the inventory.
        """
        ...
    def add_item(self, *args) -> dict[int, ItemStack]:
        """
        Stores the given ItemStacks in the inventory.
        This will try to fill existing stacks and empty slots as well as it can.

        The returned map contains what it couldn't store, where the key is the index, and the value is the ItemStack.
        If all items are stored, it will return an empty dict.
        """
        ...
    def remove_item(self, *args) -> dict[int, ItemStack]:
        """
        Removes the given ItemStacks from the inventory.
        It will try to remove 'as much as possible' from the types and amounts you give as arguments.

        The returned HashMap contains what it couldn't remove, where the key is the index, and the value is the ItemStack.
        If all the given ItemStacks are removed, it will return an empty dict.
        """
        ...
    @property
    def contents(self) -> list[ItemStack | None]:
        """
        Returns all ItemStacks from the inventory
        """
        ...
    @contents.setter
    def contents(self, arg1: list[ItemStack | None]) -> None: ...
    @typing.overload
    def contains(self, item: ItemStack, amount: int) -> bool:
        """
        Checks if the inventory contains at least the minimum amount specified of exactly matching ItemStacks.
        An ItemStack only counts if both the type and the amount of the stack match.
        """
        ...
    @typing.overload
    def contains(self, item: ItemStack) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemStack.
        This will only return true if both the type and the amount of the stack match.
        """
        ...
    @typing.overload
    def contains(self, type: str) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemType.
        """
        ...
    @typing.overload
    def contains_at_least(self, item: ItemStack, amount: int) -> bool:
        """
        Checks if the inventory contains ItemStacks matching the given ItemStack whose amounts sum to at least the minimum amount specified.
        """
        ...
    @typing.overload
    def contains_at_least(self, type: str, amount: int) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemType, adding to at least the minimum amount specified.
        """
        ...
    @typing.overload
    def all(self, item: ItemStack) -> dict[int, ItemStack]:
        """
        Finds all slots in the inventory containing any ItemStacks with the given ItemStack.
        This will only match slots if both the type and the amount of the stack match
        The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.
        """
        ...
    @typing.overload
    def all(self, type: str) -> dict[int, ItemStack]:
        """
        Finds all slots in the inventory containing any ItemStacks with the given ItemType.
        The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.
        """
        ...
    @typing.overload
    def first(self, item: ItemStack) -> int:
        """
        Returns the first slot in the inventory containing an ItemStack with the given stack.
        This will only match slots if both the type and the amount of the stack match
        The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.
        """
        ...
    @typing.overload
    def first(self, type: str) -> int:
        """
        Finds the first slot in the inventory containing an ItemStack with the given ItemType.
        The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.
        """
        ...
    @property
    def first_empty(self) -> int:
        """
        Returns the first empty Slot.
        """
        ...
    @property
    def is_empty(self) -> bool:
        """
        Check whether this inventory is empty. An inventory is considered to be empty if there are no ItemStacks in any slot of this inventory.
        """
        ...
    @typing.overload
    def remove(self, item: ItemStack) -> None:
        """
        Removes all stacks in the inventory matching the given stack.
        This will only match a slot if both the type and the amount of the stack match
        """
        ...
    @typing.overload
    def remove(self, type: str) -> None:
        """
        Removes all stacks in the inventory matching the given ItemType.
        """
        ...
    @typing.overload
    def clear(self, index: int) -> None:
        """
        Clears out a particular slot in the index.
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
        Returns the size of the inventory
        """
        ...
    def __getitem__(self, index: int) -> ItemStack | None:
        """
        Returns the ItemStack found in the slot at the given index
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
        Gets or sets the ItemStack in the helmet slot
        """
        ...
    @helmet.setter
    def helmet(self, arg1: ItemStack | None) -> None: ...
    @property
    def chestplate(self) -> ItemStack | None:
        """
        Gets or sets the ItemStack in the chestplate slot
        """
        ...
    @chestplate.setter
    def chestplate(self, arg1: ItemStack | None) -> None: ...
    @property
    def leggings(self) -> ItemStack | None:
        """
        Gets or sets the ItemStack in the leg slot
        """
        ...
    @leggings.setter
    def leggings(self, arg1: ItemStack | None) -> None: ...
    @property
    def boots(self) -> ItemStack | None:
        """
        Gets or sets the ItemStack in the boots slot
        """
        ...
    @boots.setter
    def boots(self, arg1: ItemStack | None) -> None: ...
    @property
    def item_in_main_hand(self) -> ItemStack | None:
        """
        Gets or sets the item the player is currently holding in their main hand.
        """
        ...
    @item_in_main_hand.setter
    def item_in_main_hand(self, arg1: ItemStack | None) -> None: ...
    @property
    def item_in_off_hand(self) -> ItemStack | None:
        """
        Gets or sets the item the player is currently holding in their off hand.
        """
        ...
    @item_in_off_hand.setter
    def item_in_off_hand(self, arg1: ItemStack | None) -> None: ...
    @property
    def held_item_slot(self) -> int:
        """
        Gets or sets the slot number of the currently held item
        """
        ...
    @held_item_slot.setter
    def held_item_slot(self, arg1: int) -> None: ...
