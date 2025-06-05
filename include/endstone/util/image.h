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
    enum class Type {
        Invalid = 0,
        /**
         * @brief 8-bit pixels, grayscale
         */
        Grayscale = 1,
        /**
         * @brief 3x8-bit pixels, true color
         */
        RGB = 3,
        /**
         * 4x8-bit pixels, true color with transparency mask
         */
        RGBA = 4,
    };

    /**
     * @brief Create an empty image (all pixels transparent black).
     *
     * @param type   Image type
     * @param width  Image width in pixels
     * @param height Image height in pixels
     */
    explicit Image(const Type type, const int width, const int height)
        : type_(type), width_(width), height_(height), data_(width * height * 4, '\0')
    {
    }

private:
    Image(const Type type, const int width, const int height, const std::string_view data)
        : type_(type), width_(width), height_(height), data_(data)
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
     * @brief Get the image depth.
     *
     * @return Image depth
     */
    [[nodiscard]] int getDepth() const
    {
        switch (type_) {
        case Type::Grayscale:
            return 1;
        case Type::RGB:
            return 3;
        case Type::RGBA:
            return 4;
        default:
            return 0;
        }
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
        std::uint8_t r = 0, g = 0, b = 0, a = 255;
        switch (type_) {
        case Type::Grayscale:
            r = g = b = static_cast<uint8_t>(data_[idx]);
            break;
        case Type::RGB:
            r = static_cast<uint8_t>(data_[idx]);
            g = static_cast<uint8_t>(data_[idx + 1]);
            b = static_cast<uint8_t>(data_[idx + 2]);
            break;
        case Type::RGBA:
            r = static_cast<uint8_t>(data_[idx]);
            g = static_cast<uint8_t>(data_[idx + 1]);
            b = static_cast<uint8_t>(data_[idx + 2]);
            a = static_cast<uint8_t>(data_[idx + 3]);
            break;
        default:
            break;
        }
        return {r, g, b, a};
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
        switch (type_) {
        case Type::Grayscale:
            data_[idx] = static_cast<char>(color.getRed());
            break;
        case Type::RGB:
            data_[idx] = static_cast<char>(color.getRed());
            data_[idx + 1] = static_cast<char>(color.getGreen());
            data_[idx + 2] = static_cast<char>(color.getBlue());
            break;
        case Type::RGBA:
            data_[idx] = static_cast<char>(color.getRed());
            data_[idx + 1] = static_cast<char>(color.getGreen());
            data_[idx + 2] = static_cast<char>(color.getBlue());
            data_[idx + 3] = static_cast<char>(color.getAlpha());
            break;
        default:
            return;
        }
    }

    /**
     * @brief Gets the raw pixel buffer (row-major, H x W x D).
     *
     * @return The underlying image data buffer
     */
    [[nodiscard]] std::string_view getData() const
    {
        return data_;
    }

    /**
     * @brief Creates an image from the pixel data in a byte buffer.
     *
     * @param type     Image type
     * @param width    Image width in pixels
     * @param height   Image height in pixels
     * @param buffer
     * @return         The image on success, or an error on failure.
     */
    static Result<Image> fromBuffer(Type type, const int width, const int height, std::string_view buffer)
    {
        size_t depth = 0;
        switch (type) {
        case Type::Grayscale:
            depth = 1;
            break;
        case Type::RGB:
            depth = 3;
            break;
        case Type::RGBA:
            depth = 4;
            break;
        default:
            return nonstd::make_unexpected("Image::fromBuffer: invalid image type");
        }

        ENDSTONE_CHECKF(buffer.size() == width * height * depth,
                        "Image::fromBuffer: invalid buffer size, got {} bytes, expected {} bytes.", buffer.size(),
                        width * height * depth);

        return Image(type, width, height, buffer);
    }

private:
    Type type_;
    int width_;
    int height_;
    std::string data_;
};
}  // namespace endstone
