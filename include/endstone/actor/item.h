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

#include "endstone/actor/actor.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {
/**
 * @brief Represents a base actor in the level.
 */
class Item : virtual Actor {
public:
    /**
     * @brief Gets the item stack associated with this item drop.
     *
     * @return An item stack.
     */
    [[nodiscard]] virtual std::unique_ptr<ItemStack> getItemStack() const = 0;

    /**
     * @brief Sets the item stack associated with this item drop.
     *
     * @param stack An item stack.
     */
    virtual void setItemStack(const ItemStack &stack) = 0;

    /**
     * @brief Gets the delay before this Item is available to be picked up by players
     *
     * @return Remaining delay
     */
    [[nodiscard]] virtual int getPickupDelay() const = 0;

    /**
     * @brief Sets the delay before this Item is available to be picked up by players
     *
     * @param delay New delay
     */
    virtual void setPickupDelay(int delay) = 0;

    /**
     * @brief Sets if this Item should live forever
     *
     * @param unlimited true if the lifetime is unlimited
     */
    virtual void setUnlimitedLifetime(bool unlimited) = 0;

    /**
     * @brief Gets if this Item lives forever
     *
     * @return true if the lifetime is unlimited
     */
    virtual bool isUnlimitedLifetime() const = 0;

    /**
     * @brief Sets the owner of this item.
     *
     * Other entities will not be able to pickup this item when an owner is set.
     *
     * @param owner UUID of new owner
     */
    virtual void setOwner(UUID owner) = 0;

    /**
     * @brief Get the owner of this item.
     *
     * @return UUID of owner
     */
    virtual UUID getOwner() const = 0;

    /**
     * @brief Set the thrower of this item.
     *
     * The thrower is the entity which dropped the item. This affects the
     * trigger criteria for item pickups, for things such as advancements.
     *
     * @param uuid UUID of thrower
     */
    virtual void setThrower(UUID uuid) = 0;

    /**
     * @brief Get the thrower of this item.
     *
     * The thrower is the entity which dropped the item.
     *
     * @return UUID of thrower
     */
    virtual UUID getThrower() const = 0;
};

}  // namespace endstone
