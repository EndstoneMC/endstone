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
void init_potion(py::module_ &m)
{
    py::class_<PotionType>(m, "PotionType", "All potion types.")
        .def_property_readonly_static("WATER", [](const py::object &) { return PotionType::Water; })
        .def_property_readonly_static("MUNDANE", [](const py::object &) { return PotionType::Mundane; })
        .def_property_readonly_static("LONG_MUNDANE", [](const py::object &) { return PotionType::LongMundane; })
        .def_property_readonly_static("THICK", [](const py::object &) { return PotionType::Thick; })
        .def_property_readonly_static("AWKWARD", [](const py::object &) { return PotionType::Awkward; })
        .def_property_readonly_static("NIGHTVISION", [](const py::object &) { return PotionType::Nightvision; })
        .def_property_readonly_static("LONG_NIGHTVISION",
                                      [](const py::object &) { return PotionType::LongNightvision; })
        .def_property_readonly_static("INVISIBILITY", [](const py::object &) { return PotionType::Invisibility; })
        .def_property_readonly_static("LONG_INVISIBILITY",
                                      [](const py::object &) { return PotionType::LongInvisibility; })
        .def_property_readonly_static("LEAPING", [](const py::object &) { return PotionType::Leaping; })
        .def_property_readonly_static("LONG_LEAPING", [](const py::object &) { return PotionType::LongLeaping; })
        .def_property_readonly_static("STRONG_LEAPING", [](const py::object &) { return PotionType::StrongLeaping; })
        .def_property_readonly_static("FIRE_RESISTANCE", [](const py::object &) { return PotionType::FireResistance; })
        .def_property_readonly_static("LONG_FIRE_RESISTANCE",
                                      [](const py::object &) { return PotionType::LongFireResistance; })
        .def_property_readonly_static("SWIFTNESS", [](const py::object &) { return PotionType::Swiftness; })
        .def_property_readonly_static("LONG_SWIFTNESS", [](const py::object &) { return PotionType::LongSwiftness; })
        .def_property_readonly_static("STRONG_SWIFTNESS",
                                      [](const py::object &) { return PotionType::StrongSwiftness; })
        .def_property_readonly_static("SLOWNESS", [](const py::object &) { return PotionType::Slowness; })
        .def_property_readonly_static("LONG_SLOWNESS", [](const py::object &) { return PotionType::LongSlowness; })
        .def_property_readonly_static("STRONG_SLOWNESS", [](const py::object &) { return PotionType::StrongSlowness; })
        .def_property_readonly_static("WATER_BREATHING", [](const py::object &) { return PotionType::WaterBreathing; })
        .def_property_readonly_static("LONG_WATER_BREATHING",
                                      [](const py::object &) { return PotionType::LongWaterBreathing; })
        .def_property_readonly_static("HEALING", [](const py::object &) { return PotionType::Healing; })
        .def_property_readonly_static("STRONG_HEALING", [](const py::object &) { return PotionType::StrongHealing; })
        .def_property_readonly_static("HARMING", [](const py::object &) { return PotionType::Harming; })
        .def_property_readonly_static("STRONG_HARMING", [](const py::object &) { return PotionType::StrongHarming; })
        .def_property_readonly_static("POISON", [](const py::object &) { return PotionType::Poison; })
        .def_property_readonly_static("LONG_POISON", [](const py::object &) { return PotionType::LongPoison; })
        .def_property_readonly_static("STRONG_POISON", [](const py::object &) { return PotionType::StrongPoison; })
        .def_property_readonly_static("REGENERATION", [](const py::object &) { return PotionType::Regeneration; })
        .def_property_readonly_static("LONG_REGENERATION",
                                      [](const py::object &) { return PotionType::LongRegeneration; })
        .def_property_readonly_static("STRONG_REGENERATION",
                                      [](const py::object &) { return PotionType::StrongRegeneration; })
        .def_property_readonly_static("STRENGTH", [](const py::object &) { return PotionType::Strength; })
        .def_property_readonly_static("LONG_STRENGTH", [](const py::object &) { return PotionType::LongStrength; })
        .def_property_readonly_static("STRONG_STRENGTH", [](const py::object &) { return PotionType::StrongStrength; })
        .def_property_readonly_static("WEAKNESS", [](const py::object &) { return PotionType::Weakness; })
        .def_property_readonly_static("LONG_WEAKNESS", [](const py::object &) { return PotionType::LongWeakness; })
        .def_property_readonly_static("WITHER", [](const py::object &) { return PotionType::Wither; })
        .def_property_readonly_static("TURTLE_MASTER", [](const py::object &) { return PotionType::TurtleMaster; })
        .def_property_readonly_static("LONG_TURTLE_MASTER",
                                      [](const py::object &) { return PotionType::LongTurtleMaster; })
        .def_property_readonly_static("STRONG_TURTLE_MASTER",
                                      [](const py::object &) { return PotionType::StrongTurtleMaster; })
        .def_property_readonly_static("SLOW_FALLING", [](const py::object &) { return PotionType::SlowFalling; })
        .def_property_readonly_static("LONG_SLOW_FALLING",
                                      [](const py::object &) { return PotionType::LongSlowFalling; })
        .def_property_readonly_static("WIND_CHARGED", [](const py::object &) { return PotionType::WindCharged; })
        .def_property_readonly_static("WEAVING", [](const py::object &) { return PotionType::Weaving; })
        .def_property_readonly_static("OOZING", [](const py::object &) { return PotionType::Oozing; })
        .def_property_readonly_static("INFESTED", [](const py::object &) { return PotionType::Infested; });
}
}  // namespace endstone::python
