

# File attribute.h

[**File List**](files.md) **>** [**attribute**](dir_5bd322557a2ad4dbfaf5ca599d780f7c.md) **>** [**attribute.h**](attribute_8h.md)

[Go to the documentation of this file](attribute_8h.md)


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
using AttributeId = Identifier<class Attribute>;
class Attribute {
public:
    static constexpr auto Health = AttributeId::minecraft("health");

    static constexpr auto FollowRange = AttributeId::minecraft("follow_range");

    static constexpr auto KnockbackResistance = AttributeId::minecraft("knockback_resistance");

    static constexpr auto MovementSpeed = AttributeId::minecraft("movement");

    static constexpr auto UnderwaterMovementSpeed = AttributeId::minecraft("underwater_movement");

    static constexpr auto LavaMovementSpeed = AttributeId::minecraft("lava_movement");

    static constexpr auto AttackDamage = AttributeId::minecraft("attack_damage");

    static constexpr auto Absorption = AttributeId::minecraft("absorption");

    static constexpr auto Luck = AttributeId::minecraft("luck");

    static constexpr auto JumpStrength = AttributeId::minecraft("jump_strength");

    static constexpr auto PlayerHunger = AttributeId::minecraft("player.hunger");

    static constexpr auto PlayerSaturation = AttributeId::minecraft("player.saturation");

    static constexpr auto PlayerExhaustion = AttributeId::minecraft("player.exhaustion");

    static constexpr auto PlayerLevel = AttributeId::minecraft("player.level");

    static constexpr auto PlayerExperience = AttributeId::minecraft("player.experience");

    static constexpr auto ZombieSpawnReinforcements = AttributeId::minecraft("zombie.spawn_reinforcements");
};
}  // namespace endstone
```


