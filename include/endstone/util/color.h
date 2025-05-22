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

#include <optional>
#include <string>
#include <utility>

namespace endstone {
/**
 * @brief Represents a color with red, green, blue, and alpha components.
 */
class Color {
public:
    /**
     * @brief Creates a color with the specified red, green, blue, and alpha values in the range (0 - 255).
     *
     * @param red Red component (0–255)
     * @param green Green component (0–255)
     * @param blue Blue component (0–255)
     * @param alpha Alpha component (0–255, default 255 = opaque)
     */
    explicit Color(const std::uint8_t red = 0, const std::uint8_t green = 0, const std::uint8_t blue = 0,
                   const std::uint8_t alpha = 255)
        : r_(red), g_(green), b_(blue), a_(alpha)
    {
    }

    /**
     * @brief Gets the red component of the color.
     *
     * @return The red component (0–255) of the color.
     */
    [[nodiscard]] std::uint8_t getRed() const
    {
        return r_;
    }

    /**
     * @brief Gets the green component of the color.
     *
     * @return The green component (0–255) of the color.
     */
    [[nodiscard]] std::uint8_t getGreen() const
    {
        return g_;
    }

    /**
     * @brief Gets the blue component of the color.
     *
     * @return The blue component (0–255) of the color.
     */
    [[nodiscard]] std::uint8_t getBlue() const
    {
        return b_;
    }

    /**
     * @brief Gets the alpha component of the color.
     *
     * @return The alpha component (0–255) of the color.
     */
    [[nodiscard]] std::uint8_t getAlpha() const
    {
        return a_;
    }

private:
    std::uint8_t r_, g_, b_, a_;
};
}  // namespace endstone
