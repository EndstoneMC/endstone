

# File potion\_type.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**potion**](dir_3aeea1eb013bc4968668ceaff3f9e2f0.md) **>** [**potion\_type.h**](potion__type_8h.md)

[Go to the documentation of this file](potion__type_8h.md)


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

#include "endstone/identifier.h"

namespace endstone {
using PotionId = Identifier<class PotionType>;
class PotionType {
    static constexpr std::string_view Minecraft = "minecraft:potion_type";

public:
    static constexpr auto Water = PotionId(Minecraft, "water");
    static constexpr auto Mundane = PotionId(Minecraft, "mundane");
    static constexpr auto LongMundane = PotionId(Minecraft, "long_mundane");
    static constexpr auto Thick = PotionId(Minecraft, "thick");
    static constexpr auto Awkward = PotionId(Minecraft, "awkward");
    static constexpr auto Nightvision = PotionId(Minecraft, "nightvision");
    static constexpr auto LongNightvision = PotionId(Minecraft, "long_nightvision");
    static constexpr auto Invisibility = PotionId(Minecraft, "invisibility");
    static constexpr auto LongInvisibility = PotionId(Minecraft, "long_invisibility");
    static constexpr auto Leaping = PotionId(Minecraft, "leaping");
    static constexpr auto LongLeaping = PotionId(Minecraft, "long_leaping");
    static constexpr auto StrongLeaping = PotionId(Minecraft, "strong_leaping");
    static constexpr auto FireResistance = PotionId(Minecraft, "fire_resistance");
    static constexpr auto LongFireResistance = PotionId(Minecraft, "long_fire_resistance");
    static constexpr auto Swiftness = PotionId(Minecraft, "swiftness");
    static constexpr auto LongSwiftness = PotionId(Minecraft, "long_swiftness");
    static constexpr auto StrongSwiftness = PotionId(Minecraft, "strong_swiftness");
    static constexpr auto Slowness = PotionId(Minecraft, "slowness");
    static constexpr auto LongSlowness = PotionId(Minecraft, "long_slowness");
    static constexpr auto StrongSlowness = PotionId(Minecraft, "strong_slowness");
    static constexpr auto WaterBreathing = PotionId(Minecraft, "water_breathing");
    static constexpr auto LongWaterBreathing = PotionId(Minecraft, "long_water_breathing");
    static constexpr auto Healing = PotionId(Minecraft, "healing");
    static constexpr auto StrongHealing = PotionId(Minecraft, "strong_healing");
    static constexpr auto Harming = PotionId(Minecraft, "harming");
    static constexpr auto StrongHarming = PotionId(Minecraft, "strong_harming");
    static constexpr auto Poison = PotionId(Minecraft, "poison");
    static constexpr auto LongPoison = PotionId(Minecraft, "long_poison");
    static constexpr auto StrongPoison = PotionId(Minecraft, "strong_poison");
    static constexpr auto Regeneration = PotionId(Minecraft, "regeneration");
    static constexpr auto LongRegeneration = PotionId(Minecraft, "long_regeneration");
    static constexpr auto StrongRegeneration = PotionId(Minecraft, "strong_regeneration");
    static constexpr auto Strength = PotionId(Minecraft, "strength");
    static constexpr auto LongStrength = PotionId(Minecraft, "long_strength");
    static constexpr auto StrongStrength = PotionId(Minecraft, "strong_strength");
    static constexpr auto Weakness = PotionId(Minecraft, "weakness");
    static constexpr auto LongWeakness = PotionId(Minecraft, "long_weakness");
    static constexpr auto Wither = PotionId(Minecraft, "wither");
    static constexpr auto TurtleMaster = PotionId(Minecraft, "turtle_master");
    static constexpr auto LongTurtleMaster = PotionId(Minecraft, "long_turtle_master");
    static constexpr auto StrongTurtleMaster = PotionId(Minecraft, "strong_turtle_master");
    static constexpr auto SlowFalling = PotionId(Minecraft, "slow_falling");
    static constexpr auto LongSlowFalling = PotionId(Minecraft, "long_slow_falling");
    static constexpr auto WindCharged = PotionId(Minecraft, "wind_charged");
    static constexpr auto Weaving = PotionId(Minecraft, "weaving");
    static constexpr auto Oozing = PotionId(Minecraft, "oozing");
    static constexpr auto Infested = PotionId(Minecraft, "infested");
};
}  // namespace endstone
```


