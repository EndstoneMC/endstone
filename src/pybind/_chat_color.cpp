// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/chat_color.h"
#include "pybind/pybind.h"

void def_chat_color(py::module &m)
{
    py::enum_<ChatColor>(m, "ChatColor")
        .value("BLACK", ChatColor::Black)
        .value("DARK_BLUE", ChatColor::DarkBlue)
        .value("DARK_GREEN", ChatColor::DarkGreen)
        .value("DARK_AQUA", ChatColor::DarkAqua)
        .value("DARK_RED", ChatColor::DarkRed)
        .value("DARK_PURPLE", ChatColor::DarkPurple)
        .value("GOLD", ChatColor::Gold)
        .value("GRAY", ChatColor::Gray)
        .value("DARK_GRAY", ChatColor::DarkGray)
        .value("BLUE", ChatColor::Blue)
        .value("GREEN", ChatColor::Green)
        .value("AQUA", ChatColor::Aqua)
        .value("RED", ChatColor::Red)
        .value("LIGHT_PURPLE", ChatColor::LightPurple)
        .value("YELLOW", ChatColor::Yellow)
        .value("WHITE", ChatColor::White)
        .value("MINECOIN_GOLD", ChatColor::MinecoinGold)
        .value("MATERIAL_QUARTZ", ChatColor::MaterialQuartz)
        .value("MATERIAL_IRON", ChatColor::MaterialIron)
        .value("MATERIAL_NETHERITE", ChatColor::MaterialNetherite)
        .value("MAGIC", ChatColor::Magic)
        .value("BOLD", ChatColor::Bold)
        .value("MATERIAL_REDSTONE", ChatColor::MaterialRedstone)
        .value("MATERIAL_COPPER", ChatColor::MaterialCopper)
        .value("ITALIC", ChatColor::Italic)
        .value("MATERIAL_GOLD", ChatColor::MaterialGold)
        .value("MATERIAL_EMERALD", ChatColor::MaterialEmerald)
        .value("RESET", ChatColor::Reset)
        .value("MATERIAL_DIAMOND", ChatColor::MaterialDiamond)
        .value("MATERIAL_LAPIS", ChatColor::MaterialLapis)
        .value("MATERIAL_AMETHYST", ChatColor::MaterialAmethyst)
        .export_values()
        .def(py::self + py::str())
        .def(py::str() + py::self)
        .def("__str__", &ChatColors::toString, py::prepend{});
}
