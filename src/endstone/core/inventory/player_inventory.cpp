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

std::unique_ptr<ItemStack> EndstonePlayerInventory::getItem(int index) const
{
    return EndstoneInventory::getItem(index);
}

void EndstonePlayerInventory::setItem(int index, const ItemStack *item)
{
    EndstoneInventory::setItem(index, item);
    // Notify the client about the updated item in the slot
    // https://github.com/EndstoneMC/endstone/issues/242
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::InventorySlot);
    auto &pk = static_cast<InventorySlotPacket &>(*packet);
    pk.inventory_id = holder_.getSupplies().getSelectedContainerId();
    pk.item = EndstoneItemStack::toMinecraft(item);
    pk.slot = index;
    holder_.sendNetworkPacket(pk);
}

std::unordered_map<int, ItemStack *> EndstonePlayerInventory::addItem(std::vector<ItemStack *> items)
{
    return EndstoneInventory::addItem(items);
}

std::unordered_map<int, ItemStack *> EndstonePlayerInventory::removeItem(std::vector<ItemStack *> items)
{
    return EndstoneInventory::removeItem(items);
}

std::vector<std::unique_ptr<ItemStack>> EndstonePlayerInventory::getContents() const
{
    return EndstoneInventory::getContents();
}

Result<void> EndstonePlayerInventory::setContents(std::vector<ItemStack const *> items)
{
    return EndstoneInventory::setContents(items);
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

std::unordered_map<int, std::unique_ptr<ItemStack>> EndstonePlayerInventory::all(const std::string &type) const
{
    return EndstoneInventory::all(type);
}

std::unordered_map<int, std::unique_ptr<ItemStack>> EndstonePlayerInventory::all(const ItemStack &item) const
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

std::unique_ptr<ItemStack> EndstonePlayerInventory::getHelmet() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getArmor(ArmorSlot::Head));
}

std::unique_ptr<ItemStack> EndstonePlayerInventory::getChestplate() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getArmor(ArmorSlot::Torso));
}

std::unique_ptr<ItemStack> EndstonePlayerInventory::getLeggings() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getArmor(ArmorSlot::Legs));
}

std::unique_ptr<ItemStack> EndstonePlayerInventory::getBoots() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getArmor(ArmorSlot::Feet));
}

void EndstonePlayerInventory::setHelmet(const ItemStack *helmet)
{
    holder_.setArmor(ArmorSlot::Head, EndstoneItemStack::toMinecraft(helmet));
}

void EndstonePlayerInventory::setChestplate(const ItemStack *chestplate)
{
    holder_.setArmor(ArmorSlot::Torso, EndstoneItemStack::toMinecraft(chestplate));
}

void EndstonePlayerInventory::setLeggings(const ItemStack *leggings)
{
    holder_.setArmor(ArmorSlot::Legs, EndstoneItemStack::toMinecraft(leggings));
}

void EndstonePlayerInventory::setBoots(const ItemStack *boots)
{
    holder_.setArmor(ArmorSlot::Feet, EndstoneItemStack::toMinecraft(boots));
}

std::unique_ptr<ItemStack> EndstonePlayerInventory::getItemInMainHand() const
{
    return getItem(holder_.getSelectedItemSlot());
}

void EndstonePlayerInventory::setItemInMainHand(const ItemStack *item)
{
    setItem(holder_.getSelectedItemSlot(), item);
}

std::unique_ptr<ItemStack> EndstonePlayerInventory::getItemInOffHand() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getOffhandSlot());
}

void EndstonePlayerInventory::setItemInOffHand(const ItemStack *item)
{
    holder_.setOffhandSlot(EndstoneItemStack::toMinecraft(item));
}

int EndstonePlayerInventory::getHeldItemSlot() const
{
    return holder_.getSelectedItemSlot();
}

Result<void> EndstonePlayerInventory::setHeldItemSlot(int slot)
{
    ENDSTONE_CHECKF(slot >= 0 && slot < FillingContainer::HOTBAR_SIZE, "Slot ({}) is not between 0 and {} inclusive",
                    slot, FillingContainer::HOTBAR_SIZE - 1)
    holder_.setSelectedSlot(slot);
    return {};
}

}  // namespace endstone::core
