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

#include "endstone/core/inventory/meta/map_meta.h"

#include "bedrock/world/item/map_item.h"
#include "endstone/core/server.h"

namespace endstone::core {
EndstoneMapMeta::EndstoneMapMeta(const EndstoneItemMeta *meta) : EndstoneItemMeta(meta)
{
    if (!meta || meta->getType() != Type::Map) {
        return;
    }
    *this = *static_cast<const EndstoneMapMeta *>(meta);
}

EndstoneMapMeta::EndstoneMapMeta(const CompoundTag &tag) : EndstoneItemMeta(tag)
{
    if (const auto map_id = tag.getInt64(MapItem::TAG_MAP_UUID)) {
        map_id_ = map_id;
    }
}

ItemMeta::Type EndstoneMapMeta::getType() const
{
    return Type::Map;
}

bool EndstoneMapMeta::isEmpty() const
{
    return EndstoneItemMeta::isEmpty() && isMapEmpty();
}

std::unique_ptr<ItemMeta> EndstoneMapMeta::clone() const
{
    std::unique_ptr<MapMeta> copy = std::make_unique<EndstoneMapMeta>(*this);
    return copy;
}

bool EndstoneMapMeta::equalsCommon(const EndstoneItemMeta &other) const
{
    if (!EndstoneItemMeta::equalsCommon(other)) {
        return false;
    }
    if (other.getType() == Type::Map) {
        const auto &that = static_cast<const EndstoneMapMeta &>(other);
        return (hasMapId() ? that.hasMapId() && map_id_ == that.map_id_ : !that.hasMapId());
    }
    return true;
}

bool EndstoneMapMeta::notUncommon(const EndstoneItemMeta &other) const
{
    return EndstoneItemMeta::notUncommon(other) && (other.getType() == Type::Map || isMapEmpty());
}

void EndstoneMapMeta::applyToItem(CompoundTag &tag) const
{
    EndstoneItemMeta::applyToItem(tag);
    if (hasMapId()) {
        tag.putInt64(MapItem::TAG_MAP_UUID, getMapId());
    }
}

bool EndstoneMapMeta::hasMapView() const
{
    return hasMapId();
}

MapView *EndstoneMapMeta::getMapView() const
{
    if (!hasMapView()) {
        return nullptr;
    }
    return EndstoneServer::getInstance().getMap(getMapId());
}

void EndstoneMapMeta::setMapView(const MapView *map)
{
    if (map == nullptr) {
        map_id_ = std::nullopt;
    }
    else {
        map_id_ = map->getId();
    }
}

bool EndstoneMapMeta::hasMapId() const
{
    return map_id_.has_value();
}

std::int64_t EndstoneMapMeta::getMapId() const
{
    return map_id_.value_or(-1);
}

void EndstoneMapMeta::setMapId(std::int64_t id)
{
    map_id_ = id;
}

bool EndstoneMapMeta::isMapEmpty() const
{
    return !(hasMapId());
}
}  // namespace endstone::core
