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
/**
 * @brief All attribute types.
 */
class Attribute {
    static std::string getAttribute(const std::string &key)
    {
        return NamespacedKey::minecraft(key);
    }

public:
    /** @brief Health of an entity. */
    inline static const std::string Health = getAttribute("health");

    /** @brief Range at which an entity will follow others. */
    inline static const std::string FollowRange = getAttribute("follow_range");

    /** @brief Resistance of an entity to knockback. */
    inline static const std::string KnockbackResistance = getAttribute("knockback_resistance");

    /** @brief Movement speed of an entity. */
    inline static const std::string MovementSpeed = getAttribute("movement");

    /** @brief Movement speed of an entity underwater. */
    inline static const std::string UnderwaterMovementSpeed = getAttribute("underwater_movement");

    /** @brief Movement speed of an entity in lava. */
    inline static const std::string LavaMovementSpeed = getAttribute("lava_movement");

    /** @brief Attack damage of an entity. */
    inline static const std::string AttackDamage = getAttribute("attack_damage");

    /** @brief Absorption of an entity. */
    inline static const std::string Absorption = getAttribute("absorption");

    /** @brief Luck bonus of an entity. */
    inline static const std::string Luck = getAttribute("luck");

    /** @brief Strength with which an entity will jump. */
    inline static const std::string JumpStrength = getAttribute("jump_strength");

    /** @brief Hunger level of a player. */
    inline static const std::string PlayerHunger = getAttribute("player.hunger");

    /** @brief Saturation level of a player. */
    inline static const std::string PlayerSaturation = getAttribute("player.saturation");

    /** @brief Exhaustion level of a player. */
    inline static const std::string PlayerExhaustion = getAttribute("player.exhaustion");

    /** @brief Experience level of a player. */
    inline static const std::string PlayerLevel = getAttribute("player.level");

    /** @brief Progress toward the next experience level of a player. */
    inline static const std::string PlayerExperience = getAttribute("player.experience");

    /** @brief Chance of a zombie to spawn reinforcements. */
    inline static const std::string ZombieSpawnReinforcements = getAttribute("zombie.spawn_reinforcements");
};
}  // namespace endstone
