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

#include "endstone/util/color.h"
#include "endstone/util/image.h"

namespace endstone {

class MapView;

/**
 * @brief Represents a canvas for drawing to a map. Each canvas is associated with a
 *        specific MapRenderer and represents that renderer's layer on the map.
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
     * @brief Draw a pixel to the canvas.
     *
     * If std::nullopt is used as color, then the color returned by getBasePixelColor() is shown on the map.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     * @param color The color, or std::nullopt for base color.
     */
    virtual void setPixelColor(int x, int y, std::optional<Color> color) = 0;

    /**
     * @brief Get a pixel from the canvas.
     *
     * If no color is set at the given position for this canvas, then std::nullopt is
     * returned and the color returned by getBasePixelColor() is shown on the map.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     * @return The color, or std::nullopt if no color is set.
     */
    [[nodiscard]] virtual std::optional<Color> getPixelColor(int x, int y) const = 0;

    /**
     * @brief Get a pixel from the layers below this canvas.
     * @param x The x coordinate, from 0 to 127.
     *
     * @param y The y coordinate, from 0 to 127.
     * @return The color.
     */
    [[nodiscard]] virtual Color getBasePixelColor(int x, int y) const = 0;

    /**
     * @brief Draw a pixel to the canvas.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     * @param color The color.
     */
    virtual void setPixel(int x, int y, int color) = 0;

    /**
     * @brief Get a pixel from the canvas.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     * @return The color.
     */
    [[nodiscard]] virtual int getPixel(int x, int y) const = 0;

    /**
     * @brief Get a pixel from the layers below this canvas.
     *
     * @param x The x coordinate, from 0 to 127.
     * @param y The y coordinate, from 0 to 127.
     * @return The color.
     */
    [[nodiscard]] virtual int getBasePixel(int x, int y) const = 0;

    /**
     * @brief Draw an image to the map. The image will be clipped if necessary.
     *
     * @param x The x coordinate of the image.
     * @param y The y coordinate of the image.
     * @param image The Image to draw.
     */
    virtual void drawImage(int x, int y, const Image &image) = 0;

    // TODO(map): getCursors and setCursors
};

}  // namespace endstone
