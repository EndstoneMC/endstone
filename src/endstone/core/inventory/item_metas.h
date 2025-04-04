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

#include <entt/entt.hpp>

#include "bedrock/world/item/item_stack.h"
#include "endstone/inventory/item_type.h"
#include "endstone/inventory/meta/item_meta.h"
#include "endstone/inventory/meta/map_meta.h"

namespace endstone::core {

class EndstoneItemMetas {
public:
    class ItemMetaData {
    public:
        template <typename Base>
        struct type : Base {
            std::unique_ptr<ItemMeta> fromItemStack(const ::ItemStack &stack) const
            {
                return entt::poly_call<0>(*this, stack);
            }

            std::unique_ptr<ItemMeta> fromItemMeta(const ItemMeta &meta) const
            {
                return entt::poly_call<1>(*this, meta);
            }

            void restoreItemMeta(::ItemStack &stack)
            {
                entt::poly_call<2>(*this, stack);
            }

            void applyMetaToItem(const ItemMeta &meta, ::ItemStack &stack)
            {
                entt::poly_call<3>(*this, meta, stack);
            }
        };

        template <typename Type>
        using impl =
            entt::value_list<&Type::fromItemStack, &Type::fromItemMeta, &Type::restore, &Type::applyToItem>;
    };

    static entt::poly<ItemMetaData> getItemMetaData(const ItemType &item_type);
};

}  // namespace endstone::core
