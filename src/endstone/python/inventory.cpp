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
    py::enum_<EquipmentSlot>(m, "EquipmentSlot")
        .value("HAND", EquipmentSlot::Hand)
        .value("OFF_HAND", EquipmentSlot::OffHand)
        .value("FEET", EquipmentSlot::Feet)
        .value("LEGS", EquipmentSlot::Legs)
        .value("CHEST", EquipmentSlot::Chest)
        .value("HEAD", EquipmentSlot::Head)
        .value("BODY", EquipmentSlot::Body, "Only for certain entities such as horses and wolves.");

    py::class_<ItemType>(m, "ItemType", "Represents an item type.")
        .def_property_readonly("id", &ItemType::getId, "Return the identifier of this item type.")
        .def_property_readonly("key", &ItemType::getKey, "Return the namespaced identifier of this item type.")
        .def_property_readonly("translation_key", &ItemType::getTranslationKey,
                               "Get the translation key, suitable for use in a translation component.")
        .def_property_readonly("max_stack_size", &ItemType::getMaxStackSize,
                               "Gets the maximum amount of this item type that can be held in a stack.")
        .def_property_readonly("max_durability", &ItemType::getMaxDurability,
                               "Gets the maximum durability of this item type")
        .def_static("get", &ItemType::get, py::arg("name"), "Attempts to get the ItemType with the given name.",
                    py::return_value_policy::reference)
        .def("__str__", &ItemType::getId)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self == std::string_view())
        .def(py::self != std::string_view());

    py::class_<ItemMeta>(m, "ItemMeta", "Represents the metadata of a generic item.")
        .def("clone", &ItemMeta::clone, "Creates a clone of the current metadata.")
        .def_property_readonly("has_display_name", &ItemMeta::hasDisplayName, "Checks for existence of a display name.")
        .def_property("display_name", &ItemMeta::getDisplayName, &ItemMeta::setDisplayName,
                      "Gets or sets the display name.")
        .def_property_readonly("has_lore", &ItemMeta::hasLore, "Checks for existence of lore.")
        .def_property("lore", &ItemMeta::getLore, &ItemMeta::setLore, "Gets or sets the lore for this item.")
        .def_property_readonly("has_damage", &ItemMeta::hasDamage, "Checks to see if this item has damage.")
        .def_property("damage", &ItemMeta::getDamage, &ItemMeta::setDamage, "Gets or sets the damage.")
        .def_property_readonly("has_enchants", &ItemMeta::hasEnchants, "Checks for the existence of any enchantments.")
        .def("has_enchant", &ItemMeta::hasEnchant, py::arg("id"), "Checks for existence of the specified enchantment.")
        .def("get_enchant_level", &ItemMeta::getEnchantLevel, py::arg("id"),
             "Checks for the level of the specified enchantment.")
        .def_property_readonly("enchants", &ItemMeta::getEnchants, "Returns a copy the enchantments in this ItemMeta.")
        .def("add_enchant", &ItemMeta::addEnchant, py::arg("id"), py::arg("level"), py::arg("force") = false,
             "Adds the specified enchantment to this item meta.")
        .def("remove_enchant", &ItemMeta::removeEnchant, py::arg("id"),
             "Removes the specified enchantment from this item meta.")
        .def("remove_enchants", &ItemMeta::removeEnchants, "Removes all enchantments from this item meta.")
        .def_property_readonly("has_repair_cost", &ItemMeta::hasRepairCost,
                               "Checks to see if this item has a repair penalty.")
        .def_property("repair_cost", &ItemMeta::getRepairCost, &ItemMeta::setRepairCost,
                      "Gets or sets the repair penalty.")
        .def_property("is_unbreakable", &ItemMeta::isUnbreakable, &ItemMeta::setUnbreakable,
                      "Gets or sets the unbreakable tag. An unbreakable item will not lose durability.");

    py::class_<MapMeta, ItemMeta>(m, "MapMeta", "Represents the metadata for a map item.");

    py::class_<ItemFactory>(m, "ItemFactory")
        .def("get_item_meta", &ItemFactory::getItemMeta, py::arg("type"),
             "This creates a new item meta for the item type.")
        .def("is_applicable", &ItemFactory::isApplicable, py::arg("meta"), py::arg("type"),
             "This method checks the item meta to confirm that it is applicable (no data lost if applied) to the "
             "specified ItemStack")
        .def("equals", &ItemFactory::equals, py::arg("meta1"), py::arg("meta2"),
             "This method is used to compare two ItemMeta objects.")
        .def("as_meta_for", &ItemFactory::asMetaFor, py::arg("meta"), py::arg("type"),
             "Returns an appropriate item meta for the specified item type.");

    item_stack
        .def(py::init([](std::variant<const ItemType *, std::string> type, int amount) {
                 auto result =
                     std::visit(overloaded{
                                    [amount](const ItemType *arg) { return ItemStack::create(*arg, amount); },
                                    [amount](const std::string &arg) { return ItemStack::create(arg, amount); },
                                },
                                type);
                 if (!result) {
                     throw std::runtime_error(result.error());
                 }
                 return result.value();
             }),
             py::arg("type"), py::arg("amount") = 1)
        .def_property(
            "type", &ItemStack::getType,
            [](ItemStack &self, std::variant<const ItemType *, std::string> type) {
                std::visit(overloaded{
                               [&self](const ItemType *arg) { self.setType(*arg); },
                               [&self](const std::string &arg) { self.setType(arg); },
                           },
                           type);
            },
            py::return_value_policy::reference, "Gets or sets the type of this item.")
        .def_property("amount", &ItemStack::getAmount, &ItemStack::setAmount,
                      "Gets or sets the amount of items in this stack.")
        .def_property_readonly("max_stack_size", &ItemStack::getMaxStackSize,
                               "Get the maximum stack size for this item.")
        .def("is_similar", &ItemStack::isSimilar, py::arg("other"),
             "Checks if the two stacks are equal, but does not consider stack size (amount).")
        .def_property_readonly("item_meta", &ItemStack::getItemMeta, "Gets a copy of the ItemMeta of this ItemStack.")
        .def("set_item_meta", &ItemStack::setItemMeta, py::arg("meta"), "Set the ItemMeta of this ItemStack.")
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__str__", [](const ItemStack &self) { return fmt::format("{}", self); });

    py::class_<Inventory>(m, "Inventory", "Interface to the various inventories.")
        .def_property_readonly("size", &Inventory::getSize, "Returns the size of the inventory")
        .def_property_readonly("max_stack_size", &Inventory::getMaxStackSize,
                               "Returns the maximum stack size for an ItemStack in this inventory.")
        .def("get_item", &Inventory::getItem, py::arg("index"),
             "Returns the ItemStack found in the slot at the given index")
        .def("set_item", &Inventory::setItem, py::arg("index"), py::arg("item"),
             "Stores the ItemStack at the given index of the inventory.")
        .def(
            "add_item",
            [](Inventory &self, const py::args &args) {
                std::vector<const ItemStack *> items;
                items.reserve(args.size());
                for (auto obj : args) {
                    items.push_back(obj.cast<const ItemStack *>());
                }
                return self.addItem(items);
            },
            "Stores the given ItemStacks in the inventory.\n"
            "This will try to fill existing stacks and empty slots as well as it can.\n\n"
            "The returned map contains what it couldn't store, where the key is the index, and the value is the "
            "ItemStack.\nIf all items are stored, it will return an empty dict.")
        .def(
            "remove_item",
            [](Inventory &self, const py::args &args) {
                std::vector<const ItemStack *> items;
                items.reserve(args.size());
                for (auto obj : args) {
                    items.push_back(obj.cast<const ItemStack *>());
                }
                return self.removeItem(items);
            },
            "Removes the given ItemStacks from the inventory.\n"
            "It will try to remove 'as much as possible' from the types and amounts you give as arguments.\n\n"
            "The returned HashMap contains what it couldn't remove, where the key is the index, and the value is the "
            "ItemStack.\nIf all the given ItemStacks are removed, it will return an empty dict.")
        .def_property("contents", &Inventory::getContents, &Inventory::setContents,
                      "Returns all ItemStacks from the inventory")
        .def("contains", py::overload_cast<const ItemStack &, int>(&Inventory::contains, py::const_),
             "Checks if the inventory contains at least the minimum amount specified of exactly matching ItemStacks.\n"
             "An ItemStack only counts if both the type and the amount of the stack match.",
             py::arg("item"), py::arg("amount"))
        .def("contains", py::overload_cast<const ItemStack &>(&Inventory::contains, py::const_),
             "Checks if the inventory contains any ItemStacks with the given ItemStack.\n"
             "This will only return true if both the type and the amount of the stack match.",
             py::arg("item"))
        .def("contains", py::overload_cast<const std::string &>(&Inventory::contains, py::const_),
             "Checks if the inventory contains any ItemStacks with the given ItemType.", py::arg("type"))
        .def("contains_at_least", py::overload_cast<const ItemStack &, int>(&Inventory::containsAtLeast, py::const_),
             "Checks if the inventory contains ItemStacks matching the given ItemStack whose amounts sum to at least "
             "the minimum amount specified.",
             py::arg("item"), py::arg("amount"))
        .def("contains_at_least", py::overload_cast<const std::string &, int>(&Inventory::containsAtLeast, py::const_),
             "Checks if the inventory contains any ItemStacks with the given ItemType, adding to at least the minimum "
             "amount specified.",
             py::arg("type"), py::arg("amount"))
        .def("all", py::overload_cast<const ItemStack &>(&Inventory::all, py::const_), py::arg("item"),
             "Finds all slots in the inventory containing any ItemStacks with the given ItemStack.\n"
             "This will only match slots if both the type and the amount of the stack match\n"
             "The returned map contains entries where, the key is the slot index, and the value is the "
             "ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.")
        .def("all", py::overload_cast<const std::string &>(&Inventory::all, py::const_), py::arg("type"),
             "Finds all slots in the inventory containing any ItemStacks with the given ItemType.\n"
             "The returned map contains entries where, the key is the slot index, and the value is the "
             "ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.")
        .def("first", py::overload_cast<const ItemStack &>(&Inventory::first, py::const_), py::arg("item"),
             "Returns the first slot in the inventory containing an ItemStack with the given stack.\n"
             "This will only match slots if both the type and the amount of the stack match\n"
             "The returned map contains entries where, the key is the slot index, and the value is the "
             "ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.")
        .def("first", py::overload_cast<const std::string &>(&Inventory::first, py::const_), py::arg("type"),
             "Finds the first slot in the inventory containing an ItemStack with the given ItemType.\n"
             "The returned map contains entries where, the key is the slot index, and the value is the "
             "ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.")
        .def_property_readonly("first_empty", &Inventory::firstEmpty, "Returns the first empty Slot.")
        .def_property_readonly("is_empty", &Inventory::isEmpty,
                               " Check whether this inventory is empty. An inventory is considered to be empty if "
                               "there are no ItemStacks in any slot of this inventory.")
        .def("remove", py::overload_cast<const ItemStack &>(&Inventory::remove), py::arg("item"),
             "Removes all stacks in the inventory matching the given stack.\n"
             "This will only match a slot if both the type and the amount of the stack match")
        .def("remove", py::overload_cast<const std::string &>(&Inventory::remove), py::arg("type"),
             "Removes all stacks in the inventory matching the given ItemType.")
        .def("clear", py::overload_cast<int>(&Inventory::clear), py::arg("index"),
             "Clears out a particular slot in the index.")
        .def("clear", py::overload_cast<>(&Inventory::clear), "Clears out the whole Inventory.")
        .def("__len__", &Inventory::getSize, "Returns the size of the inventory")
        .def("__getitem__", &Inventory::getItem, py::arg("index"),
             "Returns the ItemStack found in the slot at the given index")
        .def("__setitem__", &Inventory::setItem, py::arg("index"), py::arg("item"),
             "Stores the ItemStack at the given index of the inventory.")
        .def("__contains__", py::overload_cast<const ItemStack &>(&Inventory::contains, py::const_),
             "Checks if the inventory contains any ItemStacks with the given ItemStack.", py::arg("item"))
        .def("__contains__", py::overload_cast<const std::string &>(&Inventory::contains, py::const_),
             "Checks if the inventory contains any ItemStacks with the given ItemType.", py::arg("type"));

    py::class_<PlayerInventory, Inventory>(
        m, "PlayerInventory",
        "Interface to the inventory of a Player, including the four armor slots and any extra slots.")
        .def_property("helmet", &PlayerInventory::getHelmet, &PlayerInventory::setHelmet,
                      "Gets or sets the ItemStack in the helmet slot")
        .def_property("chestplate", &PlayerInventory::getChestplate, &PlayerInventory::setChestplate,
                      "Gets or sets the ItemStack in the chestplate slot")
        .def_property("leggings", &PlayerInventory::getLeggings, &PlayerInventory::setLeggings,
                      "Gets or sets the ItemStack in the leg slot")
        .def_property("boots", &PlayerInventory::getBoots, &PlayerInventory::setBoots,
                      "Gets or sets the ItemStack in the boots slot")
        .def_property("item_in_main_hand", &PlayerInventory::getItemInMainHand, &PlayerInventory::setItemInMainHand,
                      "Gets or sets the item the player is currently holding in their main hand.")
        .def_property("item_in_off_hand", &PlayerInventory::getItemInOffHand, &PlayerInventory::setItemInOffHand,
                      "Gets or sets the item the player is currently holding in their off hand.")
        .def_property("held_item_slot", &PlayerInventory::getHeldItemSlot, &PlayerInventory::setHeldItemSlot,
                      "Gets or sets the slot number of the currently held item");
}

}  // namespace endstone::python
