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

#include "endstone/registry.h"

namespace endstone {
class ItemStack;
/**
 * @brief Represents an item type.
 */
using ItemTypeId = Identifier<ItemType>;
class ItemType : public Registry<ItemType>::Type {
public:
    static constexpr auto Air = ItemTypeId::minecraft("air");

    /**
     * @brief Get the translation key, suitable for use in a translation component.
     *
     * @return the translation key
     */
    [[nodiscard]] std::string getTranslationKey() const override = 0;

    /**
     * @brief Get the translation key, suitable for use in a translation component.
     *
     * @param data Data for this item type
     *
     * @return the translation key
     */
    [[nodiscard]] virtual std::string getTranslationKey(int data) const = 0;

    /**
     * @brief Gets the maximum amount of this item type that can be held in a stack
     *
     * @return Maximum stack size for this item type
     */
    [[nodiscard]] virtual int getMaxStackSize() const = 0;

    /**
     * @brief Gets the maximum durability of this item type
     *
     * @return Maximum durability for this item type
     */
    [[nodiscard]] virtual int getMaxDurability() const = 0;

    /**
     * @brief Constructs a new ItemStack with this item type that has the amount 1.
     *
     * @return the constructed item stack.
     */
    [[nodiscard]] virtual ItemStack createItemStack() const = 0;

    /**
     * @brief Constructs a new ItemStack with this item type.
     *
     * @param amount the amount in the stack
     * @return An ItemStack of this item type
     */
    [[nodiscard]] virtual ItemStack createItemStack(int amount) const = 0;
};
}  // namespace endstone
