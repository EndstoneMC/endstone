

# File color\_format.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**color\_format.h**](color__format_8h.md)

[Go to the documentation of this file](color__format_8h.md)


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

#include <string>

namespace endstone {

struct ColorFormat {
    // Escape
    inline static const std::string Escape = "§";

    // Color code
    inline static const std::string Black = Escape + '0';
    inline static const std::string DarkBlue = Escape + '1';
    inline static const std::string DarkGreen = Escape + '2';
    inline static const std::string DarkAqua = Escape + '3';
    inline static const std::string DarkRed = Escape + '4';
    inline static const std::string DarkPurple = Escape + '5';
    inline static const std::string Gold = Escape + '6';
    inline static const std::string Gray = Escape + '7';
    inline static const std::string DarkGray = Escape + '8';
    inline static const std::string Blue = Escape + '9';
    inline static const std::string Green = Escape + 'a';
    inline static const std::string Aqua = Escape + 'b';
    inline static const std::string Red = Escape + 'c';
    inline static const std::string LightPurple = Escape + 'd';
    inline static const std::string Yellow = Escape + 'e';
    inline static const std::string White = Escape + 'f';
    inline static const std::string MinecoinGold = Escape + 'g';
    inline static const std::string MaterialQuartz = Escape + 'h';
    inline static const std::string MaterialIron = Escape + 'i';
    inline static const std::string MaterialNetherite = Escape + 'j';
    inline static const std::string MaterialRedstone = Escape + 'm';
    inline static const std::string MaterialCopper = Escape + 'n';
    inline static const std::string MaterialGold = Escape + 'p';
    inline static const std::string MaterialEmerald = Escape + 'q';
    inline static const std::string MaterialDiamond = Escape + 's';
    inline static const std::string MaterialLapis = Escape + 't';
    inline static const std::string MaterialAmethyst = Escape + 'u';

    // Format code
    inline static const std::string Obfuscated = Escape + 'k';
    inline static const std::string Bold = Escape + 'l';
    inline static const std::string Italic = Escape + 'o';
    inline static const std::string Reset = Escape + 'r';
};  // namespace ColorFormat

}  // namespace endstone
```


