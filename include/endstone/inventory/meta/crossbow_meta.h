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
 * @brief Represents the meta for a crossbow that can have charged projectiles.
 */
class CrossbowMeta : public ItemMeta {
public:
    ENDSTONE_ITEM_META_TYPE(CrossBow)

    /**
     * @brief Returns whether the item has any charged projectiles.
     *
     * @return whether charged projectiles are present
     */
    [[nodiscard]] virtual bool hasChargedProjectiles() const = 0;

    /**
     * @brief Returns an immutable list of the projectiles charged on this item.
     *
     * @return charged projectiles
     */
    [[nodiscard]] virtual std::vector<ItemStack> getChargedProjectiles() const = 0;

    /**
     * @brief Sets the projectiles charged on this item.
     *
     * @param projectiles the projectiles to set
     */
    virtual void setChargedProjectiles(std::vector<ItemStack> projectiles) = 0;

    /**
     * @brief Adds a charged projectile to this item.
     *
     * @param item projectile
     */
    virtual void addChargedProjectile(const ItemStack &item) = 0;
};
}  // namespace endstone
