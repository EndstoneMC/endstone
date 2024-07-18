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

#include "endstone/scoreboard/display_slot.h"
#include "endstone/scoreboard/objective.h"

namespace py = pybind11;

namespace endstone::detail {

void init_scoreboard(py::module_ &m)
{
    py::enum_<endstone::DisplaySlot>(m, "DisplaySlot", "Locations for displaying objectives to the player")
        .value("BELOW_NAME", endstone::DisplaySlot::BelowName, "Displays the score below the player's name.")
        .value("PLAYER_LIST", endstone::DisplaySlot::PlayerList,
               "Displays the score in the player list on the pause screen.")
        .value("SIDE_BAR", endstone::DisplaySlot::SideBar, "Displays the score on the side of the player's screen.");

    py::class_<Objective>(m, "Objective",
                          "Represents an objective on a scoreboard that can show scores specific to entries.")
        .def_property_readonly("name", &Objective::getName, "Gets the name of this Objective")
        .def_property_readonly("display_name", &Objective::getDisplayName,
                               "Gets the name displayed to players for this objective")
        .def_property_readonly("is_valid", &Objective::isValid, "Determines if the Objective is valid.");
}

}  // namespace endstone::detail
