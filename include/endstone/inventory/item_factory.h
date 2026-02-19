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

#include "endstone/inventory/meta/item_meta.h"
#include "item_type.h"

namespace endstone {
class ItemFactory {
public:
    virtual ~ItemFactory() = default;

    /**
     * @brief This creates a new item meta for the item type.
     *
     * @param type The item type to consider as base for the meta
     * @return a new ItemMeta that could be applied to an item stack of the specified item type
     */
    [[nodiscard]] virtual std::unique_ptr<ItemMeta> getItemMeta(ItemTypeId type) const = 0;

    /**
     * @brief This method checks the item meta to confirm that it is applicable (no data lost if applied) to the
     * specified ItemStack.
     *
     * @param meta Meta to check
     * @param type The item type that meta will be applied to
     * @return true if the meta can be applied without losing data, false otherwise
     */
    [[nodiscard]] virtual bool isApplicable(const ItemMeta *meta, ItemTypeId type) const = 0;

    /**
     * @brief This method is used to compare two ItemMeta objects.
     *
     * @param meta1 First meta to compare, and may be null to indicate no data
     * @param meta2 Second meta to compare, and may be null to indicate no data
     * @return false if one of the meta has data the other does not, otherwise true
     */
    [[nodiscard]] virtual bool equals(const ItemMeta *meta1, const ItemMeta *meta2) const = 0;

    /**
     * @brief Returns an appropriate item meta for the specified item type.
     *
     * The item meta returned will always be a valid meta for a given ItemStack of the specified item type. It may be a
     * more or less specific meta, and could also be the same meta or meta type as the parameter. The item meta returned
     * will also always be the most appropriate meta.
     *
     * @param meta the meta to convert
     * @param type the item type to convert the meta for
     * @return An appropriate item meta for the specified item type.
     */
    [[nodiscard]] virtual std::unique_ptr<ItemMeta> asMetaFor(const ItemMeta *meta, ItemTypeId type) const = 0;
};
}  // namespace endstone
