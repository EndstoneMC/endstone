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
    explicit Image(const int width, const int height) : width_(width), height_(height), data_(width * height * 4, '\0')
    {
    }

private:
    Image(const int width, const int height, const std::string_view data) : width_(width), height_(height), data_(data)
    {
    }

public:
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

    /**
     * @brief Creates an image from the pixel data in a byte buffer.
     * @param width    Image width in pixels
     * @param height   Image height in pixels
     * @param buffer  A string_view over exactly width*height*4 bytes of RGBA data
     * @return         The image on success, or an error on failure.
     */
    static Result<Image> fromBuffer(const int width, const int height, std::string_view buffer)
    {
        ENDSTONE_CHECK(width > 0 && height > 0, "Width and height must be positive");
        ENDSTONE_CHECKF(buffer.size() == width * height * 4,
                        "Buffer size {} does not match expected size {} (width * height * 4)", buffer.size(),
                        width * height * 4);
        return Image(width, height, buffer);
    }

private:
    int width_;
    int height_;
    std::string data_;
};
}  // namespace endstone
