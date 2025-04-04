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

#include "endstone/core/inventory/item_type.h"

namespace endstone::core {

EndstoneItemType::EndstoneItemType(std::string key, const ::Item &item) : EndstoneRegistryItem(std::move(key), item)
{
    item_metadata_ = EndstoneItemMetas::getItemMetaData(*this);
}

std::unique_ptr<ItemMeta> EndstoneItemType::getItemMeta(const ItemStack &stack)
{
    return item_metadata_->fromItemStack(stack);
}

std::unique_ptr<ItemMeta> EndstoneItemType::getItemMeta(const ItemMeta &meta)
{
    return item_metadata_->fromItemMeta(meta);
}

void EndstoneItemType::restoreItemMeta(::ItemStack &stack)
{
    return item_metadata_->restoreItemMeta(stack);
}

void EndstoneItemType::applyMetaToItem(const ItemMeta &meta, ItemStack &stack)
{
    return item_metadata_->applyMetaToItem(meta, stack);
}

std::unique_ptr<EndstoneItemType> EndstoneItemType::fromMinecraft(const ::Item &item)
{
    // TODO(registry): get from item registry instead
    return std::make_unique<EndstoneItemType>(item.getFullItemName(), item);
}

}  // namespace endstone::core
