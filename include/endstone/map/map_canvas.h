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
#include <vector>

#include "endstone/map/map_cursor.h"
#include "endstone/util/color.h"
#include "endstone/util/image.h"

namespace endstone {

class MapView;

/**
 * @brief Represents a canvas for drawing to a map.
 *
 * @note Each canvas is associated with a specific MapRenderer and represents that renderer's layer on the map.
 */
class MapCanvas {
public:
    virtual ~MapCanvas() = default;

    /**
     * @brief Get the map this canvas is attached to.
     *
     * @return The MapView this canvas is attached to.
     */
    [[nodiscard]] virtual MapView &getMapView() const = 0;

    /**
     * @brief Get the cursors associated with this canvas.
     *
     * @return The MapCursorCollection associated with this canvas.
     */
    [[nodiscard]] virtual std::vector<MapCursor> getCursors() const = 0;

    /**
     * @brief Set the cursors associated with this canvas.
     *
     * @param cursors The MapCursorCollection to associate with this canvas.
     */
    virtual void setCursors(const std::vector<MapCursor> &cursors) = 0;

    /**
     * @brief Draw a pixel to the canvas.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     * @param color The color
     */
    virtual void setPixelColor(int x, int y, Color color) = 0;

    /**
     * @brief Get a pixel from the canvas.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     *
     * @return The color
     */
    [[nodiscard]] virtual Color getPixelColor(int x, int y) const = 0;

    /**
     * @brief Get a pixel from the layers below this canvas.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     *
     * @return The color
     */
    [[nodiscard]] virtual Color getBasePixelColor(int x, int y) const = 0;

    /**
     * @brief Draw a pixel to the canvas.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     * @param color The color.
     */
    virtual void setPixel(int x, int y, std::uint32_t color) = 0;

    /**
     * @brief Get a pixel from the canvas.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     * @return The color.
     */
    [[nodiscard]] virtual std::uint32_t getPixel(int x, int y) const = 0;

    /**
     * @brief Get a pixel from the layers below this canvas.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     * @return The color.
     */
    [[nodiscard]] virtual std::uint32_t getBasePixel(int x, int y) const = 0;

    /**
     * @brief Draw an image to the map. The image will be clipped if necessary.
     *
     * @param x The x coordinate of the image.
     * @param y The y coordinate of the image.
     * @param image The Image to draw.
     */
    virtual void drawImage(int x, int y, const Image &image) = 0;
};

}  // namespace endstone
