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
 * @brief All effect types.
 */
class EffectType {
    static std::string getEffectType(const std::string &key)
    {
        return NamespacedKey::minecraft(key);
    }

public:
    /**
     * Increases movement speed.
     */
    inline static const auto Speed = getEffectType("speed");

    /**
     * Decreases movement speed.
     */
    inline static const auto Slowness = getEffectType("slowness");

    /**
     * Increases dig speed.
     */
    inline static const auto Haste = getEffectType("haste");

    /**
     * Decreases dig speed.
     */
    inline static const auto MiningFatigue = getEffectType("mining_fatigue");

    /**
     * Increases damage dealt.
     */
    inline static const auto Strength = getEffectType("strength");

    /**
     * Heals an entity.
     */
    inline static const auto InstantHealth = getEffectType("instant_health");

    /**
     * Hurts an entity.
     */
    inline static const auto InstantDamage = getEffectType("instant_damage");

    /**
     * Increases jump height.
     */
    inline static const auto JumpBoost = getEffectType("jump_boost");

    /**
     * Warps vision on the client.
     */
    inline static const auto Nausea = getEffectType("nausea");

    /**
     * Regenerates health.
     */
    inline static const auto Regeneration = getEffectType("regeneration");

    /**
     * Decreases damage dealt to an entity.
     */
    inline static const auto Resistance = getEffectType("resistance");

    /**
     * Stops fire damage.
     */
    inline static const auto FireResistance = getEffectType("fire_resistance");

    /**
     * Allows breathing underwater.
     */
    inline static const auto WaterBreathing = getEffectType("water_breathing");

    /**
     * Grants invisibility.
     */
    inline static const auto Invisibility = getEffectType("invisibility");

    /**
     * Blinds an entity.
     */
    inline static const auto Blindness = getEffectType("blindness");

    /**
     * Allows an entity to see in the dark.
     */
    inline static const auto NightVision = getEffectType("night_vision");

    /**
     * Increases hunger.
     */
    inline static const auto Hunger = getEffectType("hunger");

    /**
     * Decreases damage dealt by an entity.
     */
    inline static const auto Weakness = getEffectType("weakness");

    /**
     * Deals damage to an entity over time.
     */
    inline static const auto Poison = getEffectType("poison");

    /**
     * Deals damage to an entity over time and gives the health to the
     * shooter.
     */
    inline static const auto Wither = getEffectType("wither");

    /**
     * Increases the maximum health of an entity.
     */
    inline static const auto HealthBoost = getEffectType("health_boost");

    /**
     * Increases the maximum health of an entity with health that cannot be
     * regenerated, but is refilled every 30 seconds.
     */
    inline static const auto Absorption = getEffectType("absorption");

    /**
     * Increases the food level of an entity each tick.
     */
    inline static const auto Saturation = getEffectType("saturation");

    /**
     * Causes the entity to float into the air.
     */
    inline static const auto Levitation = getEffectType("levitation");

    /**
     * Inflicts damage to the entity over time, and can kill both undead and non-undead mobs, similar to Wither
     */
    inline static const auto FatalPoison = getEffectType("fatal_poison");

    /**
     * Effects granted by a nearby conduit. Includes enhanced underwater abilities.
     */
    inline static const auto ConduitPower = getEffectType("conduit_power");

    /**
     * Slows entity fall rate.
     */
    inline static const auto SlowFalling = getEffectType("slow_falling");

    /**
     * Triggers an ominous event when the player enters a village or trial chambers.<br>
     * oof.
     */
    inline static const auto BadOmen = getEffectType("bad_omen");

    /**
     * Reduces the cost of villager trades.<br>
     * \o/.
     */
    inline static const auto HeroOfTheVillage = getEffectType("hero_of_the_village");

    /**
     * Causes the player's vision to dim occasionally.
     */
    inline static const auto Darkness = getEffectType("darkness");

    /**
     * Causes trial spawners to become ominous.
     */
    inline static const auto TrialOmen = getEffectType("trial_omen");

    /**
     * Emits a wind burst upon death.
     */
    inline static const auto WindCharged = getEffectType("wind_charged");

    /**
     * Creates cobwebs upon death.
     */
    inline static const auto Weaving = getEffectType("weaving");

    /**
     * Causes slimes to spawn upon death.
     */
    inline static const auto Oozing = getEffectType("oozing");

    /**
     * Chance of spawning silverfish when hurt.
     */
    inline static const auto Infested = getEffectType("infested");

    /**
     * Triggers a raid when a player enters a village.
     */
    inline static const auto RaidOmen = getEffectType("raid_omen");
};
}  // namespace endstone
