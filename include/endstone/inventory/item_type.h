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

#include "endstone/detail/endstone.h"
#include "endstone/namespaced_key.h"
#include "endstone/registry.h"

namespace endstone {
/**
 * @brief Represents an item type.
 */
class ItemType {
public:
    virtual ~ItemType() = default;
    /**
     * @brief Return the identifier of this item type.
     *
     * @return this item's identifier
     */
    [[nodiscard]] virtual std::string_view getId() const = 0;

    /**
     * @brief Return the namespaced identifier of this item type.
     *
     * @return this item's key
     */
    [[nodiscard]] virtual NamespacedKey getKey() const = 0;

    /**
     * @brief Get the translation key, suitable for use in a translation component.
     *
     * @return the translation key
     */
    [[nodiscard]] virtual std::string_view getTranslationKey() const = 0;

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
     * @brief Attempts to get the ItemType with the given name.
     *
     * @note This is a normal lookup, names must be the precise name.
     *
     * @param name Name of the item_type to get
     *
     * @return ItemType if found, or nullptr
     */
    static const ItemType *get(std::string_view name)
    {
        auto key = NamespacedKey::fromString(name);
        if (!key) {
            return nullptr;
        }
        return Endstone::getServer().getItemRegistry().get(key.value());
    }
};
}  // namespace endstone
