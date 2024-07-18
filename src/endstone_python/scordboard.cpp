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

#include "endstone/actor/actor.h"
#include "endstone/player.h"
#include "endstone/scoreboard/criteria.h"
#include "endstone/scoreboard/display_slot.h"
#include "endstone/scoreboard/objective.h"
#include "endstone/scoreboard/scoreboard.h"

namespace py = pybind11;

namespace endstone::detail {

void init_scoreboard(py::module_ &m)
{
    py::enum_<Criteria>(m, "Criteria", "Represents a scoreboard criteria.").value("DUMMY", Criteria::Dummy);

    py::enum_<DisplaySlot>(m, "DisplaySlot", "Locations for displaying objectives to the player")
        .value("BELOW_NAME", DisplaySlot::BelowName, "Displays the score below the player's name.")
        .value("PLAYER_LIST", DisplaySlot::PlayerList, "Displays the score in the player list on the pause screen.")
        .value("SIDE_BAR", DisplaySlot::SideBar, "Displays the score on the side of the player's screen.");

    py::class_<Objective>(m, "Objective",
                          "Represents an objective on a scoreboard that can show scores specific to entries.")
        .def_property_readonly("name", &Objective::getName, "Gets the name of this Objective")
        .def_property_readonly("display_name", &Objective::getDisplayName,
                               "Gets the name displayed to players for this objective")
        .def_property_readonly("is_valid", &Objective::isValid, "Determines if the Objective is valid.");

    py::class_<Scoreboard>(m, "Scoreboard")
        .def(
            "add_objective",
            [](Scoreboard &self, const std::string &name, Criteria criteria,
               const std::optional<std::string> &display_name) {
                self.addObjective(name, criteria, display_name.value_or(name));
            },
            "Registers an Objective on this Scoreboard with a name displayed to players", py::arg("name"),
            py::arg("criteria"), py::arg("display_name") = py::none(), py::return_value_policy::reference)
        .def("get_objective", py::overload_cast<std::string>(&Scoreboard::getObjective, py::const_),
             "Gets an Objective on this Scoreboard by name", py::arg("name").noconvert(),
             py::return_value_policy::reference)
        .def("get_objective", py::overload_cast<DisplaySlot>(&Scoreboard::getObjective, py::const_),
             "Gets the Objective currently displayed in a DisplaySlot on this Scoreboard", py::arg("slot").noconvert(),
             py::return_value_policy::reference)
        .def_property_readonly("objectives", &Scoreboard::getObjectives, "Gets all Objectives on this Scoreboard",
                               py::return_value_policy::reference_internal)
        .def("get_objectives_by_criteria", &Scoreboard::getObjectivesByCriteria,
             "Gets all Objectives of a Criteria on the Scoreboard", py::arg("criteria"))
        .def_property_readonly("entries", &Scoreboard::getEntries, "Gets all entries tracked by this Scoreboard",
                               py::return_value_policy::reference_internal)
        .def("clear_slot", &Scoreboard::clearSlot, "Clears any objective in the specified slot", py::arg("slot"));
}

}  // namespace endstone::detail
