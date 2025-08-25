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
             "with any enchantments already applied to the item.");
}

}  // namespace endstone::python
