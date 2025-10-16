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

#include "endstone/inventory/meta/map_meta.h"

#include "bedrock/world/item/map_item.h"
#include "endstone/core/inventory/item_metas.h"

namespace endstone::core {
template <>
bool EndstoneItemMetas::applicableTo<MapMeta>(const std::string &type)
{
    return applicableTo<ItemMeta>(type);
}

template <>
void EndstoneItemMetas::applyToItem<MapMeta>(const MapMeta &self, ::CompoundTag &tag)
{
    applyToItem<ItemMeta>(self, tag);

    tag.remove(MapItem::TAG_MAP_UUID);
    if (self.hasMapId()) {
        tag.putInt64(MapItem::TAG_MAP_UUID, self.getMapId());
    }
}

template <>
bool EndstoneItemMetas::equalsCommon<MapMeta>(const MapMeta &self, const ItemMeta &that)
{
    if (!equalsCommon<ItemMeta>(self, that)) {
        return false;
    }
    if (const auto *other = that.as<MapMeta>()) {
        return (self.hasMapId() ? other->hasMapId() && self.getMapId() == other->getMapId() : !other->hasMapId());
    }
    return true;
}

template <>
bool EndstoneItemMetas::notUncommon<MapMeta>(const MapMeta &self, const ItemMeta &that)
{
    return notUncommon<ItemMeta>(self, that);
}

template <>
void EndstoneItemMetas::loadFrom<MapMeta>(MapMeta &self, const ::ItemStackBase &item)
{
    if (!item.hasUserData()) {
        return;
    }

    loadFrom<ItemMeta>(self, item);

    const auto &tag = *item.getUserData();
    if (const auto map_id = tag.getInt64(MapItem::TAG_MAP_UUID)) {
        self.setMapId(map_id);
    }
}
}  // namespace endstone::core
