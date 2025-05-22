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

#include <string>

#include "endstone/util/color.h"

namespace endstone {
/**
 * @brief Represents an RGBA image.
 *
 * Each pixel is four bytes: R, G, B, A, in row-major order.
 */
class Image {
public:
    /**
     * @brief Create an empty image (all pixels transparent black).
     *
     * @param width  Image width in pixels
     * @param height Image height in pixels
     */
    Image(const int width, const int height) : width_(width), height_(height), data_(width * height * 4, '\0') {}

    /**
     * @brief Get the image width.
     *
     * @return Image width in pixels
     */
    [[nodiscard]] int getWidth() const
    {
        return width_;
    }

    /**
     * @brief Get the image height.
     *
     * @return Image height in pixels
     */
    [[nodiscard]] int getHeight() const
    {
        return height_;
    }

    /**
     * @brief Get the color of a pixel.
     *
     * @param x X coordinate (0 ≤ x < width)
     * @param y Y coordinate (0 ≤ y < height)
     *
     * @return Color at (x, y)
     */
    [[nodiscard]] Color getColor(const int x, const int y) const
    {
        const size_t idx = (static_cast<size_t>(y) * width_ + x) * 4;
        return Color(static_cast<uint8_t>(data_[idx]), static_cast<uint8_t>(data_[idx + 1]),
                     static_cast<uint8_t>(data_[idx + 2]), static_cast<uint8_t>(data_[idx + 3]));
    }

    /**
     * @brief Set the color of a pixel.
     * @param x     X coordinate (0 ≤ x < width)
     * @param y     Y coordinate (0 ≤ y < height)
     * @param color New color for the pixel
     */
    void setColor(const int x, const int y, const Color color)
    {
        const size_t idx = (static_cast<size_t>(y) * width_ + x) * 4;
        data_[idx + 0] = static_cast<char>(color.getRed());
        data_[idx + 1] = static_cast<char>(color.getGreen());
        data_[idx + 2] = static_cast<char>(color.getBlue());
        data_[idx + 3] = static_cast<char>(color.getAlpha());
    }

    /**
     * @brief Gets the raw pixel buffer (RGBA, row-major).
     *
     * @return The underlying image data buffer
     */
    [[nodiscard]] std::string_view getData() const
    {
        return data_;
    }

private:
    int width_;
    int height_;
    std::string data_;
};
}  // namespace endstone
