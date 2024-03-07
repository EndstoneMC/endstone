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

#include <pybind11/pybind11.h>

#include "endstone/util/color_format.h"

namespace py = pybind11;

#define ADD_COLOR_FORMAT(Name) def_property_readonly_static(#Name, [](const py::object &) { return ColorFormat::Name; })

namespace endstone::detail {

void init_util(py::module &m)
{
    py::class_<ColorFormat>(m, "ColorFormat")
        .ADD_COLOR_FORMAT(BLACK)
        .ADD_COLOR_FORMAT(DARK_BLUE)
        .ADD_COLOR_FORMAT(DARK_GREEN)
        .ADD_COLOR_FORMAT(DARK_AQUA)
        .ADD_COLOR_FORMAT(DARK_RED)
        .ADD_COLOR_FORMAT(DARK_PURPLE)
        .ADD_COLOR_FORMAT(GOLD)
        .ADD_COLOR_FORMAT(GRAY)
        .ADD_COLOR_FORMAT(DARK_GRAY)
        .ADD_COLOR_FORMAT(BLUE)
        .ADD_COLOR_FORMAT(GREEN)
        .ADD_COLOR_FORMAT(AQUA)
        .ADD_COLOR_FORMAT(RED)
        .ADD_COLOR_FORMAT(LIGHT_PURPLE)
        .ADD_COLOR_FORMAT(YELLOW)
        .ADD_COLOR_FORMAT(WHITE)
        .ADD_COLOR_FORMAT(MINECOIN_GOLD)
        .ADD_COLOR_FORMAT(MATERIAL_QUARTZ)
        .ADD_COLOR_FORMAT(MATERIAL_IRON)
        .ADD_COLOR_FORMAT(MATERIAL_NETHERITE)
        .ADD_COLOR_FORMAT(MATERIAL_REDSTONE)
        .ADD_COLOR_FORMAT(MATERIAL_COPPER)
        .ADD_COLOR_FORMAT(MATERIAL_GOLD)
        .ADD_COLOR_FORMAT(MATERIAL_EMERALD)
        .ADD_COLOR_FORMAT(MATERIAL_DIAMOND)
        .ADD_COLOR_FORMAT(MATERIAL_LAPIS)
        .ADD_COLOR_FORMAT(MATERIAL_AMETHYST)
        .ADD_COLOR_FORMAT(OBFUSCATED)
        .ADD_COLOR_FORMAT(BOLD)
        .ADD_COLOR_FORMAT(ITALIC)
        .ADD_COLOR_FORMAT(RESET);
}

}  // namespace endstone::detail
