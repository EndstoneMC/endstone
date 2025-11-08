// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License";
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

namespace endstone {
/**
 * @brief All potion types.
 */
class PotionType {
public:
    static constexpr std::string_view Water = "minecraft:potion_type:water";
    static constexpr std::string_view Mundane = "minecraft:potion_type:mundane";
    static constexpr std::string_view LongMundane = "minecraft:potion_type:long_mundane";
    static constexpr std::string_view Thick = "minecraft:potion_type:thick";
    static constexpr std::string_view Awkward = "minecraft:potion_type:awkward";
    static constexpr std::string_view Nightvision = "minecraft:potion_type:nightvision";
    static constexpr std::string_view LongNightvision = "minecraft:potion_type:long_nightvision";
    static constexpr std::string_view Invisibility = "minecraft:potion_type:invisibility";
    static constexpr std::string_view LongInvisibility = "minecraft:potion_type:long_invisibility";
    static constexpr std::string_view Leaping = "minecraft:potion_type:leaping";
    static constexpr std::string_view LongLeaping = "minecraft:potion_type:long_leaping";
    static constexpr std::string_view StrongLeaping = "minecraft:potion_type:strong_leaping";
    static constexpr std::string_view FireResistance = "minecraft:potion_type:fire_resistance";
    static constexpr std::string_view LongFireResistance = "minecraft:potion_type:long_fire_resistance";
    static constexpr std::string_view Swiftness = "minecraft:potion_type:swiftness";
    static constexpr std::string_view LongSwiftness = "minecraft:potion_type:long_swiftness";
    static constexpr std::string_view StrongSwiftness = "minecraft:potion_type:strong_swiftness";
    static constexpr std::string_view Slowness = "minecraft:potion_type:slowness";
    static constexpr std::string_view LongSlowness = "minecraft:potion_type:long_slowness";
    static constexpr std::string_view StrongSlowness = "minecraft:potion_type:strong_slowness";
    static constexpr std::string_view WaterBreathing = "minecraft:potion_type:water_breathing";
    static constexpr std::string_view LongWaterBreathing = "minecraft:potion_type:long_water_breathing";
    static constexpr std::string_view Healing = "minecraft:potion_type:healing";
    static constexpr std::string_view StrongHealing = "minecraft:potion_type:strong_healing";
    static constexpr std::string_view Harming = "minecraft:potion_type:harming";
    static constexpr std::string_view StrongHarming = "minecraft:potion_type:strong_harming";
    static constexpr std::string_view Poison = "minecraft:potion_type:poison";
    static constexpr std::string_view LongPoison = "minecraft:potion_type:long_poison";
    static constexpr std::string_view StrongPoison = "minecraft:potion_type:strong_poison";
    static constexpr std::string_view Regeneration = "minecraft:potion_type:regeneration";
    static constexpr std::string_view LongRegeneration = "minecraft:potion_type:long_regeneration";
    static constexpr std::string_view StrongRegeneration = "minecraft:potion_type:strong_regeneration";
    static constexpr std::string_view Strength = "minecraft:potion_type:strength";
    static constexpr std::string_view LongStrength = "minecraft:potion_type:long_strength";
    static constexpr std::string_view StrongStrength = "minecraft:potion_type:strong_strength";
    static constexpr std::string_view Weakness = "minecraft:potion_type:weakness";
    static constexpr std::string_view LongWeakness = "minecraft:potion_type:long_weakness";
    static constexpr std::string_view Wither = "minecraft:potion_type:wither";
    static constexpr std::string_view TurtleMaster = "minecraft:potion_type:turtle_master";
    static constexpr std::string_view LongTurtleMaster = "minecraft:potion_type:long_turtle_master";
    static constexpr std::string_view StrongTurtleMaster = "minecraft:potion_type:strong_turtle_master";
    static constexpr std::string_view SlowFalling = "minecraft:potion_type:slow_falling";
    static constexpr std::string_view LongSlowFalling = "minecraft:potion_type:long_slow_falling";
    static constexpr std::string_view WindCharged = "minecraft:potion_type:wind_charged";
    static constexpr std::string_view Weaving = "minecraft:potion_type:weaving";
    static constexpr std::string_view Oozing = "minecraft:potion_type:oozing";
    static constexpr std::string_view Infested = "minecraft:potion_type:infested";
};
}  // namespace endstone
