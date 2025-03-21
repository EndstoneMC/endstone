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

#include "endstone/core/inventory/inventory.h"

#include "endstone/core/inventory/item_stack.h"

namespace endstone::core {

EndstoneInventory::EndstoneInventory(Container &container) : container_(container) {}

int EndstoneInventory::getSize() const
{
    return container_.getContainerSize();
}

int EndstoneInventory::getMaxStackSize() const
{
    return container_.getMaxStackSize();
}

std::shared_ptr<ItemStack> EndstoneInventory::getItem(int index) const
{
    return EndstoneItemStack::fromMinecraft(container_.getItem(index));
}

void EndstoneInventory::setItem(int index, std::shared_ptr<ItemStack> item)
{
    const auto item_stack = EndstoneItemStack::toMinecraft(item);
    container_.setItemWithForceBalance(index, item_stack, true);
}

void EndstoneInventory::addItem(ItemStack &item)
{
    auto item_stack = EndstoneItemStack::toMinecraft(item.shared_from_this());
    container_.addItemWithForceBalance(item_stack);
}

std::vector<std::shared_ptr<ItemStack>> EndstoneInventory::getContents() const
{
    const auto slots = container_.getSlots();
    std::vector<std::shared_ptr<ItemStack>> contents;
    for (const auto &slot : slots) {
        if (slot && !slot->isNull()) {
            contents.push_back(EndstoneItemStack::fromMinecraft(*slot));
        }
        else {
            contents.push_back(nullptr);
        }
    }
    return contents;
}

int EndstoneInventory::first(ItemStack &item)
{
    const auto item_stack = EndstoneItemStack::toMinecraft(item.shared_from_this());
    return container_.findFirstSlotForItem(item_stack);
}

bool EndstoneInventory::isEmpty() const
{
    return container_.isEmpty();
}

void EndstoneInventory::clear()
{
    container_.removeAllItems();
}

}  // namespace endstone::core
