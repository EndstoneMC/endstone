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

#include "endstone/util/result.h"

namespace endstone {
class ItemStack;
class ItemType;
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
     *
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
     * @brief Stores the given ItemStacks in the inventory.
     *
     * @note This will try to fill existing stacks and empty slots as well as it can.
     *
     * @note The returned map contains what it couldn't store, where the key is the index, and the value is the
     * ItemStack. If all items are stored, it will return an empty map.
     *
     * @param items The ItemStacks to add
     *
     * @return A map containing items that couldn't be added.
     */
    virtual std::unordered_map<int, const ItemStack *> addItem(std::vector<const ItemStack *> items) = 0;

    /**
     * @brief Removes the given ItemStacks from the inventory.
     *
     * @note It will try to remove 'as much as possible' from the types and amounts you give as arguments.
     *
     * @note The returned HashMap contains what it couldn't remove, where the key is the index, and the value is the
     * ItemStack. If all the given ItemStacks are removed, it will return an empty map.
     *
     * @param items The ItemStacks to remove
     *
     * @return A map containing items that couldn't be removed.
     */
    virtual std::unordered_map<int, const ItemStack *> removeItem(std::vector<const ItemStack *> items) = 0;

    /**
     * @brief Stores the given ItemStacks in the inventory.
     *
     * @note This will try to fill existing stacks and empty slots as well as it can.
     *
     * @note The returned map contains what it couldn't store, where the key is the index, and the value is the
     * ItemStack. If all items are stored, it will return an empty map.
     *
     * @param items The ItemStacks to add
     *
     * @return A map containing items that couldn't be added.
     */
    template <typename... Args, typename = std::enable_if_t<(std::is_convertible_v<Args, const ItemStack &> && ...)>>
    std::unordered_map<int, const ItemStack *> addItem(Args &&...items)
    {
        return addItem(std::vector<const ItemStack *>{&items...});
    }

    /**
     * @brief Removes the given ItemStacks from the inventory.
     *
     * @note It will try to remove 'as much as possible' from the types and amounts you give as arguments.
     *
     * @note The returned HashMap contains what it couldn't remove, where the key is the index, and the value is the
     * ItemStack. If all the given ItemStacks are removed, it will return an empty map.
     *
     * @param items The ItemStacks to remove
     * @return A map containing items that couldn't be removed.
     */
    template <typename... Args, typename = std::enable_if_t<(std::is_convertible_v<Args, const ItemStack &> && ...)>>
    std::unordered_map<int, const ItemStack *> removeItem(Args &&...items)
    {
        return removeItem(std::vector<const ItemStack *>{&items...});
    }

    /**
     * @brief Returns all ItemStacks from the inventory
     *
     * @return An array of ItemStacks from the inventory. Individual items may be null.
     */
    [[nodiscard]] virtual std::vector<std::unique_ptr<ItemStack>> getContents() const = 0;

    /**
     * @brief Completely replaces the inventory's contents. Removes all existing contents and replaces it with the
     * ItemStacks given in the array.
     *
     * @param items A complete replacement for the contents; the length must be less than or equal to getSize().
     */
    virtual Result<void> setContents(std::vector<const ItemStack *> items) = 0;

    /**
     * @brief Checks if the inventory contains any ItemStacks with the given ItemType.
     *
     * @param type The item type to check for
     *
     * @return true if an ItemStack is found with the given ItemType
     */
    [[nodiscard]] virtual Result<bool> contains(const std::string &type) const = 0;

    /**
     * @brief Checks if the inventory contains any ItemStacks with the given ItemStack.
     *
     * @note This will only return true if both the type and the amount of the stack match.
     *
     * @param item The ItemStack to match against
     * @return true if any exactly matching ItemStacks were found, false otherwise
     */
    [[nodiscard]] virtual bool contains(const ItemStack &item) const = 0;

    /**
     * @brief Checks if the inventory contains at least the minimum amount specified of exactly matching ItemStacks.
     *
     * @note An ItemStack only counts if both the type and the amount of the stack match.
     *
     * @param item the ItemStack to match against
     * @param amount how many identical stacks to check for
     *
     * @return true if amount less than 1 or if amount of exactly matching ItemStacks were found, false otherwise
     */
    [[nodiscard]] virtual bool contains(const ItemStack &item, int amount) const = 0;

    /**
     * @brief Checks if the inventory contains any ItemStacks with the given ItemType, adding to at least the minimum
     * amount specified.
     *
     * @param type The ItemType to check for
     * @param amount The minimum amount
     *
     * @return true if amount is less than 1, true if enough ItemStacks were found to add to the given amount
     */
    [[nodiscard]] virtual Result<bool> containsAtLeast(const std::string &type, int amount) const = 0;

    /**
     * @brief Checks if the inventory contains ItemStacks matching the given ItemStack whose amounts sum to at least the
     * minimum amount specified.
     *
     * @param item the ItemStack to match against
     * @param amount the minimum amount
     *
     * @return true if amount less than 1 or enough ItemStacks were found to add to the given amount, false otherwise
     */
    [[nodiscard]] virtual bool containsAtLeast(const ItemStack &item, int amount) const = 0;

    /**
     * @brief Finds all slots in the inventory containing any ItemStacks with the given ItemType.
     *
     * The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot.
     * If no matching ItemStack is found, an empty map is returned.
     *
     * @param type The ItemType to match against
     *
     * @return A map from slot indexes to item at index
     */
    [[nodiscard]] virtual Result<std::unordered_map<int, std::unique_ptr<ItemStack>>> all(
        const std::string &type) const = 0;

    /**
     * @brief Finds all slots in the inventory containing any ItemStacks with the given ItemStack.
     *
     * @note This will only match slots if both the type and the amount of the stack match
     *
     * The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot.
     * If no matching ItemStack is found, an empty map is returned.
     *
     * @param item The ItemStack to match against
     *
     * @return A map from slot indexes to item at index
     */
    [[nodiscard]] virtual std::unordered_map<int, std::unique_ptr<ItemStack>> all(const ItemStack &item) const = 0;

    /**
     * @brief Finds the first slot in the inventory containing an ItemStack with the given ItemType
     *
     * @param type The ItemType to look for
     *
     * @return The slot index of the given ItemType or -1 if not found
     */
    [[nodiscard]] virtual Result<int> first(const std::string &type) const = 0;

    /**
     * @brief Returns the first slot in the inventory containing an ItemStack with the given stack.
     *
     * @param item The ItemStack to match against
     *
     * @return The slot index of the given ItemStack or -1 if not found
     */
    [[nodiscard]] virtual int first(const ItemStack &item) const = 0;

    /**
     * @brief Returns the first empty Slot.
     *
     * @return The first empty Slot found, or -1 if no empty slots.
     */
    [[nodiscard]] virtual int firstEmpty() const = 0;

    /**
     * @brief Check whether this inventory is empty. An inventory is considered to be empty if there are no ItemStacks
     * in any slot of this inventory.
     *
     * @return true if empty, false otherwise
     */
    [[nodiscard]] virtual bool isEmpty() const = 0;

    /**
     * @brief Removes all stacks in the inventory matching the given ItemType.
     *
     * @param type The ItemType to remove
     */
    virtual Result<void> remove(const std::string &type) = 0;

    /**
     * @brief Removes all stacks in the inventory matching the given stack.
     *
     * @note This will only match a slot if both the type and the amount of the stack match
     *
     * @param item The ItemStack to match against
     */
    virtual void remove(const ItemStack &item) = 0;

    /**
     * @brief Clears out a particular slot in the index.
     *
     * @param index The index to empty.
     */
    virtual void clear(int index) = 0;

    /**
     * @brief Clears out the whole Inventory.
     */
    virtual void clear() = 0;
};
}  // namespace endstone
