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

namespace endstone {
/**
 * @brief All potion types.
 */
class PotionType {
    static std::string getPotionType(const std::string &key)
    {
        return NamespacedKey::minecraft("potion_type:" + key);
    }

public:
    inline static const std::string Water = getPotionType("water");
    inline static const std::string Mundane = getPotionType("mundane");
    inline static const std::string LongMundane = getPotionType("long_mundane");
    inline static const std::string Thick = getPotionType("thick");
    inline static const std::string Awkward = getPotionType("awkward");
    inline static const std::string Nightvision = getPotionType("nightvision");
    inline static const std::string LongNightvision = getPotionType("long_nightvision");
    inline static const std::string Invisibility = getPotionType("invisibility");
    inline static const std::string LongInvisibility = getPotionType("long_invisibility");
    inline static const std::string Leaping = getPotionType("leaping");
    inline static const std::string LongLeaping = getPotionType("long_leaping");
    inline static const std::string StrongLeaping = getPotionType("strong_leaping");
    inline static const std::string FireResistance = getPotionType("fire_resistance");
    inline static const std::string LongFireResistance = getPotionType("long_fire_resistance");
    inline static const std::string Swiftness = getPotionType("swiftness");
    inline static const std::string LongSwiftness = getPotionType("long_swiftness");
    inline static const std::string StrongSwiftness = getPotionType("strong_swiftness");
    inline static const std::string Slowness = getPotionType("slowness");
    inline static const std::string LongSlowness = getPotionType("long_slowness");
    inline static const std::string StrongSlowness = getPotionType("strong_slowness");
    inline static const std::string WaterBreathing = getPotionType("water_breathing");
    inline static const std::string LongWaterBreathing = getPotionType("long_water_breathing");
    inline static const std::string Healing = getPotionType("healing");
    inline static const std::string StrongHealing = getPotionType("strong_healing");
    inline static const std::string Harming = getPotionType("harming");
    inline static const std::string StrongHarming = getPotionType("strong_harming");
    inline static const std::string Poison = getPotionType("poison");
    inline static const std::string LongPoison = getPotionType("long_poison");
    inline static const std::string StrongPoison = getPotionType("strong_poison");
    inline static const std::string Regeneration = getPotionType("regeneration");
    inline static const std::string LongRegeneration = getPotionType("long_regeneration");
    inline static const std::string StrongRegeneration = getPotionType("strong_regeneration");
    inline static const std::string Strength = getPotionType("strength");
    inline static const std::string LongStrength = getPotionType("long_strength");
    inline static const std::string StrongStrength = getPotionType("strong_strength");
    inline static const std::string Weakness = getPotionType("weakness");
    inline static const std::string LongWeakness = getPotionType("long_weakness");
    inline static const std::string Wither = getPotionType("wither");
    inline static const std::string TurtleMaster = getPotionType("turtle_master");
    inline static const std::string LongTurtleMaster = getPotionType("long_turtle_master");
    inline static const std::string StrongTurtleMaster = getPotionType("strong_turtle_master");
    inline static const std::string SlowFalling = getPotionType("slow_falling");
    inline static const std::string LongSlowFalling = getPotionType("long_slow_falling");
    inline static const std::string WindCharged = getPotionType("wind_charged");
    inline static const std::string Weaving = getPotionType("weaving");
    inline static const std::string Oozing = getPotionType("oozing");
    inline static const std::string Infested = getPotionType("infested");
};
}  // namespace endstone
