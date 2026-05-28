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
/**
 * All attribute types.
 */
class Attribute {
public:
    /** Health of an entity. */
    static constexpr auto Health = AttributeId::minecraft("health");

    /** Range at which an entity will follow others. */
    static constexpr auto FollowRange = AttributeId::minecraft("follow_range");

    /** Resistance of an entity to knockback. */
    static constexpr auto KnockbackResistance = AttributeId::minecraft("knockback_resistance");

    /** Movement speed of an entity. */
    static constexpr auto MovementSpeed = AttributeId::minecraft("movement");

    /** Movement speed of an entity underwater. */
    static constexpr auto UnderwaterMovementSpeed = AttributeId::minecraft("underwater_movement");

    /** Movement speed of an entity in lava. */
    static constexpr auto LavaMovementSpeed = AttributeId::minecraft("lava_movement");

    /** Attack damage of an entity. */
    static constexpr auto AttackDamage = AttributeId::minecraft("attack_damage");

    /** Absorption of an entity. */
    static constexpr auto Absorption = AttributeId::minecraft("absorption");

    /** Luck bonus of an entity. */
    static constexpr auto Luck = AttributeId::minecraft("luck");

    /** Strength with which an entity will jump. */
    static constexpr auto JumpStrength = AttributeId::minecraft("jump_strength");

    /** Hunger level of a player. */
    static constexpr auto PlayerHunger = AttributeId::minecraft("player.hunger");

    /** Saturation level of a player. */
    static constexpr auto PlayerSaturation = AttributeId::minecraft("player.saturation");

    /** Exhaustion level of a player. */
    static constexpr auto PlayerExhaustion = AttributeId::minecraft("player.exhaustion");

    /** Experience level of a player. */
    static constexpr auto PlayerLevel = AttributeId::minecraft("player.level");

    /** Progress toward the next experience level of a player. */
    static constexpr auto PlayerExperience = AttributeId::minecraft("player.experience");

    /** Chance of a zombie to spawn reinforcements. */
    static constexpr auto ZombieSpawnReinforcements = AttributeId::minecraft("zombie.spawn_reinforcements");
};
}  // namespace endstone
