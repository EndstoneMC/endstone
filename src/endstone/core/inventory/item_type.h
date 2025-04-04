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

#include <entt/entt.hpp>

#include "bedrock/world/item/item.h"
#include "endstone/core/inventory/item_metas.h"
#include "endstone/core/inventory/meta/item_meta.h"
#include "endstone/core/registry/registry_item.h"
#include "endstone/inventory/item_type.h"
#include "endstone/inventory/meta/item_meta.h"

namespace endstone::core {
class EndstoneItemType : public ItemType, EndstoneRegistryItem<const ::Item> {
public:
    EndstoneItemType(std::string key, const ::Item &item);
    std::unique_ptr<ItemMeta> getItemMeta(const ::ItemStack &stack);
    std::unique_ptr<ItemMeta> getItemMeta(const ItemMeta &meta);
    void restoreItemMeta(::ItemStack &stack);
    void applyMetaToItem(const ItemMeta &meta, ::ItemStack &stack);

    static std::unique_ptr<EndstoneItemType> fromMinecraft(const ::Item &item);

private:
    entt::poly<EndstoneItemMetas::ItemMetaData> item_metadata_{EndstoneItemMeta{}};
};
}  // namespace endstone::core
