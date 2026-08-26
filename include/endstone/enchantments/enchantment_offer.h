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

#include <unordered_map>
#include <utility>

#include "endstone/check.h"
#include "endstone/enchantments/enchantment.h"

namespace endstone {

/**
 * Represents an offer made by an enchanting table.
 *
 * Bedrock stores every enchantment that selecting an offer applies rather than a single hinted enchantment.
 * Enchantments in an offer must be entries obtained from Endstone's Enchantment registry.
 */
class EnchantmentOffer {
public:
    using Enchantments = std::unordered_map<const Enchantment *, int>;

    /**
     * Creates an enchanting offer.
     *
     * @param enchants the enchantments and levels applied by the offer
     * @param cost the minimum player level required to select the offer
     */
    EnchantmentOffer(Enchantments enchants, int cost) : enchants_(std::move(enchants)), cost_(cost)
    {
        validateEnchants(enchants_);
        Preconditions::checkArgument(cost > 0, "cost must be greater than 0");
    }

    /**
     * Gets the enchantments applied by this offer.
     *
     * @return the enchantments and their levels
     */
    [[nodiscard]] const Enchantments &getEnchants() const { return enchants_; }

    /**
     * Sets the enchantments applied by this offer.
     *
     * @param enchants the enchantments and their levels
     */
    void setEnchants(Enchantments enchants)
    {
        validateEnchants(enchants);
        enchants_ = std::move(enchants);
    }

    /**
     * Gets the minimum player level required to select this offer.
     *
     * @return the minimum required player level
     */
    [[nodiscard]] int getCost() const { return cost_; }

    /**
     * Sets the minimum player level required to select this offer.
     *
     * @param cost the minimum required player level
     */
    void setCost(int cost)
    {
        Preconditions::checkArgument(cost > 0, "cost must be greater than 0");
        cost_ = cost;
    }

private:
    static void validateEnchants(const Enchantments &enchants)
    {
        Preconditions::checkArgument(!enchants.empty(), "enchants must not be empty");
        for (const auto &[enchantment, level] : enchants) {
            Preconditions::checkArgument(enchantment != nullptr, "enchantment must not be null");
            Preconditions::checkArgument(level > 0, "enchantment level must be greater than 0");
        }
    }

    Enchantments enchants_;
    int cost_;
};

}  // namespace endstone
