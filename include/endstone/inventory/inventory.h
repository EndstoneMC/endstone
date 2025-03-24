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

#include <memory>
#include <vector>

#include "endstone/inventory/item_stack.h"

namespace endstone {
/**
 * @brief Interface to the various inventories.
 */
class Inventory {
public:
    virtual ~Inventory() = default;
    /**
     * @brief Returns the size of the inventory
     *
     * @return The size of the inventory
     */
    [[nodiscard]] virtual int getSize() const = 0;

    /**
     * @brief Returns the maximum stack size for an ItemStack in this inventory.
     *
     * @return The maximum size for an ItemStack in this inventory.
     */
    [[nodiscard]] virtual int getMaxStackSize() const = 0;

    /**
     * @brief Returns the ItemStack found in the slot at the given index
     *
     * @param index The index of the Slot's ItemStack to return
     * @return The ItemStack in the slot
     */
    [[nodiscard]] virtual std::unique_ptr<ItemStack> getItem(int index) const = 0;

    /**
     * @brief Stores the ItemStack at the given index of the inventory.
     *
     * @param index The index where to put the ItemStack
     * @param item The ItemStack to set
     */
    virtual void setItem(int index, const ItemStack *item) = 0;

    /**
     * @brief Stores the given ItemStacks in the inventory. This will try to fill
     * existing stacks and empty slots as well as it can.
     *
     * @param item The ItemStack to add
     */
    virtual void addItem(const ItemStack &item) = 0;

    /**
     * @brief Returns all ItemStacks from the inventory
     *
     * @return An array of ItemStacks from the inventory. Individual items may be null.
     */
    [[nodiscard]] virtual std::vector<std::unique_ptr<ItemStack>> getContents() const = 0;

    /**
     * @brief Returns the first slot in the inventory containing an ItemStack with the given stack.
     *
     * @param item The ItemStack to match against
     * @return The slot index of the given ItemStack or -1 if not found
     */
    [[nodiscard]] virtual int first(const ItemStack &item) const = 0;

    /**
     * @brief Check whether this inventory is empty. An inventory is considered
     * to be empty if there are no ItemStacks in any slot of this inventory.
     *
     * @return true if empty, false otherwise
     */
    [[nodiscard]] virtual bool isEmpty() const = 0;

    /**
     * @brief Clears out the whole Inventory.
     */
    virtual void clear() = 0;
};
}  // namespace endstone
