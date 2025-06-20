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

void init_scoreboard(py::module_ &m)
{
    py::enum_<RenderType>(m, "RenderType", "Controls the way in which an Objective is rendered on the client side.")
        .value("INTEGER", RenderType::Integer)
        .value("HEARTS", RenderType::Hearts);

    py::enum_<DisplaySlot>(m, "DisplaySlot", "Locations for displaying objectives to the player")
        .value("BELOW_NAME", DisplaySlot::BelowName, "Displays the score below the player's name.")
        .value("PLAYER_LIST", DisplaySlot::PlayerList, "Displays the score in the player list on the pause screen.")
        .value("SIDE_BAR", DisplaySlot::SideBar, "Displays the score on the side of the player's screen.");

    py::enum_<ObjectiveSortOrder>(m, "ObjectiveSortOrder", "Represents the sort order of objectives on a DisplaySlot.")
        .value("ASCENDING", ObjectiveSortOrder::Ascending, "Sorts the objectives in the ascending order")
        .value("DESCENDING", ObjectiveSortOrder::Descending, "Sorts the objectives in the descending order");

    auto scoreboard = py::class_<Scoreboard, std::shared_ptr<Scoreboard>>(m, "Scoreboard", "Represents a scoreboard");
    auto objective = py::class_<Objective>(
        m, "Objective", "Represents an objective on a scoreboard that can show scores specific to entries.");

    auto criteria = py::class_<Criteria>(m, "Criteria", "Represents a scoreboard criteria.");

    py::enum_<Criteria::Type>(criteria, "Type", "Represents a scoreboard criteria.")
        .value("DUMMY", Criteria::Type::Dummy, "The dummy criteria. Not changed by the server.")
        .export_values();

    criteria.def_property_readonly("name", &Criteria::getName)
        .def_property_readonly("is_read_only", &Criteria::isReadOnly)
        .def_property_readonly("default_render_type", &Criteria::getDefaultRenderType);

    py::class_<Score>(m, "Score", "Represents a score for an objective on a scoreboard.")
        .def_property_readonly("entry", &Score::getEntry, "Gets the entry being tracked by this Score",
                               py::return_value_policy::reference_internal)
        .def_property("value", &Score::getValue, &Score::setValue, "Gets or sets the current score.")
        .def_property_readonly("is_score_set", &Score::isScoreSet,
                               "Shows if this score has been set at any point in time.")
        .def_property_readonly("objective", &Score::getObjective, "Gets the Objective being tracked by this Score.",
                               py::return_value_policy::reference)
        .def_property_readonly("scoreboard", &Score::getScoreboard, "Gets the scoreboard for the associated objective.",
                               py::return_value_policy::reference);

    objective.def_property_readonly("name", &Objective::getName, "Gets the name of this Objective")
        .def_property("display_name", &Objective::getDisplayName, &Objective::setDisplayName,
                      "Gets or sets the name displayed to players for this objective")
        .def_property_readonly("criteria", &Objective::getCriteria, "Gets the criteria this objective tracks")
        .def_property_readonly("is_modifiable", &Objective::isModifiable,
                               "Gets if the objective's scores can be modified directly by a plugin")
        .def_property_readonly("scoreboard", &Objective::getScoreboard,
                               "Gets the scoreboard to which this objective is attached",
                               py::return_value_policy::reference)
        .def("unregister", &Objective::unregister, "Unregisters this objective from the associated Scoreboard.")
        .def_property_readonly("is_displayed", &Objective::isDisplayed,
                               "Gets if the objective is currently displayed in a slot.")
        .def_property(
            "display_slot",
            [](const Objective &self) -> std::optional<DisplaySlot> {
                if (auto result = self.isDisplayed(); result) {
                    if (result.value()) {
                        return self.getDisplaySlot().value();
                    }
                    return std::nullopt;
                }
                else {
                    throw std::runtime_error(result.error());
                }
            },
            &Objective::setDisplaySlot, "Gets or sets the display slot this objective is displayed at")
        .def_property("sort_order", &Objective::getSortOrder, &Objective::setSortOrder,
                      "Gets or sets the sort order for this objective")
        .def(
            "set_display",
            [](Objective &self, std::optional<DisplaySlot> slot, std::optional<ObjectiveSortOrder> order) {
                self.setDisplay(slot, order.value_or(ObjectiveSortOrder::Ascending));
            },
            "Sets the display slot and sort order for this objective. This will remove it from any other display slot.",
            py::arg("slot"), py::arg("order") = std::nullopt)

        .def_property_readonly("render_type", &Objective::getRenderType,
                               "Gets the manner in which this objective will be rendered.")
        .def("get_score", &Objective::getScore, "Gets an entry's Score for this objective", py::arg("entry"))
        .def(py::self == py::self)
        .def(py::self != py::self);

    scoreboard
        .def(
            "add_objective",
            [](Scoreboard &self, const std::string &name, Criteria::Type criteria,
               const std::optional<std::string> &display_name, RenderType render_type) {
                return self.addObjective(name, criteria, display_name.value_or(name), render_type);
            },
            "Registers an Objective on this Scoreboard with a name displayed to players", py::arg("name"),
            py::arg("criteria"), py::arg("display_name") = py::none(), py::arg("render_type") = RenderType::Integer)
        .def("get_objective", py::overload_cast<std::string>(&Scoreboard::getObjective, py::const_),
             "Gets an Objective on this Scoreboard by name", py::arg("name").noconvert())
        .def("get_objective", py::overload_cast<DisplaySlot>(&Scoreboard::getObjective, py::const_),
             "Gets the Objective currently displayed in a DisplaySlot on this Scoreboard", py::arg("slot").noconvert())
        .def_property_readonly("objectives", &Scoreboard::getObjectives, "Gets all Objectives on this Scoreboard")
        .def("get_objectives_by_criteria", &Scoreboard::getObjectivesByCriteria,
             "Gets all Objectives of a Criteria on the Scoreboard", py::arg("criteria"))
        .def("get_scores", &Scoreboard::getScores, "Gets all scores for an entry on this Scoreboard", py::arg("entry"))
        .def("reset_scores", &Scoreboard::resetScores, "Removes all scores for an entry on this Scoreboard",
             py::arg("entry"))
        .def_property_readonly("entries", &Scoreboard::getEntries, "Gets all entries tracked by this Scoreboard",
                               py::return_value_policy::reference_internal)
        .def("clear_slot", &Scoreboard::clearSlot, "Clears any objective in the specified slot", py::arg("slot"));
}

}  // namespace endstone::python
