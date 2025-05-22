

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

#include <optional>
#include <string>
#include <utility>

namespace endstone {
class Color {
public:
    explicit Color(const std::uint8_t red = 0, const std::uint8_t green = 0, const std::uint8_t blue = 0,
                   const std::uint8_t alpha = 255)
        : r_(red), g_(green), b_(blue), a_(alpha)
    {
    }

    [[nodiscard]] std::uint8_t getRed() const
    {
        return r_;
    }

    [[nodiscard]] std::uint8_t getGreen() const
    {
        return g_;
    }

    [[nodiscard]] std::uint8_t getBlue() const
    {
        return b_;
    }

    [[nodiscard]] std::uint8_t getAlpha() const
    {
        return a_;
    }

private:
    std::uint8_t r_, g_, b_, a_;
};
}  // namespace endstone
```


