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

#include <fmt/format.h>

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

void init_enchantments(py::module_ &m)
{
    py::class_<Enchantment, std::shared_ptr<Enchantment>>(m, "Enchantment")
        .def_property_readonly_static("PROTECTION", [](const py::object &) { return Enchantment::Protection; })
        .def_property_readonly_static("FIRE_PROTECTION", [](const py::object &) { return Enchantment::FireProtection; })
        .def_property_readonly_static("FEATHER_FALLING", [](const py::object &) { return Enchantment::FeatherFalling; })
        .def_property_readonly_static("BLAST_PROTECTION",
                                      [](const py::object &) { return Enchantment::BlastProtection; })
        .def_property_readonly_static("PROJECTILE_PROTECTION",
                                      [](const py::object &) { return Enchantment::ProjectileProtection; })
        .def_property_readonly_static("THORNS", [](const py::object &) { return Enchantment::Thorns; })
        .def_property_readonly_static("RESPIRATION", [](const py::object &) { return Enchantment::Respiration; })
        .def_property_readonly_static("DEPTH_STRIDER", [](const py::object &) { return Enchantment::DepthStrider; })
        .def_property_readonly_static("AQUA_AFFINITY", [](const py::object &) { return Enchantment::AquaAffinity; })
        .def_property_readonly_static("SHARPNESS", [](const py::object &) { return Enchantment::Sharpness; })
        .def_property_readonly_static("SMITE", [](const py::object &) { return Enchantment::Smite; })
        .def_property_readonly_static("BANE_OF_ARTHROPODS",
                                      [](const py::object &) { return Enchantment::BaneOfArthropods; })
        .def_property_readonly_static("KNOCKBACK", [](const py::object &) { return Enchantment::Knockback; })
        .def_property_readonly_static("FIRE_ASPECT", [](const py::object &) { return Enchantment::FireAspect; })
        .def_property_readonly_static("LOOTING", [](const py::object &) { return Enchantment::Looting; })
        .def_property_readonly_static("EFFICIENCY", [](const py::object &) { return Enchantment::Efficiency; })
        .def_property_readonly_static("SILK_TOUCH", [](const py::object &) { return Enchantment::SilkTouch; })
        .def_property_readonly_static("UNBREAKING", [](const py::object &) { return Enchantment::Unbreaking; })
        .def_property_readonly_static("POWER", [](const py::object &) { return Enchantment::Power; })
        .def_property_readonly_static("PUNCH", [](const py::object &) { return Enchantment::Punch; })
        .def_property_readonly_static("FLAME", [](const py::object &) { return Enchantment::Flame; })
        .def_property_readonly_static("INFINITY", [](const py::object &) { return Enchantment::Infinity; })
        .def_property_readonly_static("LUCK_OF_THE_SEA", [](const py::object &) { return Enchantment::LuckOfTheSea; })
        .def_property_readonly_static("LURE", [](const py::object &) { return Enchantment::Lure; })
        .def_property_readonly_static("FROST_WALKER", [](const py::object &) { return Enchantment::FrostWalker; })
        .def_property_readonly_static("MENDING", [](const py::object &) { return Enchantment::Mending; })
        .def_property_readonly_static("CURSE_OF_BINDING",
                                      [](const py::object &) { return Enchantment::CurseOfBinding; })
        .def_property_readonly_static("CURSE_OF_VANISHING",
                                      [](const py::object &) { return Enchantment::CurseOfVanishing; })
        .def_property_readonly_static("IMPALING", [](const py::object &) { return Enchantment::Impaling; })
        .def_property_readonly_static("RIPTIDE", [](const py::object &) { return Enchantment::Riptide; })
        .def_property_readonly_static("LOYALTY", [](const py::object &) { return Enchantment::Loyalty; })
        .def_property_readonly_static("CHANNELING", [](const py::object &) { return Enchantment::Channeling; })
        .def_property_readonly_static("MULTISHOT", [](const py::object &) { return Enchantment::Multishot; })
        .def_property_readonly_static("PIERCING", [](const py::object &) { return Enchantment::Piercing; })
        .def_property_readonly_static("QUICK_CHARGE", [](const py::object &) { return Enchantment::QuickCharge; })
        .def_property_readonly_static("SOUL_SPEED", [](const py::object &) { return Enchantment::SoulSpeed; })
        .def_property_readonly_static("SWIFT_SNEAK", [](const py::object &) { return Enchantment::SwiftSneak; })
        .def_property_readonly_static("WIND_BURST", [](const py::object &) { return Enchantment::WindBurst; })
        .def_property_readonly_static("DENSITY", [](const py::object &) { return Enchantment::Density; })
        .def_property_readonly_static("BREACH", [](const py::object &) { return Enchantment::Breach; })

        .def_property_readonly("id", &Enchantment::getId, "Return the identifier for this enchantment.")
        .def_property_readonly("translation_key", &Enchantment::getTranslationKey,
                               "Get the translation key, suitable for use in a translation component.")
        .def_property_readonly("max_level", &Enchantment::getMaxLevel,
                               "Gets the maximum level that this Enchantment may become.")
        .def_property_readonly("start_level", &Enchantment::getStartLevel,
                               "Gets the level that this Enchantment should start at (also known as minimum level).")
        .def("conflicts_with", &Enchantment::conflictsWith, py::arg("other"),
             "Check if this enchantment conflicts with another enchantment.")
        .def("can_enchant_item", &Enchantment::canEnchantItem, py::arg("item"),
             "Checks if this Enchantment may be applied to the given ItemStack.\n\nThis does not check if it conflicts "
             "with any enchantments already applied to the item.")
        .def_static("get", &Enchantment::get, py::arg("name"), "Attempts to get the Enchantment with the given name.",
                    py::return_value_policy::reference)
        .def("__str__", &Enchantment::getId)
        .def("__hash__", [](const Enchantment &self) { return std::hash<EnchantmentId>{}(self.getId()); })
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self == std::string_view())
        .def(py::self != std::string_view());
}

}  // namespace endstone::python
