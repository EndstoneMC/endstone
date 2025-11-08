

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

#include <string>

#include "endstone/namespaced_key.h"

namespace endstone {
class Attribute {
    static std::string getAttribute(const std::string &key)
    {
        return NamespacedKey::minecraft(key);
    }

public:
    inline static const std::string Health = getAttribute("health");

    inline static const std::string FollowRange = getAttribute("follow_range");

    inline static const std::string KnockbackResistance = getAttribute("knockback_resistance");

    inline static const std::string MovementSpeed = getAttribute("movement");

    inline static const std::string UnderwaterMovementSpeed = getAttribute("underwater_movement");

    inline static const std::string LavaMovementSpeed = getAttribute("lava_movement");

    inline static const std::string AttackDamage = getAttribute("attack_damage");

    inline static const std::string Absorption = getAttribute("absorption");

    inline static const std::string Luck = getAttribute("luck");

    inline static const std::string JumpStrength = getAttribute("jump_strength");

    inline static const std::string PlayerHunger = getAttribute("player.hunger");

    inline static const std::string PlayerSaturation = getAttribute("player.saturation");

    inline static const std::string PlayerExhaustion = getAttribute("player.exhaustion");

    inline static const std::string PlayerLevel = getAttribute("player.level");

    inline static const std::string PlayerExperience = getAttribute("player.experience");

    inline static const std::string ZombieSpawnReinforcements = getAttribute("zombie.spawn_reinforcements");
};
}  // namespace endstone
```


