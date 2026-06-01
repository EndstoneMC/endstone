// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

void init_inventory(py::module_ &m, py::class_<ItemStack> &item_stack)
{
    py::native_enum<EquipmentSlot>(m, "EquipmentSlot", "enum.Enum")
        .value("HAND", EquipmentSlot::Hand)
        .value("OFF_HAND", EquipmentSlot::OffHand)
        .value("FEET", EquipmentSlot::Feet)
        .value("LEGS", EquipmentSlot::Legs)
        .value("CHEST", EquipmentSlot::Chest)
        .value("HEAD", EquipmentSlot::Head)
        .value("BODY", EquipmentSlot::Body, "Only for certain entities such as horses and wolves.")
        .finalize();

    py::class_<ItemType>(m, "ItemType", "Represents an item type.")
        .def_property_readonly("id", &ItemType::getId, "The identifier of this item type.")
        .def_property_readonly("translation_key", py::overload_cast<>(&ItemType::getTranslationKey, py::const_),
                               "The translation key, suitable for use in a translation component.")
        .def("get_translation_key", py::overload_cast<int>(&ItemType::getTranslationKey, py::const_),
             py::arg("data") = 0, R"doc(
    Get the translation key, suitable for use in a translation component.

    Args:
        data: Data for this item type.

    Returns:
        The translation key.
)doc")
        .def_property_readonly("max_stack_size", &ItemType::getMaxStackSize,
                               "The maximum amount of this item type that can be held in a stack.")
        .def_property_readonly("max_durability", &ItemType::getMaxDurability,
                               "The maximum durability of this item type.")
        .def("create_item_stack", py::overload_cast<int>(&ItemType::createItemStack, py::const_), py::arg("amount") = 1,
             R"doc(
    Constructs a new ItemStack with this item type.

    Args:
        amount: The amount in the stack.

    Returns:
        An ItemStack of this item type.
)doc")
        .def_static("get", &ItemType::get, py::arg("name"), "Attempts to get the ItemType with the given name.",
                    py::return_value_policy::reference)
        .def("__str__", [](const ItemType &self) { return std::string(self.getId()); })
        .def("__hash__", [](const ItemType &self) { return std::hash<ItemTypeId>{}(self.getId()); })
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self == std::string_view())
        .def(py::self != std::string_view());

    py::class_<ItemMeta>(m, "ItemMeta", "Represents the metadata of a generic item.")
        .def("clone", &ItemMeta::clone, R"doc(
    Creates a clone of the current metadata.

    Returns:
        A copy of the metadata containing the same state as the original.
)doc")
        .def_property_readonly("has_display_name", &ItemMeta::hasDisplayName, "Whether this has a display name.")
        .def_property("display_name", &ItemMeta::getDisplayName, &ItemMeta::setDisplayName,
                      "The display name that is set.")
        .def_property_readonly("has_lore", &ItemMeta::hasLore, "Whether this has lore.")
        .def_property("lore", &ItemMeta::getLore, &ItemMeta::setLore, "The lore for this item.")
        .def_property_readonly("has_damage", &ItemMeta::hasDamage, "Whether this item has damage.")
        .def_property("damage", &ItemMeta::getDamage, &ItemMeta::setDamage, "The damage.")
        .def_property_readonly("has_enchants", &ItemMeta::hasEnchants,
                               "Whether an enchantment exists on this meta.")
        .def("has_enchant", &ItemMeta::hasEnchant, py::arg("id"), R"doc(
    Checks for existence of the specified enchantment.

    Args:
        id: Enchantment id to check.

    Returns:
        True if this enchantment exists for this meta.
)doc")
        .def("get_enchant_level", &ItemMeta::getEnchantLevel, py::arg("id"), R"doc(
    Checks for the level of the specified enchantment.

    Args:
        id: Enchantment id to check.

    Returns:
        The level that the specified enchantment has, or 0 if none.
)doc")
        .def_property_readonly("enchants", &ItemMeta::getEnchants, R"doc(
    Returns a copy of the enchantments in this ItemMeta.

    Returns an empty map if none.
)doc")
        .def("add_enchant", &ItemMeta::addEnchant, py::arg("id"), py::arg("level"), py::arg("force") = false, R"doc(
    Adds the specified enchantment to this item meta.

    Args:
        id: Enchantment id to add.
        level: Level for the enchantment.
        force: This indicates the enchantment should be applied, ignoring the level limit.

    Returns:
        True if the item meta changed as a result of this call, False otherwise.
)doc")
        .def("remove_enchant", &ItemMeta::removeEnchant, py::arg("id"), R"doc(
    Removes the specified enchantment from this item meta.

    Args:
        id: Enchantment id to remove.

    Returns:
        True if the item meta changed as a result of this call, False otherwise.
)doc")
        .def("remove_enchants", &ItemMeta::removeEnchants, "Removes all enchantments from this item meta.")
        .def_property_readonly("has_repair_cost", &ItemMeta::hasRepairCost,
                               "Whether this item has a repair penalty.")
        .def_property("repair_cost", &ItemMeta::getRepairCost, &ItemMeta::setRepairCost, "The repair penalty.")
        .def_property("is_unbreakable", &ItemMeta::isUnbreakable, &ItemMeta::setUnbreakable,
                      "The unbreakable tag. An unbreakable item will not lose durability.");

    py::class_<MapMeta, ItemMeta>(m, "MapMeta", "Represents the metadata for a map item.")
        .def_property_readonly("has_map_id", &MapMeta::hasMapId, "Whether this has a map ID number.")
        .def_property("map_id", &MapMeta::getMapId, &MapMeta::setMapId,
                      "The map ID. This is used to determine what map is displayed.")
        .def_property_readonly("has_map_view", &MapMeta::hasMapView, "Whether this item has an associated map.")
        .def_property("map_view", &MapMeta::getMapView, &MapMeta::setMapView, py::return_value_policy::reference,
                      "The map view associated with this map item.");

    py::class_<WritableBookMeta, ItemMeta>(m, "WritableBookMeta", "Represents the meta for a writable book that can have pages.")
        .def_property_readonly("has_pages", &WritableBookMeta::hasPages, "Whether the book has pages.")
        .def("get_page", &WritableBookMeta::getPage, py::arg("page"), R"doc(
    Gets the specified page in the book. The given page must exist.

    Note:
        Pages are 1-indexed.

    Args:
        page: The page number to get, in range [1, page_count].

    Returns:
        The page from the book.
)doc")
        .def("set_page", &WritableBookMeta::setPage, py::arg("page"), py::arg("data"), R"doc(
    Sets the specified page in the book. Pages of the book must be contiguous.

    Note:
        The data can be up to 1024 characters in length, additional characters are truncated.

    Pages are 1-indexed.

    Args:
        page: The page number to set, in range [1, page_count].
        data: The data to set for that page.
)doc")
        .def_property(
            "pages", &WritableBookMeta::getPages,
            [](WritableBookMeta &self, std::vector<std::string> pages) { self.setPages(std::move(pages)); },
            "All the pages in the book.")
        .def(
            "add_page",
            [](WritableBookMeta &self, const py::Args<std::string> &pages) {
                std::vector<std::string> vec;
                vec.reserve(pages.size());
                for (auto page : pages) {
                    vec.push_back(page.cast<std::string>());
                }
                self.addPages(std::move(vec));
            },
            R"doc(
    Adds new pages to the end of the book.

    Note:
        Up to a maximum of 50 pages with 256 characters per page.

    Args:
        pages: A list of strings, each being a page.
)doc")
        .def_property_readonly("page_count", &WritableBookMeta::getPageCount, "The number of pages in the book.");

    py::native_enum<BookMeta::Generation>(m, "BookMetaGeneration", "enum.Enum")
        .value("ORIGINAL", BookMeta::Generation::Original)
        .value("COPY_OF_ORIGINAL", BookMeta::Generation::CopyOfOriginal)
        .value("COPY_OF_COPY", BookMeta::Generation::CopyOfCopy)
        .finalize();

    py::class_<BookMeta, WritableBookMeta>(m, "BookMeta",
                                            "Represents the meta for a written book that can have a title, an author, and pages.")
        .def_property_readonly("has_title", &BookMeta::hasTitle, "Whether the book has a title.")
        .def_property("title", &BookMeta::getTitle, &BookMeta::setTitle, "The title of the book.")
        .def_property_readonly("has_author", &BookMeta::hasAuthor, "Whether the book has an author.")
        .def_property("author", &BookMeta::getAuthor, &BookMeta::setAuthor, "The author of the book.")
        .def_property_readonly("has_generation", &BookMeta::hasGeneration, "Whether the book has a generation level.")
        .def_property("generation", &BookMeta::getGeneration, &BookMeta::setGeneration, "The generation of the book.");

    py::class_<CrossbowMeta, ItemMeta>(m, "CrossbowMeta",
                                        "Represents the meta for a crossbow that can have a charged projectile.")
        .def_property_readonly("has_charged_projectile", &CrossbowMeta::hasChargedProjectile,
                               "Whether the crossbow has a charged projectile.")
        .def_property("charged_projectile", &CrossbowMeta::getChargedProjectile, &CrossbowMeta::setChargedProjectile,
                      "The charged projectile, or None if none.");

    py::class_<ItemFactory>(m, "ItemFactory", "An interface to the methods used to create item metadata.")
        .def("get_item_meta", &ItemFactory::getItemMeta, py::arg("type"), R"doc(
    This creates a new item meta for the item type.

    Args:
        type: The item type to consider as base for the meta.

    Returns:
        A new ItemMeta that could be applied to an item stack of the specified item type.
)doc")
        .def("is_applicable", &ItemFactory::isApplicable, py::arg("meta"), py::arg("type"), R"doc(
    This method checks the item meta to confirm that it is applicable (no data lost if applied) to the specified ItemStack.

    Args:
        meta: Meta to check.
        type: The item type that meta will be applied to.

    Returns:
        True if the meta can be applied without losing data, False otherwise.
)doc")
        .def("equals", &ItemFactory::equals, py::arg("meta1"), py::arg("meta2"), R"doc(
    This method is used to compare two ItemMeta objects.

    Args:
        meta1: First meta to compare; may be None to indicate no data.
        meta2: Second meta to compare; may be None to indicate no data.

    Returns:
        False if one of the meta has data the other does not, otherwise True.
)doc")
        .def("as_meta_for", &ItemFactory::asMetaFor, py::arg("meta"), py::arg("type"), R"doc(
    Returns an appropriate item meta for the specified item type.

    The item meta returned will always be a valid meta for a given ItemStack of the specified item type. It may be a
    more or less specific meta, and could also be the same meta or meta type as the parameter. The item meta returned
    will also always be the most appropriate meta.

    Args:
        meta: The meta to convert.
        type: The item type to convert the meta for.

    Returns:
        An appropriate item meta for the specified item type.
)doc");

    item_stack
        .def(py::init([](ItemTypeId type, const int amount, const int data) {
                 if (const auto *item = ItemType::get(type); !item) {
                     throw std::runtime_error(std::format("Unknown item type: {}", type));
                 }
                 if (amount < 1 || amount > 0xff) {
                     throw std::runtime_error(
                         std::format("Item stack amount must be between 1 to 255, got {}.", amount));
                 }
                 return ItemStack(type, amount, data);
             }),
             py::arg("type"), py::arg("amount") = 1, py::arg("data") = 0)
        .def_property("type", &ItemStack::getType, &ItemStack::setType, py::return_value_policy::reference,
                      "The type of this item.")
        .def_property("amount", &ItemStack::getAmount, &ItemStack::setAmount, "The amount of items in this stack.")
        .def_property("data", &ItemStack::getData, &ItemStack::setData, "The data for this stack of items.")
        .def_property_readonly("translation_key", &ItemStack::getTranslationKey,
                               "The translation key for this item.")
        .def_property_readonly("max_stack_size", &ItemStack::getMaxStackSize,
                               "The maximum stack size for this item.")
        .def("is_similar", &ItemStack::isSimilar, py::arg("other"), R"doc(
    Checks if the two stacks are equal, but does not consider stack size (amount).

    Args:
        other: The item stack to compare to.

    Returns:
        True if the two stacks are equal, ignoring the amount.
)doc")
        .def_property_readonly("item_meta", &ItemStack::getItemMeta, "A copy of the ItemMeta of this ItemStack.")
        .def("set_item_meta", &ItemStack::setItemMeta, py::arg("meta"), R"doc(
    Set the ItemMeta of this ItemStack.

    Args:
        meta: New ItemMeta, or None to clear the metadata.

    Returns:
        True if successfully applied ItemMeta.
)doc")
        .def_property("nbt", &ItemStack::getNbt, &ItemStack::setNbt, "The NBT compound tag of this item stack.")
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__str__", [](const ItemStack &self) { return std::format("{}", self); });

    py::class_<Inventory>(m, "Inventory", "Interface to the various inventories.")
        .def_property_readonly("size", &Inventory::getSize, "The size of the inventory.")
        .def_property_readonly("max_stack_size", &Inventory::getMaxStackSize,
                               "The maximum stack size for an ItemStack in this inventory.")
        .def("get_item", &Inventory::getItem, py::arg("index"), R"doc(
    Returns the ItemStack found in the slot at the given index.

    Args:
        index: The index of the Slot's ItemStack to return.

    Returns:
        The ItemStack in the slot.
)doc")
        .def("set_item", &Inventory::setItem, py::arg("index"), py::arg("item"), R"doc(
    Stores the ItemStack at the given index of the inventory.

    Args:
        index: The index where to put the ItemStack.
        item: The ItemStack to set.
)doc")
        .def(
            "add_item",
            [](Inventory &self, const py::args &item) {
                std::vector<ItemStack> items;
                items.reserve(item.size());
                for (auto obj : item) {
                    items.push_back(obj.cast<ItemStack>());
                }
                return self.addItem(std::move(items));
            },
            R"doc(
    Stores the given ItemStacks in the inventory.

    Note:
        This will try to fill existing stacks and empty slots as well as it can.

    Note:
        The returned map contains what it couldn't store, where the key is the index, and the value is the
        ItemStack. If all items are stored, it will return an empty dict.

    Returns:
        A map containing items that couldn't be added.
)doc")
        .def(
            "remove_item",
            [](Inventory &self, const py::args &item) {
                std::vector<ItemStack> items;
                items.reserve(item.size());
                for (auto obj : item) {
                    items.push_back(obj.cast<ItemStack>());
                }
                return self.removeItem(std::move(items));
            },
            R"doc(
    Removes the given ItemStacks from the inventory.

    Note:
        It will try to remove 'as much as possible' from the types and amounts you give as arguments.

    Note:
        The returned map contains what it couldn't remove, where the key is the index, and the value is the
        ItemStack. If all the given ItemStacks are removed, it will return an empty dict.

    Returns:
        A map containing items that couldn't be removed.
)doc")
        .def_property("contents", &Inventory::getContents, &Inventory::setContents,
                      "All ItemStacks from the inventory. Empty slots are represented as None.")
        .def("contains", py::overload_cast<const ItemStack &, int>(&Inventory::contains, py::const_),
             py::arg("item"), py::arg("amount"), R"doc(
    Checks if the inventory contains at least the minimum amount specified of exactly matching ItemStacks.

    Note:
        An ItemStack only counts if both the type and the amount of the stack match.

    Args:
        item: The ItemStack to match against.
        amount: How many identical stacks to check for.

    Returns:
        True if amount less than 1 or if amount of exactly matching ItemStacks were found, False otherwise.
)doc")
        .def("contains", py::overload_cast<const ItemStack &>(&Inventory::contains, py::const_), py::arg("item"),
             R"doc(
    Checks if the inventory contains any ItemStacks with the given ItemStack.

    Note:
        This will only return True if both the type and the amount of the stack match.

    Args:
        item: The ItemStack to match against.

    Returns:
        True if any exactly matching ItemStacks were found, False otherwise.
)doc")
        .def("contains", py::overload_cast<ItemTypeId>(&Inventory::contains, py::const_), py::arg("type"),
             R"doc(
    Checks if the inventory contains any ItemStacks with the given ItemType.

    Args:
        type: The item type to check for.

    Returns:
        True if an ItemStack is found with the given ItemType.
)doc")
        .def("contains_at_least", py::overload_cast<const ItemStack &, int>(&Inventory::containsAtLeast, py::const_),
             py::arg("item"), py::arg("amount"), R"doc(
    Checks if the inventory contains ItemStacks matching the given ItemStack whose amounts sum to at least the
    minimum amount specified.

    Args:
        item: The ItemStack to match against.
        amount: The minimum amount.

    Returns:
        True if amount less than 1 or enough ItemStacks were found to add to the given amount, False otherwise.
)doc")
        .def("contains_at_least", py::overload_cast<ItemTypeId, int>(&Inventory::containsAtLeast, py::const_),
             py::arg("type"), py::arg("amount"), R"doc(
    Checks if the inventory contains any ItemStacks with the given ItemType, adding to at least the minimum
    amount specified.

    Args:
        type: The ItemType to check for.
        amount: The minimum amount.

    Returns:
        True if amount is less than 1, True if enough ItemStacks were found to add to the given amount.
)doc")
        .def("all", py::overload_cast<const ItemStack &>(&Inventory::all, py::const_), py::arg("item"), R"doc(
    Finds all slots in the inventory containing any ItemStacks with the given ItemStack.

    Note:
        This will only match slots if both the type and the amount of the stack match.

    The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot.
    If no matching ItemStack is found, an empty map is returned.

    Args:
        item: The ItemStack to match against.

    Returns:
        A map from slot indexes to item at index.
)doc")
        .def("all", py::overload_cast<ItemTypeId>(&Inventory::all, py::const_), py::arg("type"), R"doc(
    Finds all slots in the inventory containing any ItemStacks with the given ItemType.

    The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot.
    If no matching ItemStack is found, an empty map is returned.

    Args:
        type: The ItemType to match against.

    Returns:
        A map from slot indexes to item at index.
)doc")
        .def("first", py::overload_cast<const ItemStack &>(&Inventory::first, py::const_), py::arg("item"), R"doc(
    Returns the first slot in the inventory containing an ItemStack with the given stack.

    Args:
        item: The ItemStack to match against.

    Returns:
        The slot index of the given ItemStack or -1 if not found.
)doc")
        .def("first", py::overload_cast<ItemTypeId>(&Inventory::first, py::const_), py::arg("type"), R"doc(
    Finds the first slot in the inventory containing an ItemStack with the given ItemType.

    Args:
        type: The ItemType to look for.

    Returns:
        The slot index of the given ItemType or -1 if not found.
)doc")
        .def_property_readonly("first_empty", &Inventory::firstEmpty,
                               "The first empty Slot found, or -1 if no empty slots.")
        .def_property_readonly("is_empty", &Inventory::isEmpty, R"doc(
    Whether this inventory is empty.

    An inventory is considered to be empty if there are no ItemStacks in any slot of this inventory.
)doc")
        .def("remove", py::overload_cast<const ItemStack &>(&Inventory::remove), py::arg("item"), R"doc(
    Removes all stacks in the inventory matching the given stack.

    Note:
        This will only match a slot if both the type and the amount of the stack match.

    Args:
        item: The ItemStack to match against.
)doc")
        .def("remove", py::overload_cast<ItemTypeId>(&Inventory::remove), py::arg("type"), R"doc(
    Removes all stacks in the inventory matching the given ItemType.

    Args:
        type: The ItemType to remove.
)doc")
        .def("clear", py::overload_cast<int>(&Inventory::clear), py::arg("index"), R"doc(
    Clears out a particular slot in the index.

    Args:
        index: The index to empty.
)doc")
        .def("clear", py::overload_cast<>(&Inventory::clear), "Clears out the whole Inventory.")
        .def("__len__", &Inventory::getSize, "Returns the size of the inventory.")
        .def("__getitem__", &Inventory::getItem, py::arg("index"),
             "Returns the ItemStack found in the slot at the given index.")
        .def("__setitem__", &Inventory::setItem, py::arg("index"), py::arg("item"),
             "Stores the ItemStack at the given index of the inventory.")
        .def("__contains__", py::overload_cast<const ItemStack &>(&Inventory::contains, py::const_), py::arg("item"),
             "Checks if the inventory contains any ItemStacks with the given ItemStack.")
        .def("__contains__", py::overload_cast<ItemTypeId>(&Inventory::contains, py::const_), py::arg("type"),
             "Checks if the inventory contains any ItemStacks with the given ItemType.");

    py::class_<PlayerInventory, Inventory>(
        m, "PlayerInventory",
        "Interface to the inventory of a Player, including the four armor slots and any extra slots.")
        .def_property("helmet", &PlayerInventory::getHelmet, &PlayerInventory::setHelmet,
                      "The ItemStack in the helmet slot.")
        .def_property("chestplate", &PlayerInventory::getChestplate, &PlayerInventory::setChestplate,
                      "The ItemStack in the chestplate slot.")
        .def_property("leggings", &PlayerInventory::getLeggings, &PlayerInventory::setLeggings,
                      "The ItemStack in the leg slot.")
        .def_property("boots", &PlayerInventory::getBoots, &PlayerInventory::setBoots,
                      "The ItemStack in the boots slot.")
        .def_property("item_in_main_hand", &PlayerInventory::getItemInMainHand, &PlayerInventory::setItemInMainHand,
                      "The item the player is currently holding in their main hand.")
        .def_property("item_in_off_hand", &PlayerInventory::getItemInOffHand, &PlayerInventory::setItemInOffHand,
                      "The item the player is currently holding in their off hand.")
        .def_property("held_item_slot", &PlayerInventory::getHeldItemSlot, &PlayerInventory::setHeldItemSlot,
                      "The slot number of the currently held item.");
}

}  // namespace endstone::python
