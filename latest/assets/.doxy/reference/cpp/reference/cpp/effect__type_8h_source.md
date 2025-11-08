

# File effect\_type.h

[**File List**](files.md) **>** [**effect**](dir_aaf7aa7272c9e0e62216386f1c427eea.md) **>** [**effect\_type.h**](effect__type_8h.md)

[Go to the documentation of this file](effect__type_8h.md)


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

#include "endstone/namespaced_key.h"

namespace endstone {
class EffectType {
    static std::string getEffectType(const std::string &key)
    {
        return NamespacedKey::minecraft(key);
    }

public:
    inline static const auto Speed = getEffectType("speed");

    inline static const auto Slowness = getEffectType("slowness");

    inline static const auto Haste = getEffectType("haste");

    inline static const auto MiningFatigue = getEffectType("mining_fatigue");

    inline static const auto Strength = getEffectType("strength");

    inline static const auto InstantHealth = getEffectType("instant_health");

    inline static const auto InstantDamage = getEffectType("instant_damage");

    inline static const auto JumpBoost = getEffectType("jump_boost");

    inline static const auto Nausea = getEffectType("nausea");

    inline static const auto Regeneration = getEffectType("regeneration");

    inline static const auto Resistance = getEffectType("resistance");

    inline static const auto FireResistance = getEffectType("fire_resistance");

    inline static const auto WaterBreathing = getEffectType("water_breathing");

    inline static const auto Invisibility = getEffectType("invisibility");

    inline static const auto Blindness = getEffectType("blindness");

    inline static const auto NightVision = getEffectType("night_vision");

    inline static const auto Hunger = getEffectType("hunger");

    inline static const auto Weakness = getEffectType("weakness");

    inline static const auto Poison = getEffectType("poison");

    inline static const auto Wither = getEffectType("wither");

    inline static const auto HealthBoost = getEffectType("health_boost");

    inline static const auto Absorption = getEffectType("absorption");

    inline static const auto Saturation = getEffectType("saturation");

    inline static const auto Levitation = getEffectType("levitation");

    inline static const auto FatalPoison = getEffectType("fatal_poison");

    inline static const auto ConduitPower = getEffectType("conduit_power");

    inline static const auto SlowFalling = getEffectType("slow_falling");

    inline static const auto BadOmen = getEffectType("bad_omen");

    inline static const auto HeroOfTheVillage = getEffectType("hero_of_the_village");

    inline static const auto Darkness = getEffectType("darkness");

    inline static const auto TrialOmen = getEffectType("trial_omen");

    inline static const auto WindCharged = getEffectType("wind_charged");

    inline static const auto Weaving = getEffectType("weaving");

    inline static const auto Oozing = getEffectType("oozing");

    inline static const auto Infested = getEffectType("infested");

    inline static const auto RaidOmen = getEffectType("raid_omen");
};
}  // namespace endstone
```


