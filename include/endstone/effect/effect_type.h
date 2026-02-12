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
/**
 * @brief All effect types.
 */
class EffectType {
public:
    /**
     * Increases movement speed.
     */
    static constexpr auto Speed = EffectId::minecraft("speed");

    /**
     * Decreases movement speed.
     */
    static constexpr auto Slowness = EffectId::minecraft("slowness");

    /**
     * Increases dig speed.
     */
    static constexpr auto Haste = EffectId::minecraft("haste");

    /**
     * Decreases dig speed.
     */
    static constexpr auto MiningFatigue = EffectId::minecraft("mining_fatigue");

    /**
     * Increases damage dealt.
     */
    static constexpr auto Strength = EffectId::minecraft("strength");

    /**
     * Heals an entity.
     */
    static constexpr auto InstantHealth = EffectId::minecraft("instant_health");

    /**
     * Hurts an entity.
     */
    static constexpr auto InstantDamage = EffectId::minecraft("instant_damage");

    /**
     * Increases jump height.
     */
    static constexpr auto JumpBoost = EffectId::minecraft("jump_boost");

    /**
     * Warps vision on the client.
     */
    static constexpr auto Nausea = EffectId::minecraft("nausea");

    /**
     * Regenerates health.
     */
    static constexpr auto Regeneration = EffectId::minecraft("regeneration");

    /**
     * Decreases damage dealt to an entity.
     */
    static constexpr auto Resistance = EffectId::minecraft("resistance");

    /**
     * Stops fire damage.
     */
    static constexpr auto FireResistance = EffectId::minecraft("fire_resistance");

    /**
     * Allows breathing underwater.
     */
    static constexpr auto WaterBreathing = EffectId::minecraft("water_breathing");

    /**
     * Grants invisibility.
     */
    static constexpr auto Invisibility = EffectId::minecraft("invisibility");

    /**
     * Blinds an entity.
     */
    static constexpr auto Blindness = EffectId::minecraft("blindness");

    /**
     * Allows an entity to see in the dark.
     */
    static constexpr auto NightVision = EffectId::minecraft("night_vision");

    /**
     * Increases hunger.
     */
    static constexpr auto Hunger = EffectId::minecraft("hunger");

    /**
     * Decreases damage dealt by an entity.
     */
    static constexpr auto Weakness = EffectId::minecraft("weakness");

    /**
     * Deals damage to an entity over time.
     */
    static constexpr auto Poison = EffectId::minecraft("poison");

    /**
     * Deals damage to an entity over time and gives the health to the
     * shooter.
     */
    static constexpr auto Wither = EffectId::minecraft("wither");

    /**
     * Increases the maximum health of an entity.
     */
    static constexpr auto HealthBoost = EffectId::minecraft("health_boost");

    /**
     * Increases the maximum health of an entity with health that cannot be
     * regenerated, but is refilled every 30 seconds.
     */
    static constexpr auto Absorption = EffectId::minecraft("absorption");

    /**
     * Increases the food level of an entity each tick.
     */
    static constexpr auto Saturation = EffectId::minecraft("saturation");

    /**
     * Causes the entity to float into the air.
     */
    static constexpr auto Levitation = EffectId::minecraft("levitation");

    /**
     * Inflicts damage to the entity over time, and can kill both undead and non-undead mobs, similar to Wither
     */
    static constexpr auto FatalPoison = EffectId::minecraft("fatal_poison");

    /**
     * Effects granted by a nearby conduit. Includes enhanced underwater abilities.
     */
    static constexpr auto ConduitPower = EffectId::minecraft("conduit_power");

    /**
     * Slows entity fall rate.
     */
    static constexpr auto SlowFalling = EffectId::minecraft("slow_falling");

    /**
     * Triggers an ominous event when the player enters a village or trial chambers.<br>
     * oof.
     */
    static constexpr auto BadOmen = EffectId::minecraft("bad_omen");

    /**
     * Reduces the cost of villager trades.<br>
     * \o/.
     */
    static constexpr auto HeroOfTheVillage = EffectId::minecraft("hero_of_the_village");

    /**
     * Causes the player's vision to dim occasionally.
     */
    static constexpr auto Darkness = EffectId::minecraft("darkness");

    /**
     * Causes trial spawners to become ominous.
     */
    static constexpr auto TrialOmen = EffectId::minecraft("trial_omen");

    /**
     * Emits a wind burst upon death.
     */
    static constexpr auto WindCharged = EffectId::minecraft("wind_charged");

    /**
     * Creates cobwebs upon death.
     */
    static constexpr auto Weaving = EffectId::minecraft("weaving");

    /**
     * Causes slimes to spawn upon death.
     */
    static constexpr auto Oozing = EffectId::minecraft("oozing");

    /**
     * Chance of spawning silverfish when hurt.
     */
    static constexpr auto Infested = EffectId::minecraft("infested");

    /**
     * Triggers a raid when a player enters a village.
     */
    static constexpr auto RaidOmen = EffectId::minecraft("raid_omen");

    /**
     * Prevents the rider's oxygen bar from depleting while riding a nautilus or zombie nautilus.
     */
    static constexpr auto BreathOfTheNautilus = EffectId::minecraft("breath_of_the_nautilus");
};
}  // namespace endstone
