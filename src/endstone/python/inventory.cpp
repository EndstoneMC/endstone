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

void init_inventory(py::module_ &m)
{
    py::class_<ItemMeta>(m, "ItemMeta", "Represents the metadata of a generic item.")
        .def("clone", &ItemMeta::clone, "Creates a clone of the current metadata.")
        .def_property_readonly("has_display_name", &ItemMeta::hasDisplayName, "Checks for existence of a display name.")
        .def_property("display_name", &ItemMeta::getDisplayName, &ItemMeta::setDisplayName,
                      "Gets or sets the display name.")
        .def_property_readonly("has_lore", &ItemMeta::hasLore, "Checks for existence of lore.")
        .def_property("lore", &ItemMeta::getLore, &ItemMeta::setLore, "Gets or sets the lore for this item.")
        .def_property_readonly("has_damage", &ItemMeta::hasDamage, "Checks to see if this item has damage")
        .def_property("damage", &ItemMeta::getDamage, &ItemMeta::setDamage, "Gets or sets the damage.");

    py::class_<MapMeta, ItemMeta>(m, "MapMeta", "Represents the metadata for a map item.");

    py::class_<ItemStack>(m, "ItemStack", "Represents a stack of items.")
        .def(py::init<std::string, int>(), py::arg("type") = "minecraft:air", py::arg("amount") = 1)
        .def_property("type", &ItemStack::getType, &ItemStack::setType, "Gets or sets the type of this item.")
        .def_property("amount", &ItemStack::getAmount, &ItemStack::setAmount,
                      "Gets or sets the amount of items in this stack.")
        .def_property_readonly("item_meta", &ItemStack::getItemMeta, "Gets a copy of the ItemMeta of this ItemStack.")
        .def("set_item_meta", &ItemStack::setItemMeta, py::arg("meta"), "Set the ItemMeta of this ItemStack.")
        .def("__str__", [](const ItemStack &self) { return fmt::format("{}", self); });

    py::class_<Inventory>(m, "Inventory", "Interface to the various inventories.")
        .def_property_readonly("size", &Inventory::getSize, "Returns the size of the inventory")
        .def_property_readonly("max_stack_size", &Inventory::getMaxStackSize,
                               "Returns the maximum stack size for an ItemStack in this inventory.")
        .def("get_item", &Inventory::getItem, py::arg("index"),
             "Returns the ItemStack found in the slot at the given index")
        .def("set_item", &Inventory::setItem, py::arg("index"), py::arg("item"),
             "Stores the ItemStack at the given index of the inventory.")
        .def("add_item", &Inventory::addItem, py::arg("item"),
             "Stores the given ItemStacks in the inventory. This will try to fill existing stacks and empty slots as "
             "well as it can.")
        .def_property_readonly("contents", &Inventory::getContents, "Returns all ItemStacks from the inventory")
        .def("first", &Inventory::first, py::arg("item"),
             "Returns the first slot in the inventory containing an ItemStack with the given stack.")
        .def_property_readonly("is_empty", &Inventory::isEmpty,
                               " Check whether this inventory is empty. An inventory is considered to be empty if "
                               "there are no ItemStacks in any slot of this inventory.")
        .def("clear", &Inventory::clear, "Clears out the whole Inventory.")
        .def("__len__", &Inventory::getSize, "Returns the size of the inventory")
        .def("__get_item__", &Inventory::getItem, py::arg("index"),
             "Returns the ItemStack found in the slot at the given index")
        .def("__set_item__", &Inventory::setItem, py::arg("index"), py::arg("item"),
             "Stores the ItemStack at the given index of the inventory.");

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
