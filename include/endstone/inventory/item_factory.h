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

#include "endstone/inventory/meta/item_meta.h"
#include "endstone/inventory/meta/map_meta.h"

namespace endstone {
class ItemFactory {
public:
    virtual ~ItemFactory() = default;

    /**
     * @brief This creates a new item meta for the material.
     *
     * @param type The item type to consider as base for the meta
     * @return a new ItemMeta that could be applied to an item stack of the specified type
     */
    [[nodiscard]] virtual std::unique_ptr<ItemMeta> getItemMeta(std::string_view type) const = 0;
    /**
     * @brief Returns an appropriate item meta for the specified stack.

     * @param meta the meta to convert
     * @param type the item type to convert the meta for
     * @return An appropriate item meta for the specified item stack. No guarantees are made as to if a copy is
     returned. This will be null for a stack of air.
     */
    [[nodiscard]] virtual std::unique_ptr<ItemMeta> asMetaFor(const ItemMeta *meta, std::string_view type) const = 0;
};
}  // namespace endstone
