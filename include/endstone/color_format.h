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
 * All supported color and format codes.
 */
struct ColorFormat {
    /**
     * The escape character (§) that prefixes every color and format code.
     */
    inline static const std::string Escape = "§";

    // Color code

    /**
     * Black (§0).
     */
    inline static const std::string Black = Escape + '0';
    /**
     * Dark blue (§1).
     */
    inline static const std::string DarkBlue = Escape + '1';
    /**
     * Dark green (§2).
     */
    inline static const std::string DarkGreen = Escape + '2';
    /**
     * Dark aqua (§3).
     */
    inline static const std::string DarkAqua = Escape + '3';
    /**
     * Dark red (§4).
     */
    inline static const std::string DarkRed = Escape + '4';
    /**
     * Dark purple (§5).
     */
    inline static const std::string DarkPurple = Escape + '5';
    /**
     * Gold (§6).
     */
    inline static const std::string Gold = Escape + '6';
    /**
     * Gray (§7).
     */
    inline static const std::string Gray = Escape + '7';
    /**
     * Dark gray (§8).
     */
    inline static const std::string DarkGray = Escape + '8';
    /**
     * Blue (§9).
     */
    inline static const std::string Blue = Escape + '9';
    /**
     * Green (§a).
     */
    inline static const std::string Green = Escape + 'a';
    /**
     * Aqua (§b).
     */
    inline static const std::string Aqua = Escape + 'b';
    /**
     * Red (§c).
     */
    inline static const std::string Red = Escape + 'c';
    /**
     * Light purple (§d).
     */
    inline static const std::string LightPurple = Escape + 'd';
    /**
     * Yellow (§e).
     */
    inline static const std::string Yellow = Escape + 'e';
    /**
     * White (§f).
     */
    inline static const std::string White = Escape + 'f';
    /**
     * Minecoin gold (§g).
     */
    inline static const std::string MinecoinGold = Escape + 'g';
    /**
     * Quartz material color (§h).
     */
    inline static const std::string MaterialQuartz = Escape + 'h';
    /**
     * Iron material color (§i).
     */
    inline static const std::string MaterialIron = Escape + 'i';
    /**
     * Netherite material color (§j).
     */
    inline static const std::string MaterialNetherite = Escape + 'j';
    /**
     * Redstone material color (§m).
     */
    inline static const std::string MaterialRedstone = Escape + 'm';
    /**
     * Copper material color (§n).
     */
    inline static const std::string MaterialCopper = Escape + 'n';
    /**
     * Gold material color (§p).
     */
    inline static const std::string MaterialGold = Escape + 'p';
    /**
     * Emerald material color (§q).
     */
    inline static const std::string MaterialEmerald = Escape + 'q';
    /**
     * Diamond material color (§s).
     */
    inline static const std::string MaterialDiamond = Escape + 's';
    /**
     * Lapis material color (§t).
     */
    inline static const std::string MaterialLapis = Escape + 't';
    /**
     * Amethyst material color (§u).
     */
    inline static const std::string MaterialAmethyst = Escape + 'u';
    /**
     * Resin material color (§v).
     */
    inline static const std::string MaterialResin = Escape + 'v';

    // Format code

    /**
     * Makes the text obfuscated, randomly cycling through characters (§k).
     */
    inline static const std::string Obfuscated = Escape + 'k';
    /**
     * Makes the text bold (§l).
     */
    inline static const std::string Bold = Escape + 'l';
    /**
     * Makes the text italic (§o).
     */
    inline static const std::string Italic = Escape + 'o';
    /**
     * Resets all colors and formatting codes (§r).
     */
    inline static const std::string Reset = Escape + 'r';
};  // namespace ColorFormat

}  // namespace endstone
