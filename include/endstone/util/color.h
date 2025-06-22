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

#include "endstone/util/result.h"

namespace endstone {
/**
 * @brief Represents a color with red, green, blue, and alpha components.
 */
class Color {
public:
    static constexpr int BIT_MASK = 0xff;
    static constexpr int DEFAULT_ALPHA = 255;

    constexpr Color(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue,
                    const std::uint8_t alpha = DEFAULT_ALPHA)
        : red_(red), green_(green), blue_(blue), alpha_(alpha)
    {
    }

    /**
     * @brief Creates a new Color object from a red, green, blue, and alpha
     *
     * @param red integer from 0-255
     * @param green integer from 0-255
     * @param blue integer from 0-255
     * @param alpha integer from 0-255
     *
     * @return a new Color object for the red, green, blue, alpha
     */
    static Result<Color> fromRGBA(int red, int green, int blue, int alpha)
    {
        ENDSTONE_CHECKF(red >= 0 && red <= BIT_MASK, "Red[{}] must be between 0-255", red);
        ENDSTONE_CHECKF(green >= 0 && green <= BIT_MASK, "Green[{}] must be between 0-255", green);
        ENDSTONE_CHECKF(blue >= 0 && blue <= BIT_MASK, "Blue[{}] must be between 0-255", blue);
        ENDSTONE_CHECKF(alpha >= 0 && alpha <= BIT_MASK, "Alpha[{}] must be between 0-255", alpha);
        return Color(static_cast<std::uint8_t>(red), static_cast<std::uint8_t>(green), static_cast<std::uint8_t>(blue),
                     static_cast<std::uint8_t>(alpha));
    }

    /**
     * @brief Creates a new Color object from a red, green, and blue
     *
     * @param red integer from 0-255
     * @param green integer from 0-255
     * @param blue integer from 0-255
     *
     * @return a new Color object for the red, green, blue
     */
    static Result<Color> fromRGB(const int red, const int green, const int blue)
    {
        return fromRGBA(red, green, blue, DEFAULT_ALPHA);
    }

    /**
     * @brief Creates a new Color object from a blue, green, and red
     *
     * @param blue integer from 0-255
     * @param green integer from 0-255
     * @param red integer from 0-255
     *
     * @return a new Color object for the red, green, blue
     */
    static Result<Color> fromBGR(const int blue, const int green, const int red)
    {
        return fromRGBA(red, green, blue, DEFAULT_ALPHA);
    }

    /**
     * @brief Creates a new color object from an integer that contains the red, green,
     * blue, and alpha bytes.
     *
     * @param rgba the integer storing the red, green, blue, and alpha values
     *
     * @return a new color object for specified values
     */
    static Result<Color> fromRGBA(const int rgba)
    {
        return fromRGBA(rgba >> 24 & BIT_MASK, rgba >> 16 & BIT_MASK, rgba >> 8 & BIT_MASK, rgba & BIT_MASK);
    }

    /**
     * @brief Creates a new color object from an integer that contains the red, green,
     * and blue bytes in the lowest order 24 bits.
     *
     * @param rgb the integer storing the red, green, and blue values
     *
     * @return a new color object for specified values
     */
    static Result<Color> fromRGB(const int rgb)
    {
        ENDSTONE_CHECKF((rgb >> 24) == 0, "Extraneous data in: {}", rgb);
        return fromRGB(rgb >> 16 & BIT_MASK, rgb >> 8 & BIT_MASK, rgb & BIT_MASK);
    }

    static Result<Color> fromBGR(const int bgr)
    {
        ENDSTONE_CHECKF((bgr >> 24) == 0, "Extraneous data in: {}", bgr);
        return fromBGR(bgr >> 16 & BIT_MASK, bgr >> 8 & BIT_MASK, bgr & BIT_MASK);
    }

    /**
     * @brief Gets the red component of the color.
     *
     * @return The red component (0–255) of the color.
     */
    [[nodiscard]] int getRed() const
    {
        return BIT_MASK & red_;
    }

    /**
     * @brief Creates a new Color object with specified component
     *
     * @param red the red component, from 0 to 255
     *
     * @return a new color object with the red component
     */
    [[nodiscard]] Result<Color> setRed(const int red) const
    {
        return fromRGBA(red, getGreen(), getBlue(), getAlpha());
    }

    /**
     * @brief Gets the green component of the color.
     *
     * @return The green component (0–255) of the color.
     */
    [[nodiscard]] int getGreen() const
    {
        return BIT_MASK & green_;
    }

    /**
     * @brief Creates a new Color object with specified component
     *
     * @param green the green component, from 0 to 255
     *
     * @return a new color object with the green component
     */
    [[nodiscard]] Result<Color> setGreen(const int green) const
    {
        return fromRGBA(getRed(), green, getBlue(), getAlpha());
    }

    /**
     * @brief Gets the blue component of the color.
     *
     * @return The blue component (0–255) of the color.
     */
    [[nodiscard]] int getBlue() const
    {
        return BIT_MASK & blue_;
    }

    /**
     * @brief Creates a new Color object with specified component
     *
     * @param blue the blue component, from 0 to 255
     *
     * @return a new color object with the blue component
     */
    [[nodiscard]] Result<Color> setBlue(const int blue) const
    {
        return fromRGBA(getRed(), getGreen(), blue, getAlpha());
    }

    /**
     * @brief Gets the alpha component of the color.
     *
     * @return The alpha component (0–255) of the color.
     */
    [[nodiscard]] int getAlpha() const
    {
        return BIT_MASK & alpha_;
    }

    /**
     * @brief Creates a new Color object with specified component
     *
     * @param alpha the alpha component, from 0 to 255
     *
     * @return a new color object with the alpha component
     */
    [[nodiscard]] Result<Color> setAlpha(const int alpha) const
    {
        return fromRGBA(getRed(), getGreen(), getBlue(), alpha);
    }

    /**
     * @brief Gets the color as an RGBA integer.
     *
     * @return An integer representation of this color, as 0xRRGGBBAA
     */
    [[nodiscard]] int asRGBA() const
    {
        return getRed() << 24 | getGreen() << 16 | getBlue() << 8 | getAlpha();
    }

    /**
     * @brief Gets the color as an RGB integer.
     *
     * @return An integer representation of this color, as 0xRRGGBB
     */
    [[nodiscard]] int asRGB() const
    {
        return getRed() << 16 | getGreen() << 8 | getBlue();
    }

    /**
     * @brief Gets the color as an BGR integer.
     *
     * @return An integer representation of this color, as 0xBBGGRR
     */
    [[nodiscard]] int asBGR() const
    {
        return getBlue() << 16 | getGreen() << 8 | getRed();
    }

    bool operator==(const Color &other) const
    {
        return alpha_ == other.alpha_ && red_ == other.red_ && green_ == other.green_ && blue_ == other.blue_;
    }

    bool operator!=(const Color &other) const
    {
        return !(*this == other);
    }

private:
    std::uint8_t red_, green_, blue_, alpha_;
};

class Colors {
public:
    static constexpr auto WHITE = Color(Color::DEFAULT_ALPHA, 255, 255, 255);
    static constexpr auto BLACK = Color(Color::DEFAULT_ALPHA, 0, 0, 0);
};
}  // namespace endstone
