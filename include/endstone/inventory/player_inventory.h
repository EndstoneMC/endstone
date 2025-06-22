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

#include "endstone/inventory/inventory.h"

namespace endstone {

/**
 * @brief Interface to the inventory of a Player, including the four armor slots and any extra slots.
 */
class PlayerInventory : public Inventory {
public:
    /**
     * @brief Return the ItemStack from the helmet slot
     *
     * @return The ItemStack in the helmet slot
     */
    [[nodiscard]] virtual std::unique_ptr<ItemStack> getHelmet() const = 0;

    /**
     * @brief Return the ItemStack from the chestplate slot
     *
     * @return The ItemStack in the chestplate slot
     */
    [[nodiscard]] virtual std::unique_ptr<ItemStack> getChestplate() const = 0;

    /**
     * @brief Return the ItemStack from the leg slot
     *
     * @return The ItemStack in the leg slot
     */
    [[nodiscard]] virtual std::unique_ptr<ItemStack> getLeggings() const = 0;

    /**
     * @brief Return the ItemStack from the boots slot
     *
     * @return The ItemStack in the boots slot
     */
    [[nodiscard]] virtual std::unique_ptr<ItemStack> getBoots() const = 0;

    /**
     * @brief Put the given ItemStack into the helmet slot.
     *
     * This does not check if the ItemStack is a helmet.
     *
     * @param helmet The ItemStack to use as helmet
     */
    virtual void setHelmet(const ItemStack *helmet) = 0;

    /**
     * @brief Put the given ItemStack into the chestplate slot.
     *
     * This does not check if the ItemStack is a chestplate.
     *
     * @param chestplate The ItemStack to use as chestplate
     */
    virtual void setChestplate(const ItemStack *chestplate) = 0;

    /**
     * @brief Put the given ItemStack into the leggings slot.
     *
     * This does not check if the ItemStack is a pair of leggings.
     *
     * @param leggings The ItemStack to use as leggings
     */
    virtual void setLeggings(const ItemStack *leggings) = 0;

    /**
     * @brief Put the given ItemStack into the boots slot.
     *
     * This does not check if the ItemStack is a pair of boots.
     *
     * @param boots The ItemStack to use as boots
     */
    virtual void setBoots(const ItemStack *boots) = 0;

    /**
     * @brief Gets the item the player is currently holding in their main hand.
     *
     * @return the currently held item
     */
    [[nodiscard]] virtual std::unique_ptr<ItemStack> getItemInMainHand() const = 0;

    /**
     * @brief Sets the item the player is holding in their main hand.
     *
     * @param item The item to put into the player's hand
     */
    virtual void setItemInMainHand(const ItemStack *item) = 0;

    /**
     * @brief Gets the item the player is currently holding in their off hand.
     *
     * @return the currently held item
     */
    [[nodiscard]] virtual std::unique_ptr<ItemStack> getItemInOffHand() const = 0;

    /**
     * @brief Sets the item the player is holding in their off hand.
     *
     * @param item The item to put into the player's hand
     */
    virtual void setItemInOffHand(const ItemStack *item) = 0;

    /**
     * @brief Get the slot number of the currently held item
     *
     * @return Held item slot number
     */
    [[nodiscard]] virtual int getHeldItemSlot() const = 0;

    /**
     * @brief Set the slot number of the currently held item.
     *
     * This validates whether the slot is between 0 and 8 inclusive.
     *
     * @param slot The new slot number
     */
    virtual Result<void> setHeldItemSlot(int slot) = 0;
};

}  // namespace endstone
