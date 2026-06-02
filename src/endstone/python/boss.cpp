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

void init_boss(py::module_ &m)
{
    py::native_enum<BarColor>(m, "BarColor", "enum.Enum", "Represents the color of a boss bar.")  //
        .value("PINK", BarColor::Pink)
        .value("BLUE", BarColor::Blue)
        .value("RED", BarColor::Red)
        .value("GREEN", BarColor::Green)
        .value("YELLOW", BarColor::Yellow)
        .value("PURPLE", BarColor::Purple)
        .value("REBECCA_PURPLE", BarColor::RebeccaPurple)
        .value("WHITE", BarColor::White)
        .finalize();

    py::native_enum<BarFlag>(m, "BarFlag", "enum.Enum",
                             "Represents an optional flag that changes the appearance or behaviour of a boss bar.")  //
        .value("DARKEN_SKY", BarFlag::DarkenSky, "Darkens the sky like during fighting a wither.")
        .value("CREATE_FOG", BarFlag::CreateFog, "Creates fog around the world.")
        .finalize();

    py::native_enum<BarStyle>(m, "BarStyle", "enum.Enum", "Represents the segment style of a boss bar.")  //
        .value("SOLID", BarStyle::Solid, "Makes the boss bar solid (no segments).")
        .value("SEGMENTED_6", BarStyle::Segmented6, "Splits the boss bar into 6 segments.")
        .value("SEGMENTED_10", BarStyle::Segmented10, "Splits the boss bar into 10 segments.")
        .value("SEGMENTED_12", BarStyle::Segmented12, "Splits the boss bar into 12 segments.")
        .value("SEGMENTED_20", BarStyle::Segmented20, "Splits the boss bar into 20 segments.")
        .finalize();

    py::classh<BossBar>(m, "BossBar", "Represents a boss bar that is displayed to players.")
        .def_property("title", &BossBar::getTitle, &BossBar::setTitle, "The title of this boss bar.")
        .def_property("color", &BossBar::getColor, &BossBar::setColor, "The color of this boss bar.")
        .def_property("style", &BossBar::getStyle, &BossBar::setStyle, "The style of this boss bar.")
        .def_property("progress", &BossBar::getProgress, &BossBar::setProgress, R"doc(
    The progress of the bar between `0.0` and `1.0`.

    Values should be between `0.0` (empty) and `1.0` (full).
)doc")
        .def_property("is_visible", &BossBar::isVisible, &BossBar::setVisible,
                      "Whether the boss bar is displayed to attached players.")
        .def("has_flag", &BossBar::hasFlag, py::arg("flag"), R"doc(
    Returns whether this boss bar has the passed flag set.

    Args:
        flag: The flag to check.

    Returns:
        Whether it has the flag.
)doc")
        .def("add_flag", &BossBar::addFlag, py::arg("flag"), R"doc(
    Add an optional flag to this boss bar.

    Args:
        flag: An optional flag to set on the boss bar.
)doc")
        .def("remove_flag", &BossBar::removeFlag, py::arg("flag"), R"doc(
    Remove an existing flag on this boss bar.

    Args:
        flag: The existing flag to remove.
)doc")
        .def("add_player", &BossBar::addPlayer, py::arg("player"), R"doc(
    Adds the player to this boss bar causing it to display on their screen.

    Args:
        player: The player to add.
)doc")
        .def("remove_player", &BossBar::removePlayer, py::arg("player"), R"doc(
    Removes the player from this boss bar causing it to be removed from their screen.

    Args:
        player: The player to remove.
)doc")
        .def("remove_all", &BossBar::removeAll, "Removes all players from this boss bar.")
        .def_property_readonly("players", &BossBar::getPlayers, "All players viewing this boss bar.",
                               py::return_value_policy::reference_internal);
}

}  // namespace endstone::python
