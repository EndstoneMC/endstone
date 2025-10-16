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
#include "endstone/inventory/meta/item_meta.h"

namespace endstone::core {
class EndstoneItemMetas {
public:
    template <typename T>
    static bool applicableTo(const std::string &type);
    template <typename T>
    static void applyToItem(const T &self, ::CompoundTag &tag);
    template <typename T>
    static bool equalsCommon(const T &self, const ItemMeta &other);
    template <typename T>
    static bool notUncommon(const T &self, const ItemMeta &other);
    template <typename T>
    static void loadFrom(T &self, const ::ItemStackBase &item);

    class ItemMetaDetails {
    public:
        template <typename T>
        static ItemMetaDetails create()
        {
            ItemMetaDetails details;
            if constexpr (std::is_same_v<T, void>) {
                details.from_item_stack_ = [](const ::ItemStackBase &) -> std::unique_ptr<ItemMeta> { return nullptr; };
                details.from_item_meta_ = [](const std::string &, const ItemMeta *) -> std::unique_ptr<ItemMeta> {
                    return nullptr;
                };
            }
            else {
                details.from_item_stack_ = [](const ::ItemStackBase &stack) -> std::unique_ptr<ItemMeta> {
                    auto meta = std::make_unique<T>(nullptr);
                    loadFrom(*meta, stack);
                    return meta;
                };
                details.from_item_meta_ = [](const std::string &, const ItemMeta *meta) -> std::unique_ptr<ItemMeta> {
                    return std::make_unique<T>(meta);
                };
                details.applicable_to_ = [](const std::string &type) -> bool {
                    return EndstoneItemMetas::applicableTo<T>(type);
                };
                details.apply_to_item_ = [](const ItemMeta &self, ::CompoundTag &tag) -> void {
                    EndstoneItemMetas::applyToItem<T>(static_cast<const T &>(self), tag);
                };
                details.equals_common_ = [](const ItemMeta &self, const ItemMeta &other) -> bool {
                    return EndstoneItemMetas::equalsCommon<T>(static_cast<const T &>(self), other);
                };
                details.not_uncommon_ = [](const ItemMeta &self, const ItemMeta &other) -> bool {
                    return EndstoneItemMetas::notUncommon<T>(static_cast<const T &>(self), other);
                };
            }
            return details;
        }

        [[nodiscard]] std::unique_ptr<ItemMeta> fromItemStack(const ::ItemStackBase &item) const
        {
            return from_item_stack_(item);
        }

        [[nodiscard]] std::unique_ptr<ItemMeta> fromItemMeta(const std::string &type, const ItemMeta *meta) const
        {
            return from_item_meta_(type, meta);
        }

        [[nodiscard]] bool applicableTo(const std::string &type) const
        {
            return applicable_to_(type);
        }

        void applyToItem(const ItemMeta &meta, ::CompoundTag &tag) const
        {
            apply_to_item_(meta, tag);
        }

        [[nodiscard]] bool equalsCommon(const ItemMeta &meta1, const ItemMeta &meta2) const
        {
            return equals_common_(meta1, meta2);
        }

        [[nodiscard]] bool notUncommon(const ItemMeta &meta1, const ItemMeta &meta2) const
        {
            return not_uncommon_(meta1, meta2);
        }

    private:
        std::function<std::unique_ptr<ItemMeta>(const ::ItemStackBase &)> from_item_stack_;
        std::function<std::unique_ptr<ItemMeta>(const std::string &, const ItemMeta *)> from_item_meta_;
        std::function<bool(const std::string &)> applicable_to_;
        std::function<void(const ItemMeta &, ::CompoundTag &)> apply_to_item_;
        std::function<bool(const ItemMeta &, const ItemMeta &)> equals_common_;
        std::function<bool(const ItemMeta &, const ItemMeta &)> not_uncommon_;
    };

    static ItemMetaDetails &getItemMetaDetails(ItemMeta::Type type)
    {
        switch (type) {
        case ItemMeta::Type::Map:
            return MAP;
        default:
            return ITEM;
        }
    }

    static ItemMetaDetails &getItemMetaDetails(const std::string &type)
    {
        static std::unordered_map<std::string, ItemMetaDetails *> look_up = {
            {"minecraft:air", &EMPTY},
            {"minecraft:filled_map", &MAP},
        };
        if (const auto it = look_up.find(type); it != look_up.end()) {
            return *it->second;
        }
        return ITEM;
    }

private:
    static ItemMetaDetails EMPTY;
    static ItemMetaDetails ITEM;
    static ItemMetaDetails MAP;
};
}  // namespace endstone::core
