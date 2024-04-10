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

/**
 * @brief All supported color and format codes
 */
struct ColorFormat {
    // Escape
    inline static const std::string ESCAPE = "§";

    // Color code
    inline static const std::string BLACK = ESCAPE + '0';
    inline static const std::string DARK_BLUE = ESCAPE + '1';
    inline static const std::string DARK_GREEN = ESCAPE + '2';
    inline static const std::string DARK_AQUA = ESCAPE + '3';
    inline static const std::string DARK_RED = ESCAPE + '4';
    inline static const std::string DARK_PURPLE = ESCAPE + '5';
    inline static const std::string GOLD = ESCAPE + '6';
    inline static const std::string GRAY = ESCAPE + '7';
    inline static const std::string DARK_GRAY = ESCAPE + '8';
    inline static const std::string BLUE = ESCAPE + '9';
    inline static const std::string GREEN = ESCAPE + 'a';
    inline static const std::string AQUA = ESCAPE + 'b';
    inline static const std::string RED = ESCAPE + 'c';
    inline static const std::string LIGHT_PURPLE = ESCAPE + 'd';
    inline static const std::string YELLOW = ESCAPE + 'e';
    inline static const std::string WHITE = ESCAPE + 'f';
    inline static const std::string MINECOIN_GOLD = ESCAPE + 'g';
    inline static const std::string MATERIAL_QUARTZ = ESCAPE + 'h';
    inline static const std::string MATERIAL_IRON = ESCAPE + 'i';
    inline static const std::string MATERIAL_NETHERITE = ESCAPE + 'j';
    inline static const std::string MATERIAL_REDSTONE = ESCAPE + 'm';
    inline static const std::string MATERIAL_COPPER = ESCAPE + 'n';
    inline static const std::string MATERIAL_GOLD = ESCAPE + 'p';
    inline static const std::string MATERIAL_EMERALD = ESCAPE + 'q';
    inline static const std::string MATERIAL_DIAMOND = ESCAPE + 's';
    inline static const std::string MATERIAL_LAPIS = ESCAPE + 't';
    inline static const std::string MATERIAL_AMETHYST = ESCAPE + 'u';

    // Format code
    inline static const std::string OBFUSCATED = ESCAPE + 'k';
    inline static const std::string BOLD = ESCAPE + 'l';
    inline static const std::string ITALIC = ESCAPE + 'o';
    inline static const std::string RESET = ESCAPE + 'r';
};

}  // namespace endstone
