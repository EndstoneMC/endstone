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
        .def_property_readonly_static("SPEED", [](const py::object &) { return EffectType::Speed; })
        .def_property_readonly_static("SLOWNESS", [](const py::object &) { return EffectType::Slowness; })
        .def_property_readonly_static("HASTE", [](const py::object &) { return EffectType::Haste; })
        .def_property_readonly_static("MINING_FATIGUE", [](const py::object &) { return EffectType::MiningFatigue; })
        .def_property_readonly_static("STRENGTH", [](const py::object &) { return EffectType::Strength; })
        .def_property_readonly_static("INSTANT_HEALTH", [](const py::object &) { return EffectType::InstantHealth; })
        .def_property_readonly_static("INSTANT_DAMAGE", [](const py::object &) { return EffectType::InstantDamage; })
        .def_property_readonly_static("JUMP_BOOST", [](const py::object &) { return EffectType::JumpBoost; })
        .def_property_readonly_static("NAUSEA", [](const py::object &) { return EffectType::Nausea; })
        .def_property_readonly_static("REGENERATION", [](const py::object &) { return EffectType::Regeneration; })
        .def_property_readonly_static("RESISTANCE", [](const py::object &) { return EffectType::Resistance; })
        .def_property_readonly_static("FIRE_RESISTANCE", [](const py::object &) { return EffectType::FireResistance; })
        .def_property_readonly_static("WATER_BREATHING", [](const py::object &) { return EffectType::WaterBreathing; })
        .def_property_readonly_static("INVISIBILITY", [](const py::object &) { return EffectType::Invisibility; })
        .def_property_readonly_static("BLINDNESS", [](const py::object &) { return EffectType::Blindness; })
        .def_property_readonly_static("NIGHT_VISION", [](const py::object &) { return EffectType::NightVision; })
        .def_property_readonly_static("HUNGER", [](const py::object &) { return EffectType::Hunger; })
        .def_property_readonly_static("WEAKNESS", [](const py::object &) { return EffectType::Weakness; })
        .def_property_readonly_static("POISON", [](const py::object &) { return EffectType::Poison; })
        .def_property_readonly_static("WITHER", [](const py::object &) { return EffectType::Wither; })
        .def_property_readonly_static("HEALTH_BOOST", [](const py::object &) { return EffectType::HealthBoost; })
        .def_property_readonly_static("ABSORPTION", [](const py::object &) { return EffectType::Absorption; })
        .def_property_readonly_static("SATURATION", [](const py::object &) { return EffectType::Saturation; })
        .def_property_readonly_static("LEVITATION", [](const py::object &) { return EffectType::Levitation; })
        .def_property_readonly_static("FATAL_POISON", [](const py::object &) { return EffectType::FatalPoison; })
        .def_property_readonly_static("CONDUIT_POWER", [](const py::object &) { return EffectType::ConduitPower; })
        .def_property_readonly_static("SLOW_FALLING", [](const py::object &) { return EffectType::SlowFalling; })
        .def_property_readonly_static("BAD_OMEN", [](const py::object &) { return EffectType::BadOmen; })
        .def_property_readonly_static("HERO_OF_THE_VILLAGE",
                                      [](const py::object &) { return EffectType::HeroOfTheVillage; })
        .def_property_readonly_static("DARKNESS", [](const py::object &) { return EffectType::Darkness; })
        .def_property_readonly_static("TRIAL_OMEN", [](const py::object &) { return EffectType::TrialOmen; })
        .def_property_readonly_static("WIND_CHARGED", [](const py::object &) { return EffectType::WindCharged; })
        .def_property_readonly_static("WEAVING", [](const py::object &) { return EffectType::Weaving; })
        .def_property_readonly_static("OOZING", [](const py::object &) { return EffectType::Oozing; })
        .def_property_readonly_static("INFESTED", [](const py::object &) { return EffectType::Infested; })
        .def_property_readonly_static("RAID_OMEN", [](const py::object &) { return EffectType::RaidOmen; })
        .def_property_readonly_static("BREATH_OF_THE_NAUTILUS",
                                      [](const py::object &) { return EffectType::BreathOfTheNautilus; });
}
}  // namespace endstone::python
