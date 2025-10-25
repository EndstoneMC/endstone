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

#include "endstone/detail/endstone.h"
#include "endstone/inventory/meta/item_meta.h"
#include "endstone/map/map_view.h"

namespace endstone {
/**
 * @brief Represents the metadata for a map item.
 */
class MapMeta final : public ItemMeta {
public:
    explicit MapMeta(const ItemMeta *meta) : ItemMeta(meta)
    {
        const auto *map = meta ? meta->as<MapMeta>() : nullptr;
        if (!map) {
            return;
        }
        map_id_ = map->map_id_;
    }

    static constexpr auto TYPE = Type::Map;
    [[nodiscard]] Type getType() const override
    {
        return TYPE;
    }

    [[nodiscard]] bool isEmpty() const override
    {
        return ItemMeta::isEmpty() && isMapEmpty();
    }

    [[nodiscard]] std::unique_ptr<ItemMeta> clone() const override
    {
        return std::make_unique<MapMeta>(this);
    }

    /**
     * @brief Checks for existence of a map ID number.
     *
     * @return true if this has a map ID number.
     */
    [[nodiscard]] bool hasMapId() const
    {
        return map_id_ != -1;
    }

    /**
     * @brief Gets the map ID that is set. This is used to determine what map is displayed.
     *
     * @return the map ID that is set
     */
    [[nodiscard]] std::int64_t getMapId() const
    {
        return map_id_;
    }

    /**
     * @brief Sets the map ID. This is used to determine what map is displayed.
     *
     * @param id the map id to set
     */
    void setMapId(std::int64_t id)
    {
        map_id_ = id;
    }

    /**
     * @brief Checks for existence of an associated map.
     *
     * @return true if this item has an associated map
     */
    [[nodiscard]] bool hasMapView() const
    {
        return hasMapId();
    }

    /**
     * @brief Gets the map view associated with this map item.
     *
     * @note Plugins should check that hasMapView() returns true before calling this method.
     *
     * @return the map view, or nullptr if the item hasMapView(), but this map does not exist on the server
     */
    [[nodiscard]] MapView *getMapView() const
    {
        return Endstone::getServer().getMap(getMapId());
    }

    /**
     * @brief Sets the associated map. This is used to determine what map is displayed.
     *
     * @param map the map to set
     */
    void setMapView(const MapView *map)
    {
        map_id_ = map ? map->getId() : -1;
    }

protected:
    [[nodiscard]] bool isMapEmpty() const
    {
        return !(hasMapId());  // TODO(map): add hasScaling
    }

private:
    std::int64_t map_id_ = -1;
};
}  // namespace endstone
