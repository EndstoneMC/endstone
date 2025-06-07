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

#include "endstone/map/map_renderer.h"

namespace endstone {

class Dimension;

/**
 * @brief Represents a map item.
 */
class MapView {
public:
    /**
     * @brief An enum representing all possible scales a map can be set to.
     */
    enum class Scale : uint8_t {
        Closest = 0,
        Close = 1,
        Normal = 2,
        Far = 3,
        Farthest = 4
    };

    virtual ~MapView() = default;

    /**
     * @brief Get the ID of this map item for use with MapMeta.
     *
     * @return The unique ID of the map.
     */
    [[nodiscard]] virtual std::int64_t getId() const = 0;

    /**
     * @brief Check whether this map is virtual.
     *
     * A map is virtual if its lowermost MapRenderer is plugin-provided.
     *
     * @return True if the map is virtual.
     */
    [[nodiscard]] virtual bool isVirtual() const = 0;

    /**
     * @brief Get the scale of this map.
     *
     * @return The scale of the map.
     */
    [[nodiscard]] virtual Scale getScale() const = 0;

    /**
     * @brief Set the scale of this map.
     *
     * @param scale The scale to set.
     */
    virtual void setScale(Scale scale) = 0;

    /**
     * @brief Get the center X position of this map.
     *
     * @return The center X position.
     */
    [[nodiscard]] virtual int getCenterX() const = 0;

    /**
     * @brief Get the center Z position of this map.
     *
     * @return The center Z position.
     */
    [[nodiscard]] virtual int getCenterZ() const = 0;

    /**
     * @brief Set the center X position of this map.
     *
     * @param x The center X position.
     */
    virtual void setCenterX(int x) = 0;

    /**
     * @brief Set the center Z position of this map.
     *
     * @param z The center Z position.
     */
    virtual void setCenterZ(int z) = 0;

    /**
     * @brief Get the dimension that this map is associated with.
     *
     * @return Pointer to the World this map is associated with, or nullptr.
     */
    [[nodiscard]] virtual Dimension *getDimension() const = 0;

    /**
     * @brief Set the dimension that this map is associated with.
     *
     * @param dimension The Dimension to associate this map with.
     */
    virtual void setDimension(const Dimension &dimension) = 0;

    /**
     * @brief Get a list of MapRenderers currently in effect.
     *
     * @return A list of references to each MapRenderer.
     */
    [[nodiscard]] virtual std::vector<MapRenderer *> getRenderers() const = 0;

    /**
     * @brief Add a renderer to this map.
     *
     * @param renderer The MapRenderer to add.
     */
    virtual void addRenderer(std::shared_ptr<MapRenderer> renderer) = 0;

    /**
     * @brief Remove a renderer from this map.
     *
     * @param renderer The MapRenderer to remove.
     *
     * @return True if the renderer was successfully removed.
     */
    virtual bool removeRenderer(MapRenderer *renderer) = 0;

    /**
     * @brief Whether the map will show a smaller position cursor (true), or no position cursor (false) when cursor is
     * outside of map's range.
     *
     * @return True if unlimited tracking is enabled.
     */
    [[nodiscard]] virtual bool isUnlimitedTracking() const = 0;

    /**
     * @brief Whether the map will show a smaller position cursor (true), or no position cursor (false) when cursor is
     * outside of map's range.
     *
     * @param unlimited True to enable unlimited tracking.
     */
    virtual void setUnlimitedTracking(bool unlimited) = 0;

    /**
     * @brief Gets whether the map is locked or not. A locked map may not be explored further.
     *
     * @return True if the map is locked.
     */
    [[nodiscard]] virtual bool isLocked() const = 0;

    /**
     * @brief Sets whether the map is locked or not. A locked map may not be explored further.
     *
     * @param locked True to lock the map.
     */
    virtual void setLocked(bool locked) = 0;
};

}  // namespace endstone
