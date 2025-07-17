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

#include "endstone/map/map_canvas.h"

namespace endstone {

class MapView;
class Player;

namespace core {
class EndstoneMapView;
}

/**
 * @brief Represents a renderer for a map.
 */
class MapRenderer {
public:
    /**
     * @brief Initialize the map renderer base with the given contextual status.
     *
     * @param is_contextual Whether the renderer is contextual. See isContextual().
     */
    explicit MapRenderer(const bool is_contextual = false) : is_contextual_(is_contextual) {}

    virtual ~MapRenderer() = default;

private:
    friend class core::EndstoneMapView;
    [[nodiscard]] virtual bool isEndstoneMapRenderer() const
    {
        return false;
    }

public:
    /**
     * @brief Get whether the renderer is contextual, i.e. has different canvases for different players.
     *
     * @return true if contextual, false otherwise.
     */
    [[nodiscard]] bool isContextual() const
    {
        return is_contextual_;
    }

    /**
     * @brief Initialize this MapRenderer for the given map.
     *
     * @param map The MapView being initialized.
     */
    virtual void initialize(MapView &map) {}

    /**
     * @brief Render to the given map.
     *
     * @param map The MapView being rendered to.
     * @param canvas The canvas to use for rendering.
     * @param player The player who triggered the rendering.
     */
    virtual void render(MapView &map, MapCanvas &canvas, Player &player) = 0;

private:
    bool is_contextual_;
};

}  // namespace endstone
