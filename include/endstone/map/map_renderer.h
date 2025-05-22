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

namespace endstone {

class MapView;
class MapCanvas;
class Player;

/**
 * @brief Represents a renderer for a map.
 */
class MapRenderer {
public:
    virtual ~MapRenderer() = default;

    /**
     * @brief Get whether the renderer is contextual, i.e. has different canvases for different players.
     *
     * @return true if contextual, false otherwise.
     */
    [[nodiscard]] bool isContextual() const;

    /**
     * @brief Initialize this MapRenderer for the given map.
     *
     * @param map The MapView being initialized.
     */
    virtual void initialize(MapView &map);

    /**
     * @brief Render to the given map.
     *
     * @param map The MapView being rendered to.
     * @param canvas The canvas to use for rendering.
     * @param player The player who triggered the rendering.
     */
    virtual void render(MapView &map, MapCanvas &canvas, Player &player) = 0;

protected:
    /**
     * @brief Initialize the map renderer base to be non-contextual.
     */
    MapRenderer();

    /**
     * @brief Initialize the map renderer base with the given contextual status.
     * @param contextual Whether the renderer is contextual.
     */
    explicit MapRenderer(bool contextual);

private:
    bool is_contextual_;
};

}  // namespace endstone
