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
/**
 * @brief All attribute types.
 */
class Attribute {
public:
    /** @brief Health of an entity. */
    static constexpr std::string_view Health = "minecraft:health";

    /** @brief Range at which an entity will follow others. */
    static constexpr std::string_view FollowRange = "minecraft:follow_range";

    /** @brief Resistance of an entity to knockback. */
    static constexpr std::string_view KnockbackResistance = "minecraft:knockback_resistance";

    /** @brief Movement speed of an entity. */
    static constexpr std::string_view MovementSpeed = "minecraft:movement";

    /** @brief Movement speed of an entity underwater. */
    static constexpr std::string_view UnderwaterMovementSpeed = "minecraft:underwater_movement";

    /** @brief Movement speed of an entity in lava. */
    static constexpr std::string_view LavaMovementSpeed = "minecraft:lava_movement";

    /** @brief Attack damage of an entity. */
    static constexpr std::string_view AttackDamage = "minecraft:attack_damage";

    /** @brief Absorption of an entity. */
    static constexpr std::string_view Absorption = "minecraft:absorption";

    /** @brief Luck bonus of an entity. */
    static constexpr std::string_view Luck = "minecraft:luck";

    /** @brief Strength with which an entity will jump. */
    static constexpr std::string_view JumpStrength = "minecraft:jump_strength";

    /** @brief Hunger level of a player. */
    static constexpr std::string_view PlayerHunger = "minecraft:player.hunger";

    /** @brief Saturation level of a player. */
    static constexpr std::string_view PlayerSaturation = "minecraft:player.saturation";

    /** @brief Exhaustion level of a player. */
    static constexpr std::string_view PlayerExhaustion = "minecraft:player.exhaustion";

    /** @brief Experience level of a player. */
    static constexpr std::string_view PlayerLevel = "minecraft:player.level";

    /** @brief Progress toward the next experience level of a player. */
    static constexpr std::string_view PlayerExperience = "minecraft:player.experience";

    /** @brief Chance of a zombie to spawn reinforcements. */
    static constexpr std::string_view ZombieSpawnReinforcements = "minecraft:zombie.spawn_reinforcements";
};
}  // namespace endstone
