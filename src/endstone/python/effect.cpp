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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {
void init_effect(py::module_ &m)
{
    py::class_<EffectType>(m, "EffectType", "All effect types.")
        .def_property_readonly_static(
            "SPEED", [](const py::object &) { return EffectType::Speed; }, "Increases movement speed.")
        .def_property_readonly_static(
            "SLOWNESS", [](const py::object &) { return EffectType::Slowness; }, "Decreases movement speed.")
        .def_property_readonly_static(
            "HASTE", [](const py::object &) { return EffectType::Haste; }, "Increases dig speed.")
        .def_property_readonly_static(
            "MINING_FATIGUE", [](const py::object &) { return EffectType::MiningFatigue; }, "Decreases dig speed.")
        .def_property_readonly_static(
            "STRENGTH", [](const py::object &) { return EffectType::Strength; }, "Increases damage dealt.")
        .def_property_readonly_static(
            "INSTANT_HEALTH", [](const py::object &) { return EffectType::InstantHealth; }, "Heals an entity.")
        .def_property_readonly_static(
            "INSTANT_DAMAGE", [](const py::object &) { return EffectType::InstantDamage; }, "Hurts an entity.")
        .def_property_readonly_static(
            "JUMP_BOOST", [](const py::object &) { return EffectType::JumpBoost; }, "Increases jump height.")
        .def_property_readonly_static(
            "NAUSEA", [](const py::object &) { return EffectType::Nausea; }, "Warps vision on the client.")
        .def_property_readonly_static(
            "REGENERATION", [](const py::object &) { return EffectType::Regeneration; }, "Regenerates health.")
        .def_property_readonly_static(
            "RESISTANCE", [](const py::object &) { return EffectType::Resistance; },
            "Decreases damage dealt to an entity.")
        .def_property_readonly_static(
            "FIRE_RESISTANCE", [](const py::object &) { return EffectType::FireResistance; }, "Stops fire damage.")
        .def_property_readonly_static(
            "WATER_BREATHING", [](const py::object &) { return EffectType::WaterBreathing; },
            "Allows breathing underwater.")
        .def_property_readonly_static(
            "INVISIBILITY", [](const py::object &) { return EffectType::Invisibility; }, "Grants invisibility.")
        .def_property_readonly_static(
            "BLINDNESS", [](const py::object &) { return EffectType::Blindness; }, "Blinds an entity.")
        .def_property_readonly_static(
            "NIGHT_VISION", [](const py::object &) { return EffectType::NightVision; },
            "Allows an entity to see in the dark.")
        .def_property_readonly_static(
            "HUNGER", [](const py::object &) { return EffectType::Hunger; }, "Increases hunger.")
        .def_property_readonly_static(
            "WEAKNESS", [](const py::object &) { return EffectType::Weakness; },
            "Decreases damage dealt by an entity.")
        .def_property_readonly_static(
            "POISON", [](const py::object &) { return EffectType::Poison; },
            "Deals damage to an entity over time.")
        .def_property_readonly_static(
            "WITHER", [](const py::object &) { return EffectType::Wither; },
            "Deals damage to an entity over time and gives the health to the shooter.")
        .def_property_readonly_static(
            "HEALTH_BOOST", [](const py::object &) { return EffectType::HealthBoost; },
            "Increases the maximum health of an entity.")
        .def_property_readonly_static(
            "ABSORPTION", [](const py::object &) { return EffectType::Absorption; },
            "Increases the maximum health of an entity with health that cannot be "
            "regenerated, but is refilled every 30 seconds.")
        .def_property_readonly_static(
            "SATURATION", [](const py::object &) { return EffectType::Saturation; },
            "Increases the food level of an entity each tick.")
        .def_property_readonly_static(
            "LEVITATION", [](const py::object &) { return EffectType::Levitation; },
            "Causes the entity to float into the air.")
        .def_property_readonly_static(
            "FATAL_POISON", [](const py::object &) { return EffectType::FatalPoison; },
            "Inflicts damage to the entity over time, and can kill both undead and non-undead mobs, similar to Wither.")
        .def_property_readonly_static(
            "CONDUIT_POWER", [](const py::object &) { return EffectType::ConduitPower; },
            "Effects granted by a nearby conduit. Includes enhanced underwater abilities.")
        .def_property_readonly_static(
            "SLOW_FALLING", [](const py::object &) { return EffectType::SlowFalling; }, "Slows entity fall rate.")
        .def_property_readonly_static(
            "BAD_OMEN", [](const py::object &) { return EffectType::BadOmen; },
            "Triggers an ominous event when the player enters a village or trial chambers.")
        .def_property_readonly_static(
            "HERO_OF_THE_VILLAGE", [](const py::object &) { return EffectType::HeroOfTheVillage; },
            "Reduces the cost of villager trades.")
        .def_property_readonly_static(
            "DARKNESS", [](const py::object &) { return EffectType::Darkness; },
            "Causes the player's vision to dim occasionally.")
        .def_property_readonly_static(
            "TRIAL_OMEN", [](const py::object &) { return EffectType::TrialOmen; },
            "Causes trial spawners to become ominous.")
        .def_property_readonly_static(
            "WIND_CHARGED", [](const py::object &) { return EffectType::WindCharged; },
            "Emits a wind burst upon death.")
        .def_property_readonly_static(
            "WEAVING", [](const py::object &) { return EffectType::Weaving; }, "Creates cobwebs upon death.")
        .def_property_readonly_static(
            "OOZING", [](const py::object &) { return EffectType::Oozing; }, "Causes slimes to spawn upon death.")
        .def_property_readonly_static(
            "INFESTED", [](const py::object &) { return EffectType::Infested; },
            "Chance of spawning silverfish when hurt.")
        .def_property_readonly_static(
            "RAID_OMEN", [](const py::object &) { return EffectType::RaidOmen; },
            "Triggers a raid when a player enters a village.")
        .def_property_readonly_static(
            "BREATH_OF_THE_NAUTILUS", [](const py::object &) { return EffectType::BreathOfTheNautilus; },
            "Prevents the rider's oxygen bar from depleting while riding a nautilus or zombie nautilus.");
}
}  // namespace endstone::python
