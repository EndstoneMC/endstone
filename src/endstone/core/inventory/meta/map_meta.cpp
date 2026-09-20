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
#include "endstone/core/inventory/meta/map_meta.h"
#include "endstone/core/server.h"

namespace endstone::core {
EndstoneMapMeta::EndstoneMapMeta(const ItemMeta *meta) : EndstoneItemMetaBase(meta)
{
    if (!meta || !meta->as<MapMeta>()) {
        return;
    }
    auto *m = static_cast<const EndstoneMapMeta *>(meta);
    map_id_ = m->map_id_;
    display_players_ = m->display_players_;
}

EndstoneMapMeta::EndstoneMapMeta(const ::CompoundTag &tag) : EndstoneItemMetaBase(tag)
{
    if (const auto map_id = tag.getInt64(MapItem::TAG_MAP_UUID)) {
        map_id_ = map_id;
    }
    if (tag.get(MapItem::TAG_MAP_PLAYER_DISPLAY)) {
        display_players_ = tag.getByte(MapItem::TAG_MAP_PLAYER_DISPLAY) != 0;
    }
}

bool EndstoneMapMeta::hasMapId() const
{
    return map_id_ != InvalidMapId;
}

MapMeta::MapId EndstoneMapMeta::getMapId() const
{
    return map_id_;
}

void EndstoneMapMeta::setMapId(const MapId id)
{
    map_id_ = id;
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
    map_id_ = map ? map->getId() : InvalidMapId;
}

void EndstoneMapMeta::applyToItem(::CompoundTag &tag) const
{
    EndstoneItemMetaBase::applyToItem(tag);
    tag.remove(MapItem::TAG_MAP_UUID);
    tag.remove(MapItem::TAG_MAP_PLAYER_DISPLAY);
    if (hasMapId()) {
        tag.putInt64(MapItem::TAG_MAP_UUID, getMapId());
    }
    if (display_players_.has_value()) {
        tag.putBoolean(MapItem::TAG_MAP_PLAYER_DISPLAY, display_players_.value());
    }
}

bool EndstoneMapMeta::isEmpty() const
{
    return EndstoneItemMetaBase::isEmpty() && isMapEmpty();
}

bool EndstoneMapMeta::equalsCommon(const ItemMeta &meta) const
{
    if (!EndstoneItemMetaBase<MapMeta>::equalsCommon(meta)) {
        return false;
    }
    if (meta.as<MapMeta>()) {
        auto &that = static_cast<const EndstoneMapMeta &>(meta);
        return (hasMapId() ? that.hasMapId() && map_id_ == that.map_id_ : !that.hasMapId()) &&
               display_players_ == that.display_players_;
    }
    return true;
}

bool EndstoneMapMeta::notUncommon(const ItemMeta &meta) const
{
    return EndstoneItemMetaBase::notUncommon(meta) && (meta.as<MapMeta>() || isMapEmpty());
}

std::unique_ptr<ItemMeta> EndstoneMapMeta::clone() const
{
    return std::make_unique<EndstoneMapMeta>(*this);
}

bool EndstoneMapMeta::isMapEmpty() const
{
    return !hasMapId() && !display_players_.has_value();
}
}  // namespace endstone::core
