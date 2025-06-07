

# File color.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**util**](dir_89b85071337bf933dea6c29b4c6a4410.md) **>** [**color.h**](color_8h.md)

[Go to the documentation of this file](color_8h.md)


```C++
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
class Color {
public:
    static constexpr int BIT_MASK = 0xff;
    static constexpr int DEFAULT_ALPHA = 255;

    constexpr Color(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue,
                    const std::uint8_t alpha = DEFAULT_ALPHA)
        : red_(red), green_(green), blue_(blue), alpha_(alpha)
    {
    }

    static Result<Color> fromRGBA(int red, int green, int blue, int alpha)
    {
        ENDSTONE_CHECKF(red >= 0 && red <= BIT_MASK, "Red[{}] must be between 0-255", red);
        ENDSTONE_CHECKF(green >= 0 && green <= BIT_MASK, "Green[{}] must be between 0-255", green);
        ENDSTONE_CHECKF(blue >= 0 && blue <= BIT_MASK, "Blue[{}] must be between 0-255", blue);
        ENDSTONE_CHECKF(alpha >= 0 && alpha <= BIT_MASK, "Alpha[{}] must be between 0-255", alpha);
        return Color(static_cast<std::uint8_t>(red), static_cast<std::uint8_t>(green), static_cast<std::uint8_t>(blue),
                     static_cast<std::uint8_t>(alpha));
    }

    static Result<Color> fromRGB(const int red, const int green, const int blue)
    {
        return fromRGBA(red, green, blue, DEFAULT_ALPHA);
    }

    static Result<Color> fromBGR(const int blue, const int green, const int red)
    {
        return fromRGBA(red, green, blue, DEFAULT_ALPHA);
    }

    static Result<Color> fromRGBA(const int rgba)
    {
        return fromRGBA(rgba >> 24 & BIT_MASK, rgba >> 16 & BIT_MASK, rgba >> 8 & BIT_MASK, rgba & BIT_MASK);
    }

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

    [[nodiscard]] int getRed() const
    {
        return BIT_MASK & red_;
    }

    [[nodiscard]] Result<Color> setRed(const int red) const
    {
        return fromRGBA(red, getGreen(), getBlue(), getAlpha());
    }

    [[nodiscard]] int getGreen() const
    {
        return BIT_MASK & green_;
    }

    [[nodiscard]] Result<Color> setGreen(const int green) const
    {
        return fromRGBA(getRed(), green, getBlue(), getAlpha());
    }

    [[nodiscard]] int getBlue() const
    {
        return BIT_MASK & blue_;
    }

    [[nodiscard]] Result<Color> setBlue(const int blue) const
    {
        return fromRGBA(getRed(), getGreen(), blue, getAlpha());
    }

    [[nodiscard]] int getAlpha() const
    {
        return BIT_MASK & alpha_;
    }

    [[nodiscard]] Result<Color> setAlpha(const int alpha) const
    {
        return fromRGBA(getRed(), getGreen(), getBlue(), alpha);
    }

    [[nodiscard]] int asRGBA() const
    {
        return getRed() << 24 | getGreen() << 16 | getBlue() << 8 | getAlpha();
    }

    [[nodiscard]] int asRGB() const
    {
        return getRed() << 16 | getGreen() << 8 | getBlue();
    }

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
```


