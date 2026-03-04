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

#include <optional>

#include "endstone/inventory/meta/item_meta.h"

namespace endstone {
/**
 * @brief Represents the meta for a crossbow that can have a charged projectile.
 */
class CrossbowMeta : public ItemMeta {
public:
    ENDSTONE_ITEM_META_TYPE(CrossBow)

    /**
     * @brief Returns whether the crossbow has a charged projectile.
     *
     * @return whether a charged projectile is present
     */
    [[nodiscard]] virtual bool hasChargedProjectile() const = 0;

    /**
     * @brief Returns the charged projectile, or std::nullopt if none.
     *
     * @return the charged projectile
     */
    [[nodiscard]] virtual std::optional<ItemStack> getChargedProjectile() const = 0;

    /**
     * @brief Sets the charged projectile. Pass std::nullopt to clear.
     *
     * @param projectile the projectile to set, or std::nullopt to clear
     */
    virtual void setChargedProjectile(std::optional<ItemStack> projectile) = 0;
};
}  // namespace endstone
