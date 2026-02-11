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

#include "endstone/core/inventory/player_inventory.h"

#include "bedrock/network/packet/inventory_slot_packet.h"
#include "endstone/core/inventory/item_stack.h"

namespace endstone::core {

int EndstonePlayerInventory::getSize() const
{
    return EndstoneInventory::getSize();
}

int EndstonePlayerInventory::getMaxStackSize() const
{
    return EndstoneInventory::getMaxStackSize();
}

std::optional<ItemStack> EndstonePlayerInventory::getItem(int index) const
{
    return EndstoneInventory::getItem(index);
}

void EndstonePlayerInventory::setItem(int index, std::optional<ItemStack> item)
{
    EndstoneInventory::setItem(index, std::move(item));
}

std::unordered_map<int, ItemStack> EndstonePlayerInventory::addItem(std::vector<ItemStack> items)
{
    return EndstoneInventory::addItem(std::move(items));
}

std::unordered_map<int, ItemStack> EndstonePlayerInventory::removeItem(std::vector<ItemStack> items)
{
    return EndstoneInventory::removeItem(std::move(items));
}

std::vector<std::optional<ItemStack>> EndstonePlayerInventory::getContents() const
{
    return EndstoneInventory::getContents();
}

void EndstonePlayerInventory::setContents(std::vector<std::optional<ItemStack>> items)
{
    return EndstoneInventory::setContents(std::move(items));
}

bool EndstonePlayerInventory::contains(const std::string &type) const
{
    return EndstoneInventory::contains(type);
}

bool EndstonePlayerInventory::contains(const ItemStack &item) const
{
    return EndstoneInventory::contains(item);
}

bool EndstonePlayerInventory::contains(const ItemStack &item, int amount) const
{
    return EndstoneInventory::contains(item, amount);
}

bool EndstonePlayerInventory::containsAtLeast(const std::string &type, int amount) const
{
    return EndstoneInventory::containsAtLeast(type, amount);
}

bool EndstonePlayerInventory::containsAtLeast(const ItemStack &item, int amount) const
{
    return EndstoneInventory::containsAtLeast(item, amount);
}

std::unordered_map<int, ItemStack> EndstonePlayerInventory::all(const std::string &type) const
{
    return EndstoneInventory::all(type);
}

std::unordered_map<int, ItemStack> EndstonePlayerInventory::all(const ItemStack &item) const
{
    return EndstoneInventory::all(item);
}

int EndstonePlayerInventory::first(const std::string &type) const
{
    return EndstoneInventory::first(type);
}

int EndstonePlayerInventory::first(const ItemStack &item) const
{
    return EndstoneInventory::first(item);
}

int EndstonePlayerInventory::firstEmpty() const
{
    return EndstoneInventory::firstEmpty();
}

void EndstonePlayerInventory::remove(const std::string &type)
{
    return EndstoneInventory::remove(type);
}

void EndstonePlayerInventory::remove(const ItemStack &item)
{
    EndstoneInventory::remove(item);
}

void EndstonePlayerInventory::clear(int index)
{
    EndstoneInventory::clear(index);
}

bool EndstonePlayerInventory::isEmpty() const
{
    return EndstoneInventory::isEmpty();
}

void EndstonePlayerInventory::clear()
{
    EndstoneInventory::clear();
}

std::optional<ItemStack> EndstonePlayerInventory::getHelmet() const
{
    return getArmor(ArmorSlot::Head);
}

std::optional<ItemStack> EndstonePlayerInventory::getChestplate() const
{
    return getArmor(ArmorSlot::Torso);
}

std::optional<ItemStack> EndstonePlayerInventory::getLeggings() const
{
    return getArmor(ArmorSlot::Legs);
}

std::optional<ItemStack> EndstonePlayerInventory::getBoots() const
{
    return getArmor(ArmorSlot::Feet);
}

void EndstonePlayerInventory::setHelmet(std::optional<ItemStack> helmet)
{
    setArmor(ArmorSlot::Head, helmet);
}

void EndstonePlayerInventory::setChestplate(std::optional<ItemStack> chestplate)
{
    setArmor(ArmorSlot::Torso, chestplate);
}

void EndstonePlayerInventory::setLeggings(std::optional<ItemStack> leggings)
{
    setArmor(ArmorSlot::Legs, leggings);
}

void EndstonePlayerInventory::setBoots(std::optional<ItemStack> boots)
{
    setArmor(ArmorSlot::Feet, boots);
}

std::optional<ItemStack> EndstonePlayerInventory::getItemInMainHand() const
{
    return getItem(holder_.getSelectedItemSlot());
}

void EndstonePlayerInventory::setItemInMainHand(std::optional<ItemStack> item)
{
    setItem(holder_.getSelectedItemSlot(), std::move(item));
}

std::optional<ItemStack> EndstonePlayerInventory::getItemInOffHand() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getOffhandSlot());
}

void EndstonePlayerInventory::setItemInOffHand(std::optional<ItemStack> item)
{
    const auto item_stack = item.has_value() ? EndstoneItemStack::toMinecraft(*item) : ::ItemStack::EMPTY_ITEM;
    holder_.setOffhandSlot(item_stack);
}

int EndstonePlayerInventory::getHeldItemSlot() const
{
    return holder_.getSelectedItemSlot();
}

void EndstonePlayerInventory::setHeldItemSlot(int slot)
{
    Preconditions::checkArgument(slot >= 0 && slot < FillingContainer::HOTBAR_SIZE,
                                 "Slot ({}) is not between 0 and {} inclusive", slot,
                                 FillingContainer::HOTBAR_SIZE - 1);
    holder_.setSelectedSlot(slot);
}

std::optional<ItemStack> EndstonePlayerInventory::getArmor(ArmorSlot slot) const
{
    auto item = holder_.getArmor(slot);
    if (item.isNull()) {
        return std::nullopt;
    }
    return EndstoneItemStack::fromMinecraft(item);
}

void EndstonePlayerInventory::setArmor(ArmorSlot slot, std::optional<ItemStack> armor)
{
    const auto item_stack = armor.has_value() ? EndstoneItemStack::toMinecraft(armor.value()) : ::ItemStack::EMPTY_ITEM;
    holder_.setArmor(slot, item_stack);
}

}  // namespace endstone::core
