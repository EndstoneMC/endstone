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

std::shared_ptr<ItemStack> EndstonePlayerInventory::getItem(int index) const
{
    return EndstoneInventory::getItem(index);
}

void EndstonePlayerInventory::setItem(int index, std::shared_ptr<ItemStack> item)
{
    EndstoneInventory::setItem(index, item);
    holder_.sendInventory(false);
}

void EndstonePlayerInventory::addItem(ItemStack &item)
{
    EndstoneInventory::addItem(item);
    holder_.sendInventory(false);
}

std::vector<std::shared_ptr<ItemStack>> EndstonePlayerInventory::getContents() const
{
    return EndstoneInventory::getContents();
}

int EndstonePlayerInventory::first(ItemStack &item)
{
    return EndstoneInventory::first(item);
}

bool EndstonePlayerInventory::isEmpty() const
{
    return EndstoneInventory::isEmpty();
}

void EndstonePlayerInventory::clear()
{
    EndstoneInventory::clear();
    holder_.sendInventory(false);
}

std::shared_ptr<ItemStack> EndstonePlayerInventory::getHelmet() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getArmor(ArmorSlot::Head));
}

std::shared_ptr<ItemStack> EndstonePlayerInventory::getChestplate() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getArmor(ArmorSlot::Torso));
}

std::shared_ptr<ItemStack> EndstonePlayerInventory::getLeggings() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getArmor(ArmorSlot::Legs));
}

std::shared_ptr<ItemStack> EndstonePlayerInventory::getBoots() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getArmor(ArmorSlot::Feet));
}

std::shared_ptr<ItemStack> EndstonePlayerInventory::getItemInMainHand() const
{
    return getItem(holder_.getSelectedItemSlot());
}

std::shared_ptr<ItemStack> EndstonePlayerInventory::getItemInOffHand() const
{
    return EndstoneItemStack::fromMinecraft(holder_.getOffhandSlot());
}

int EndstonePlayerInventory::getHeldItemSlot() const
{
    return holder_.getSelectedItemSlot();
}

void EndstonePlayerInventory::setHeldItemSlot(int slot)
{
    // TODO: Preconditions.checkArgument(slot >= 0 && slot < PlayerInventory.getSelectionSize(), "Slot (%s) is not
    //  between 0 and %s inclusive", slot, PlayerInventory.getSelectionSize() - 1);
    holder_.setSelectedSlot(slot);
}

}  // namespace endstone::core
