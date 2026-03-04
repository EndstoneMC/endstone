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

#pragma once

#include "bedrock/world/container.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/inventory/inventory.h"

namespace endstone::core {

template <typename Interface>
    requires std::is_base_of_v<Inventory, Interface>
class EndstoneInventoryBase : public Interface {
public:
    explicit EndstoneInventoryBase(::Container &container) : container_(container) {}

    [[nodiscard]] int getSize() const override
    {
        return container_.getContainerSize();
    }

    [[nodiscard]] int getMaxStackSize() const override
    {
        return container_.getMaxStackSize();
    }

    [[nodiscard]] std::optional<ItemStack> getItem(int index) const override
    {
        const auto &item = container_.getItem(index);
        if (item.isNull()) {
            return std::nullopt;
        }
        return EndstoneItemStack::fromMinecraft(item);
    }

    void setItem(int index, std::optional<ItemStack> item) override
    {
        const auto item_stack =
            item.has_value() ? EndstoneItemStack::toMinecraft(item.value()) : ::ItemStack::EMPTY_ITEM;
        container_.setItemWithForceBalance(index, item_stack, true);
    }

    std::unordered_map<int, ItemStack> addItem(std::vector<ItemStack> items) override
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

    std::unordered_map<int, ItemStack> removeItem(std::vector<ItemStack> items) override
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

    [[nodiscard]] std::vector<std::optional<ItemStack>> getContents() const override
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

    void setContents(std::vector<std::optional<ItemStack>> items) override
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

    [[nodiscard]] bool contains(const std::string &type) const override
    {
        for (const auto &item : getContents()) {
            if (item.has_value() && item->getType() == type) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool contains(const ItemStack &item) const override
    {
        for (const auto &i : getContents()) {
            if (i.has_value() && item == i.value()) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool contains(const ItemStack &item, int amount) const override
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

    [[nodiscard]] bool containsAtLeast(const std::string &type, int amount) const override
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

    [[nodiscard]] bool containsAtLeast(const ItemStack &item, int amount) const override
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

    [[nodiscard]] std::unordered_map<int, ItemStack> all(const std::string &type) const override
    {
        std::unordered_map<int, ItemStack> slots;
        auto inventory = getContents();
        for (auto i = 0; i < inventory.size(); i++) {
            auto &item = inventory[i];
            if (item.has_value() && item->getType() == type) {
                slots.emplace(i, std::move(item.value()));
            }
        }
        return slots;
    }

    [[nodiscard]] std::unordered_map<int, ItemStack> all(const ItemStack &item) const override
    {
        std::unordered_map<int, ItemStack> slots;
        auto inventory = getContents();
        for (auto i = 0; i < inventory.size(); i++) {
            auto &it = inventory[i];
            if (it.has_value() && item == it.value()) {
                slots.emplace(i, std::move(it.value()));
            }
        }
        return slots;
    }

    [[nodiscard]] int first(const std::string &type) const override
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

    [[nodiscard]] int first(const ItemStack &item) const override
    {
        return first(item, true);
    }

    [[nodiscard]] int firstEmpty() const override
    {
        const auto inventory = getContents();
        for (auto i = 0; i < inventory.size(); i++) {
            if (!inventory[i].has_value()) {
                return i;
            }
        }
        return -1;
    }

    [[nodiscard]] bool isEmpty() const override
    {
        return container_.isEmpty();
    }

    void remove(const std::string &type) override
    {
        const auto inventory = getContents();
        for (auto i = 0; i < inventory.size(); i++) {
            const auto &item = inventory[i];
            if (item.has_value() && item->getType() == type) {
                clear(i);
            }
        }
    }

    void remove(const ItemStack &item) override
    {
        const auto inventory = getContents();
        for (auto i = 0; i < inventory.size(); i++) {
            const auto &it = inventory[i];
            if (it.has_value() && item == it.value()) {
                clear(i);
            }
        }
    }

    void clear(int index) override
    {
        setItem(index, std::nullopt);
    }

    void clear() override
    {
        for (auto i = 0; i < getSize(); i++) {
            clear(i);
        }
    }

protected:
    ::Container &container_;

private:
    [[nodiscard]] int first(const ItemStack &item, bool with_amount) const
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

    [[nodiscard]] int firstPartial(const ItemStack &item) const
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
};

using EndstoneInventory = EndstoneInventoryBase<Inventory>;

}  // namespace endstone::core
