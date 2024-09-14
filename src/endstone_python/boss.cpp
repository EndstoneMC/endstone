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
#include <pybind11/stl.h>

#include "endstone/boss/bar_color.h"
#include "endstone/boss/bar_flag.h"
#include "endstone/boss/bar_style.h"
#include "endstone/boss/boss_bar.h"
#include "endstone/player.h"

namespace py = pybind11;

namespace endstone::detail {

void init_boss(py::module_ &m)
{
    py::enum_<BarColor>(m, "BarColor")  //
        .value("PINK", BarColor::Pink)
        .value("BLUE", BarColor::Blue)
        .value("RED", BarColor::Red)
        .value("GREEN", BarColor::Green)
        .value("YELLOW", BarColor::Yellow)
        .value("PURPLE", BarColor::Purple)
        .value("REBECCA_PURPLE", BarColor::RebeccaPurple)
        .value("WHITE", BarColor::White);

    py::enum_<BarFlag>(m, "BarFlag")  //
        .value("DARKEN_SKY", BarFlag::DarkenSky);

    py::enum_<BarStyle>(m, "BarStyle")  //
        .value("SOLID", BarStyle::Solid)
        .value("SEGMENTED_6", BarStyle::Segmented6)
        .value("SEGMENTED_10", BarStyle::Segmented10)
        .value("SEGMENTED_12", BarStyle::Segmented12)
        .value("SEGMENTED_20", BarStyle::Segmented20);

    py::class_<BossBar>(m, "BossBar", "Represents a boss bar that is displayed to players.")
        .def_property("title", &BossBar::getTitle, &BossBar::setTitle, "The title of this boss bar.")
        .def_property("color", &BossBar::getColor, &BossBar::setColor, "The color of this boss bar.")
        .def_property("style", &BossBar::getStyle, &BossBar::setStyle, "The style of this boss bar.")
        .def_property("progress", &BossBar::getProgress, &BossBar::setProgress,
                      "The progress of the bar between 0.0 and 1.0.")
        .def_property("visible", &BossBar::isVisible, &BossBar::setVisible,
                      "If the boss bar is displayed to attached players.")
        .def("has_flag", &BossBar::hasFlag, py::arg("flag"), "Checks whether this boss bar has the passed flag set.")
        .def("add_flag", &BossBar::addFlag, py::arg("flag"), "Adds an optional flag to this boss bar.")
        .def("remove_flag", &BossBar::removeFlag, py::arg("flag"), "Removes an existing flag on this boss bar.")
        .def("add_player", &BossBar::addPlayer, py::arg("player"),
             "Adds the player to this boss bar causing it to display on their screen.")
        .def("remove_player", &BossBar::removePlayer, py::arg("player"),
             "Removes the player from this boss bar causing it to be removed from their screen.")
        .def("remove_all", &BossBar::removeAll, "Removes all players from this boss bar.")
        .def_property_readonly("players", &BossBar::getPlayers, "Returns all players viewing this boss bar.",
                               py::return_value_policy::reference_internal);
}

}  // namespace endstone::detail
