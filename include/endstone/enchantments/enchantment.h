// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <cstdint>
namespace endstone {
class Enchantment {
public:
    constexpr static std::int16_t PROTECTION = 0;
    constexpr static std::int16_t FIRE_PROTECTION = 1;
    constexpr static std::int16_t FEATHER_FALLING = 2;
    constexpr static std::int16_t BLAST_PROTECTION = 3;
    constexpr static std::int16_t PROJECTILE_PROTECTION = 4;
    constexpr static std::int16_t THORNS = 5;
    constexpr static std::int16_t RESPIRATION = 6;
    constexpr static std::int16_t DEPTH_STRIDER = 7;
    constexpr static std::int16_t AQUA_AFFINITY = 8;
    constexpr static std::int16_t SHARPNESS = 9;
    constexpr static std::int16_t SMITE = 10;
    constexpr static std::int16_t BANE_OF_ARTHROPODS = 11;
    constexpr static std::int16_t KNOCKBACK = 12;
    constexpr static std::int16_t FIRE_ASPECT = 13;
    constexpr static std::int16_t LOOTING = 14;
    constexpr static std::int16_t EFFICIENCY = 15;
    constexpr static std::int16_t SILK_TOUCH = 16;
    constexpr static std::int16_t UNBREAKING = 17;
    constexpr static std::int16_t FORTUNE = 18;
    constexpr static std::int16_t POWER = 19;
    constexpr static std::int16_t PUNCH = 20;
    constexpr static std::int16_t FLAME = 21;
    constexpr static std::int16_t INFINITY = 22;
    constexpr static std::int16_t LUCK_OF_THE_SEA = 23;
    constexpr static std::int16_t LURE = 24;
    constexpr static std::int16_t FROST_WALKER = 25;
    constexpr static std::int16_t MENDING = 26;
    constexpr static std::int16_t BINDING_CURSE = 27;
    constexpr static std::int16_t VANISHING_CURSE = 28;
    constexpr static std::int16_t IMPALING = 29;
    constexpr static std::int16_t RIPTIDE = 30;
    constexpr static std::int16_t LOYALTY = 31;
    constexpr static std::int16_t CHANNELING = 32;
    constexpr static std::int16_t MULTISHOT = 33;
    constexpr static std::int16_t PIERCING = 34;
    constexpr static std::int16_t QUICK_CHARGE = 35;
    constexpr static std::int16_t SOUL_SPEED = 36;
    constexpr static std::int16_t SWIFT_SNEAK = 37;
    constexpr static std::int16_t WIND_BURST = 38;
    constexpr static std::int16_t DENSITY = 39;
    constexpr static std::int16_t BREACH = 40;
};
}  // namespace endstone
