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

#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

namespace mce {
class Color {
public:
    [[nodiscard]] std::string toHexString() const
    {
        std::stringstream stream;
        stream << "#" << std::setfill('0') << std::hex       //
               << std::setw(2) << static_cast<int>(r * 255)  //
               << std::setw(2) << static_cast<int>(g * 255)  //
               << std::setw(2) << static_cast<int>(b * 255)  //
               << std::setw(2) << static_cast<int>(a * 255);
        return stream.str();
    }

    static Color fromHexString(const std::string &hex_string)
    {
        Color color = {0};
        if (hex_string.length() != 8) {
            return color;
        }

        auto value = std::strtoul(hex_string.c_str(), nullptr, 16);
        color.r = static_cast<float>(((value >> 24) & 0xFF)) / 255.0F;
        color.g = static_cast<float>(((value >> 16) & 0xFF)) / 255.0F;
        color.b = static_cast<float>(((value >> 8) & 0xFF)) / 255.0F;
        color.a = static_cast<float>((value & 0xFF)) / 255.0F;
        return color;
    }

    float r;
    float g;
    float b;
    float a;
};

}  // namespace mce
