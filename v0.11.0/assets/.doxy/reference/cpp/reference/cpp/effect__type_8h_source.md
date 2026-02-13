

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

#include "endstone/identifier.h"

namespace endstone {
using EffectId = Identifier<class EffectType>;
class EffectType {
public:
    static constexpr auto Speed = EffectId::minecraft("speed");

    static constexpr auto Slowness = EffectId::minecraft("slowness");

    static constexpr auto Haste = EffectId::minecraft("haste");

    static constexpr auto MiningFatigue = EffectId::minecraft("mining_fatigue");

    static constexpr auto Strength = EffectId::minecraft("strength");

    static constexpr auto InstantHealth = EffectId::minecraft("instant_health");

    static constexpr auto InstantDamage = EffectId::minecraft("instant_damage");

    static constexpr auto JumpBoost = EffectId::minecraft("jump_boost");

    static constexpr auto Nausea = EffectId::minecraft("nausea");

    static constexpr auto Regeneration = EffectId::minecraft("regeneration");

    static constexpr auto Resistance = EffectId::minecraft("resistance");

    static constexpr auto FireResistance = EffectId::minecraft("fire_resistance");

    static constexpr auto WaterBreathing = EffectId::minecraft("water_breathing");

    static constexpr auto Invisibility = EffectId::minecraft("invisibility");

    static constexpr auto Blindness = EffectId::minecraft("blindness");

    static constexpr auto NightVision = EffectId::minecraft("night_vision");

    static constexpr auto Hunger = EffectId::minecraft("hunger");

    static constexpr auto Weakness = EffectId::minecraft("weakness");

    static constexpr auto Poison = EffectId::minecraft("poison");

    static constexpr auto Wither = EffectId::minecraft("wither");

    static constexpr auto HealthBoost = EffectId::minecraft("health_boost");

    static constexpr auto Absorption = EffectId::minecraft("absorption");

    static constexpr auto Saturation = EffectId::minecraft("saturation");

    static constexpr auto Levitation = EffectId::minecraft("levitation");

    static constexpr auto FatalPoison = EffectId::minecraft("fatal_poison");

    static constexpr auto ConduitPower = EffectId::minecraft("conduit_power");

    static constexpr auto SlowFalling = EffectId::minecraft("slow_falling");

    static constexpr auto BadOmen = EffectId::minecraft("bad_omen");

    static constexpr auto HeroOfTheVillage = EffectId::minecraft("hero_of_the_village");

    static constexpr auto Darkness = EffectId::minecraft("darkness");

    static constexpr auto TrialOmen = EffectId::minecraft("trial_omen");

    static constexpr auto WindCharged = EffectId::minecraft("wind_charged");

    static constexpr auto Weaving = EffectId::minecraft("weaving");

    static constexpr auto Oozing = EffectId::minecraft("oozing");

    static constexpr auto Infested = EffectId::minecraft("infested");

    static constexpr auto RaidOmen = EffectId::minecraft("raid_omen");

    static constexpr auto BreathOfTheNautilus = EffectId::minecraft("breath_of_the_nautilus");
};
}  // namespace endstone
```


