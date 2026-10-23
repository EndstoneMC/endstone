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

#include <vector>

#include "endstone/inventory/meta/item_meta.h"

namespace endstone {
/**
 * Represents the meta for a crossbow that can have charged projectiles.
 *
 * A Bedrock crossbow holds at most one charged projectile.
 */
class CrossbowMeta : public ItemMeta {
public:
    /**
     * Returns whether the item has any charged projectiles.
     *
     * @return whether charged projectiles are present
     */
    [[nodiscard]] virtual bool hasChargedProjectiles() const = 0;

    /**
     * Returns a list of the projectiles charged on this item.
     *
     * The list holds at most one projectile.
     *
     * @return charged projectiles
     */
    [[nodiscard]] virtual std::vector<ItemStack> getChargedProjectiles() const = 0;

    /**
     * Sets the projectiles charged on this item.
     *
     * Removes all projectiles when given an empty list.
     *
     * @param projectiles the projectiles to set
     * @throws std::invalid_argument if one of the projectiles is empty, or if more than one projectile is given
     */
    virtual void setChargedProjectiles(std::vector<ItemStack> projectiles) = 0;

    /**
     * Adds a charged projectile to this item.
     *
     * @param item projectile
     * @throws std::invalid_argument if the projectile is empty, or if the item already has a charged projectile
     */
    virtual void addChargedProjectile(const ItemStack &item) = 0;
};
}  // namespace endstone
