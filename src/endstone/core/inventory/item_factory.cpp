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

#include "endstone/core/inventory/item_factory.h"

namespace endstone::core {

template <>
void loadData(std::shared_ptr<ItemMeta> meta, const ItemStackBase &item)
{
    const auto *tag = item.getUserData();
    if (!tag) {
        return;
    }
    const auto *display_tag = tag->getCompound(ItemStackBase::TAG_DISPLAY);
    if (!display_tag) {
        return;
    }
    const auto *lore_tag = display_tag->getList(ItemStackBase::TAG_LORE);
    if (!lore_tag) {
        return;
    }
    std::vector<std::string> lore;
    for (auto i = 0; i < lore_tag->size(); i++) {
        if (auto str = lore_tag->getString(i); !str.empty()) {
            lore.emplace_back(str);
        }
    }
    if (!lore.empty()) {
        meta->setLore(lore);
    }
}

template <>
void loadData(std::shared_ptr<MapMeta> meta, const ItemStackBase &item)
{
    // TODO(item): implement this
    loadData<ItemMeta>(meta, item);
}

std::shared_ptr<ItemMeta> EndstoneItemFactory::getItemMeta(const std::string_view type, const ItemStackBase &item)
{
    auto meta = ItemFactory::getItemMeta(type);
    switch (meta->getType()) {
    case ItemMeta::Type::Map:
        loadData<MapMeta>(std::static_pointer_cast<MapMeta>(meta), item);
        break;
    default:
        loadData<ItemMeta>(meta, item);
        break;
    }
    return meta;
}
}  // namespace endstone::core
