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

#include <cstdint>

#include "endstone/inventory/meta/item_meta.h"
#include "endstone/map/map_view.h"

namespace endstone {
/**
 * @brief Represents the metadata for a map item.
 */
class MapMeta : public ItemMeta {
public:
    ENDSTONE_ITEM_META_TYPE(Map)

    using MapId = std::int64_t;
    /**
     * @brief Checks for existence of a map ID number.
     *
     * @return true if this has a map ID number.
     */
    [[nodiscard]] virtual bool hasMapId() const = 0;

    /**
     * @brief Gets the map ID that is set. This is used to determine what map is displayed.
     *
     * @return the map ID that is set
     */
    [[nodiscard]] virtual MapId getMapId() const = 0;

    /**
     * @brief Sets the map ID. This is used to determine what map is displayed.
     *
     * @param id the map id to set
     */
    virtual void setMapId(MapId id) = 0;

    /**
     * @brief Checks for existence of an associated map.
     *
     * @return true if this item has an associated map
     */
    [[nodiscard]] virtual bool hasMapView() const = 0;

    /**
     * @brief Gets the map view associated with this map item.
     *
     * @note Plugins should check that hasMapView() returns true before calling this method.
     *
     * @return the map view, or nullptr if the item hasMapView(), but this map does not exist on the server
     */
    [[nodiscard]] virtual MapView *getMapView() const = 0;

    /**
     * @brief Sets the associated map. This is used to determine what map is displayed.
     *
     * @param map the map to set
     */
    virtual void setMapView(const MapView *map) = 0;
};
}  // namespace endstone
