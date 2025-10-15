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

std::unique_ptr<ItemStack> EndstoneInventory::getItem(int index) const
{
    return EndstoneItemStack::fromMinecraft(container_.getItem(index));
}

void EndstoneInventory::setItem(int index, const ItemStack *item)
{
    const auto item_stack = EndstoneItemStack::toMinecraft(item);
    container_.setItemWithForceBalance(index, item_stack, true);
}

std::unordered_map<int, ItemStack *> EndstoneInventory::addItem(std::vector<ItemStack *> items)
{
    std::unordered_map<int, ItemStack *> leftover;
    for (auto i = 0; i < items.size(); ++i) {
        auto *item = items[i];
        if (item == nullptr) {
            continue;
        }

        while (true) {
            auto slot = firstPartial(*item);

            if (slot == -1) {
                slot = firstEmpty();
                if (slot == -1) {
                    // No space at all!
                    leftover[i] = item;
                    break;
                }

                if (item->getAmount() > item->getMaxStackSize()) {
                    // More than a single stack!
                    auto stack = item->clone();
                    stack->setAmount(item->getMaxStackSize());
                    setItem(slot, stack.get());
                    item->setAmount(item->getAmount() - item->getMaxStackSize());
                }
                else {
                    // Just store it
                    setItem(slot, item);
                    break;
                }
            }
            else {
                auto partial = getItem(slot);
                if (item->getAmount() + partial->getAmount() <= partial->getMaxStackSize()) {
                    // Fully fits!
                    partial->setAmount(item->getAmount() + partial->getAmount());
                    setItem(slot, partial.get());
                    break;
                }

                // Fits partially
                item->setAmount(item->getAmount() + partial->getAmount() - partial->getMaxStackSize());
                partial->setAmount(partial->getMaxStackSize());
                setItem(slot, partial.get());
            }
        }
    }
    return leftover;
}

std::unordered_map<int, ItemStack *> EndstoneInventory::removeItem(std::vector<ItemStack *> items)
{
    std::unordered_map<int, ItemStack *> leftover;
    for (auto i = 0; i < items.size(); ++i) {
        auto *item = items[i];
        if (item == nullptr) {
            continue;
        }

        int to_delete = item->getAmount();
        while (true) {
            const auto slot = first(*item, false);

            // we don't have this type in the inventory
            if (slot == -1) {
                item->setAmount(to_delete);
                leftover[i] = item;
                break;
            }

            auto item_stack = getItem(slot);
            int amount = item_stack->getAmount();

            if (amount <= to_delete) {
                to_delete -= amount;
                clear(slot);  // clear the slot, all used up
            }
            else {
                item_stack->setAmount(amount - to_delete);
                setItem(slot, item_stack.get());
                to_delete = 0;
            }

            if (to_delete <= 0) {
                break;
            }
        }
    }
    return leftover;
}

std::vector<std::unique_ptr<ItemStack>> EndstoneInventory::getContents() const
{
    const auto slots = container_.getSlots();
    std::vector<std::unique_ptr<ItemStack>> contents;
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

Result<void> EndstoneInventory::setContents(std::vector<const ItemStack *> items)
{
    ENDSTONE_CHECKF(items.size() <= getSize(), "Invalid inventory size ({}); expected {} or less", items.size(),
                    getSize());
    for (auto i = 0; i < getSize(); i++) {
        if (i >= items.size()) {
            setItem(i, nullptr);
        }
        else {
            setItem(i, items[i]);
        }
    }
    return {};
}

bool EndstoneInventory::contains(const std::string &type) const
{
    for (const auto &item : getContents()) {
        if (item != nullptr && item->getType() == type) {
            return true;
        }
    }
    return false;
}

bool EndstoneInventory::contains(const ItemStack &item) const
{
    for (const auto &i : getContents()) {
        if (i != nullptr && item == *i) {
            return true;
        }
    }
    return false;
}

bool EndstoneInventory::contains(const ItemStack &item, int amount) const
{
    if (amount <= 0) {
        return true;
    }

    for (const auto &i : getContents()) {
        if (i != nullptr && item == *i && --amount <= 0) {
            return true;
        }
    }
    return false;
}

bool EndstoneInventory::containsAtLeast(const std::string &type, int amount) const
{
    if (amount <= 0) {
        return true;
    }

    for (const auto &item : getContents()) {
        if (item != nullptr && item->getType() == type) {
            if ((amount -= item->getAmount()) <= 0) {
                return true;
            }
        }
    }
    return false;
}

bool EndstoneInventory::containsAtLeast(const ItemStack &item, int amount) const
{
    if (amount <= 0) {
        return true;
    }

    for (const auto &i : getContents()) {
        if (i != nullptr && item.isSimilar(*i) && (amount -= i->getAmount()) <= 0) {
            return true;
        }
    }
    return false;
}

std::unordered_map<int, std::unique_ptr<ItemStack>> EndstoneInventory::all(const std::string &type) const
{
    std::unordered_map<int, std::unique_ptr<ItemStack>> slots;
    auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        auto &item = inventory[i];
        if (item != nullptr && item->getType() == type) {
            slots[i] = std::move(item);
        }
    }
    return slots;
}

std::unordered_map<int, std::unique_ptr<ItemStack>> EndstoneInventory::all(const ItemStack &item) const
{
    std::unordered_map<int, std::unique_ptr<ItemStack>> slots;
    auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        auto &it = inventory[i];
        if (it != nullptr && item == *it) {
            slots[i] = std::move(it);
        }
    }
    return slots;
}

int EndstoneInventory::first(const std::string &type) const
{
    const auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        const auto &item = inventory[i];
        if (item != nullptr && item->getType() == type) {
            return i;
        }
    }
    return -1;
}

int EndstoneInventory::first(const ItemStack &item) const
{
    return first(item, true);
}

int EndstoneInventory::firstEmpty() const
{
    const auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        const auto &item = inventory[i];
        if (item == nullptr) {
            return i;
        }
    }
    return -1;
}

bool EndstoneInventory::isEmpty() const
{
    return container_.isEmpty();
}

void EndstoneInventory::remove(const std::string &type)
{
    const auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        const auto &item = inventory[i];
        if (item != nullptr && item->getType() == type) {
            clear(i);
        }
    }
}

void EndstoneInventory::remove(const ItemStack &item)
{
    const auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        const auto &it = inventory[i];
        if (it != nullptr && item == *it) {
            clear(i);
        }
    }
}

void EndstoneInventory::clear(int index)
{
    setItem(index, nullptr);
}

void EndstoneInventory::clear()
{
    for (auto i = 0; i < getSize(); i++) {
        clear(i);
    }
}

int EndstoneInventory::first(const ItemStack &item, bool with_amount) const
{
    const auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        const auto &it = inventory[i];
        if (it == nullptr) {
            continue;
        }

        if (with_amount ? item == *it : item.isSimilar(*it)) {
            return i;
        }
    }
    return -1;
}

int EndstoneInventory::firstPartial(const ItemStack &item) const
{
    const auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        const auto &it = inventory[i];
        if (it != nullptr && it->getAmount() < item.getMaxStackSize() && item.isSimilar(*it)) {
            return i;
        }
    }
    return -1;
}

}  // namespace endstone::core
