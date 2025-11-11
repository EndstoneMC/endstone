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

#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "bedrock/world/item/item_stack.h"
#include "endstone/inventory/item_type.h"
#include "endstone/inventory/meta/item_meta.h"

namespace endstone::core {
class EndstoneItemMetas {
public:
    class ItemMetaDetails {
    public:
        template <typename T>
        static ItemMetaDetails create()
        {
            ItemMetaDetails details;
            if constexpr (std::is_same_v<T, void>) {
                details.from_item_stack_ = [](const ::ItemStackBase &) -> std::unique_ptr<ItemMeta> { return nullptr; };
                details.from_item_meta_ = [](ItemId, const ItemMeta *) -> std::unique_ptr<ItemMeta> { return nullptr; };
            }
            else {
                details.from_item_stack_ = [](const ::ItemStackBase &stack) -> std::unique_ptr<ItemMeta> {
                    static auto empty_tag = std::make_unique<CompoundTag>();
                    const auto *tag = stack.hasUserData() ? stack.getUserData() : empty_tag.get();
                    return std::make_unique<T>(*tag);
                };
                details.from_item_meta_ = [](ItemId, const ItemMeta *meta) -> std::unique_ptr<ItemMeta> {
                    return std::make_unique<T>(meta);
                };
            }
            return details;
        }

        [[nodiscard]] std::unique_ptr<ItemMeta> fromItemStack(const ::ItemStackBase &item) const
        {
            return from_item_stack_(item);
        }

        [[nodiscard]] std::unique_ptr<ItemMeta> fromItemMeta(const ItemId &type, const ItemMeta *meta) const
        {
            return from_item_meta_(type, meta);
        }

    private:
        std::function<std::unique_ptr<ItemMeta>(const ::ItemStackBase &)> from_item_stack_;
        std::function<std::unique_ptr<ItemMeta>(ItemId, const ItemMeta *)> from_item_meta_;
    };

    static ItemMetaDetails &getItemMetaDetails(const ItemId &type)
    {
        static std::unordered_map<std::string, ItemMetaDetails *> look_up = {
            {"minecraft:air", &Empty},
            {"minecraft:filled_map", &Map},
        };
        if (const auto it = look_up.find(type); it != look_up.end()) {
            return *it->second;
        }
        return Item;
    }

private:
    static ItemMetaDetails Empty;
    static ItemMetaDetails Item;
    static ItemMetaDetails Map;
};
}  // namespace endstone::core
