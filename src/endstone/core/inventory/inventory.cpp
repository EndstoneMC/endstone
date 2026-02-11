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

std::optional<ItemStack> EndstoneInventory::getItem(int index) const
{
    auto item = container_.getItem(index);
    if (item.isNull()) {
        return std::nullopt;
    }
    return EndstoneItemStack::fromMinecraft(item);
}

void EndstoneInventory::setItem(int index, std::optional<ItemStack> item)
{
    const auto item_stack = item.has_value() ? EndstoneItemStack::toMinecraft(item.value()) : ::ItemStack::EMPTY_ITEM;
    container_.setItemWithForceBalance(index, item_stack, true);
}

std::unordered_map<int, ItemStack> EndstoneInventory::addItem(std::vector<ItemStack> items)
{
    std::unordered_map<int, ItemStack> leftover;
    for (auto i = 0; i < items.size(); ++i) {
        auto &item = items[i];
        while (true) {
            auto slot = firstPartial(item);

            if (slot == -1) {
                slot = firstEmpty();
                if (slot == -1) {
                    // No space at all!
                    leftover.emplace(i, item);
                    break;
                }

                if (item.getAmount() > item.getMaxStackSize()) {
                    // More than a single stack!
                    auto stack = item;
                    stack.setAmount(item.getMaxStackSize());
                    setItem(slot, stack);
                    item.setAmount(item.getAmount() - item.getMaxStackSize());
                }
                else {
                    // Just store it
                    setItem(slot, item);
                    break;
                }
            }
            else {
                auto partial = *getItem(slot);
                if (item.getAmount() + partial.getAmount() <= partial.getMaxStackSize()) {
                    // Fully fits!
                    partial.setAmount(item.getAmount() + partial.getAmount());
                    setItem(slot, partial);
                    break;
                }

                // Fits partially
                item.setAmount(item.getAmount() + partial.getAmount() - partial.getMaxStackSize());
                partial.setAmount(partial.getMaxStackSize());
                setItem(slot, partial);
            }
        }
    }
    return leftover;
}

std::unordered_map<int, ItemStack> EndstoneInventory::removeItem(std::vector<ItemStack> items)
{
    std::unordered_map<int, ItemStack> leftover;
    for (auto i = 0; i < items.size(); ++i) {
        auto &item = items[i];
        int to_delete = item.getAmount();
        while (true) {
            const auto slot = first(item, false);

            // we don't have this type in the inventory
            if (slot == -1) {
                item.setAmount(to_delete);
                leftover.emplace(i, item);
                break;
            }

            auto item_stack = *getItem(slot);
            int amount = item_stack.getAmount();

            if (amount <= to_delete) {
                to_delete -= amount;
                clear(slot);  // clear the slot, all used up
            }
            else {
                item_stack.setAmount(amount - to_delete);
                setItem(slot, item_stack);
                to_delete = 0;
            }

            if (to_delete <= 0) {
                break;
            }
        }
    }
    return leftover;
}

std::vector<std::optional<ItemStack>> EndstoneInventory::getContents() const
{
    const auto slots = container_.getSlots();
    std::vector<std::optional<ItemStack>> contents;
    for (const auto &slot : slots) {
        if (slot->isNull()) {
            contents.emplace_back(std::nullopt);
        }
        else {
            contents.emplace_back(EndstoneItemStack::fromMinecraft(*slot));
        }
    }
    return contents;
}

void EndstoneInventory::setContents(std::vector<std::optional<ItemStack>> items)
{
    Preconditions::checkArgument(items.size() <= getSize(), "Invalid inventory size ({}); expected {} or less",
                                 items.size(), getSize());
    for (auto i = 0; i < getSize(); i++) {
        if (i >= items.size()) {
            clear(i);
        }
        else {
            setItem(i, std::move(items[i]));
        }
    }
}

bool EndstoneInventory::contains(const std::string &type) const
{
    for (const auto &item : getContents()) {
        if (item.has_value() && item->getType() == type) {
            return true;
        }
    }
    return false;
}

bool EndstoneInventory::contains(const ItemStack &item) const
{
    for (const auto &i : getContents()) {
        if (i.has_value() && item == i.value()) {
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
        if (i.has_value() && item == i.value() && --amount <= 0) {
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
        if (item.has_value() && item->getType() == type) {
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
        if (i.has_value() && item.isSimilar(i.value()) && (amount -= i->getAmount()) <= 0) {
            return true;
        }
    }
    return false;
}

std::unordered_map<int, ItemStack> EndstoneInventory::all(const std::string &type) const
{
    std::unordered_map<int, ItemStack> slots;
    auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        auto &item = inventory[i];
        if (item.has_value() && item->getType() == type) {
            slots[i] = std::move(item.value());
        }
    }
    return slots;
}

std::unordered_map<int, ItemStack> EndstoneInventory::all(const ItemStack &item) const
{
    std::unordered_map<int, ItemStack> slots;
    auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        auto &it = inventory[i];
        if (it.has_value() && item == it.value()) {
            slots[i] = std::move(it.value());
        }
    }
    return slots;
}

int EndstoneInventory::first(const std::string &type) const
{
    const auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        const auto &item = inventory[i];
        if (item.has_value() && item->getType() == type) {
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
        if (!inventory[i].has_value()) {
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
        if (item.has_value() && item->getType() == type) {
            clear(i);
        }
    }
}

void EndstoneInventory::remove(const ItemStack &item)
{
    const auto inventory = getContents();
    for (auto i = 0; i < inventory.size(); i++) {
        const auto &it = inventory[i];
        if (it.has_value() && item == it.value()) {
            clear(i);
        }
    }
}

void EndstoneInventory::clear(int index)
{
    setItem(index, std::nullopt);
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
        if (!it.has_value()) {
            continue;
        }

        if (with_amount ? item == it.value() : item.isSimilar(it.value())) {
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
        if (it.has_value() && it->getAmount() < item.getMaxStackSize() && item.isSimilar(it.value())) {
            return i;
        }
    }
    return -1;
}

}  // namespace endstone::core
