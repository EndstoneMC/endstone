

# File attribute.h

[**File List**](files.md) **>** [**attribute**](dir_5bd322557a2ad4dbfaf5ca599d780f7c.md) **>** [**attribute.h**](attribute_8h.md)

[Go to the documentation of this file](attribute_8h.md)


```C++
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

#include <functional>
#include <string>

#include "endstone/detail/endstone.h"
#include "endstone/registry.h"

namespace endstone {
class Attribute {
public:
    static constexpr std::string_view Health = "minecraft:health";

    static constexpr std::string_view FollowRange = "minecraft:follow_range";

    static constexpr std::string_view KnockbackResistance = "minecraft:knockback_resistance";

    static constexpr std::string_view MovementSpeed = "minecraft:movement";

    static constexpr std::string_view UnderwaterMovementSpeed = "minecraft:underwater_movement";

    static constexpr std::string_view LavaMovementSpeed = "minecraft:lava_movement";

    static constexpr std::string_view AttackDamage = "minecraft:attack_damage";

    static constexpr std::string_view Absorption = "minecraft:absorption";

    static constexpr std::string_view Luck = "minecraft:luck";

    static constexpr std::string_view JumpStrength = "minecraft:jump_strength";

    static constexpr std::string_view PlayerHunger = "minecraft:player.hunger";

    static constexpr std::string_view PlayerSaturation = "minecraft:player.saturation";

    static constexpr std::string_view PlayerExhaustion = "minecraft:player.exhaustion";

    static constexpr std::string_view PlayerLevel = "minecraft:player.level";

    static constexpr std::string_view PlayerExperience = "minecraft:player.experience";

    static constexpr std::string_view ZombieSpawnReinforcements = "minecraft:zombie.spawn_reinforcements";
};
}  // namespace endstone
```


