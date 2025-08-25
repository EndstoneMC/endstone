// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

namespace endstone {
class ItemStack;

class Enchantment {
public:
    virtual ~Enchantment() = default;

    /**
     * @brief Return the identifier for this enchantment.
     *
     * @return this enchantment's id
     */
    [[nodiscard]] virtual std::string getId() const = 0;

    /**
     * @brief Get the translation key, suitable for use in a translation component.
     *
     * @return the translation key
     */
    [[nodiscard]] virtual std::string getTranslationKey() const = 0;

    /**
     * @brief Gets the maximum level that this Enchantment may become.
     *
     * @return Maximum level of the Enchantment
     */
    [[nodiscard]] virtual int getMaxLevel() const = 0;

    /**
     * @brief Gets the level that this Enchantment should start at (also known as minimum level).
     *
     * @return Starting level of the Enchantment
     */
    [[nodiscard]] virtual int getStartLevel() const = 0;

    /**
     * @brief Check if this enchantment conflicts with another enchantment.
     *
     * @param other The enchantment to check against
     * @return True if there is a conflict.
     */
    [[nodiscard]] virtual bool conflictsWith(const Enchantment &other) const = 0;

    /**
     * @brief Checks if this Enchantment may be applied to the given ItemStack.
     *
     * This does not check if it conflicts with any enchantments already applied to the item.
     *
     * @param item Item to test
     * @return True if the enchantment may be applied, otherwise False
     */
    [[nodiscard]] virtual bool canEnchantItem(const ItemStack &item) const = 0;
};
}  // namespace endstone
