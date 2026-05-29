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
    py::native_enum<RenderType>(m, "RenderType", "enum.Enum",
                                "Controls the way in which an Objective is rendered on the client side.")
        .value("INTEGER", RenderType::Integer)
        .value("HEARTS", RenderType::Hearts)
        .finalize();

    py::native_enum<DisplaySlot>(m, "DisplaySlot", "enum.Enum", "Locations for displaying objectives to the player")
        .value("BELOW_NAME", DisplaySlot::BelowName, "Displays the score below the player's name.")
        .value("PLAYER_LIST", DisplaySlot::PlayerList, "Displays the score in the player list on the pause screen.")
        .value("SIDE_BAR", DisplaySlot::SideBar, "Displays the score on the side of the player's screen.")
        .finalize();

    py::native_enum<ObjectiveSortOrder>(m, "ObjectiveSortOrder", "enum.Enum",
                                        "Represents the sort order of objectives on a DisplaySlot.")
        .value("ASCENDING", ObjectiveSortOrder::Ascending, "Sorts the objectives in the ascending order")
        .value("DESCENDING", ObjectiveSortOrder::Descending, "Sorts the objectives in the descending order")
        .finalize();

    auto scoreboard = py::class_<Scoreboard, std::shared_ptr<Scoreboard>>(m, "Scoreboard", "Represents a scoreboard.");
    auto objective = py::class_<Objective>(
        m, "Objective", "Represents an objective on a scoreboard that can show scores specific to entries.");

    auto criteria = py::class_<Criteria>(m, "Criteria", "Represents a scoreboard criteria.");

    py::native_enum<Criteria::Type>(criteria, "Type", "enum.Enum", "Represents a scoreboard criteria.")
        .value("DUMMY", Criteria::Type::Dummy, "The dummy criteria. Not changed by the server.")
        .export_values()
        .finalize();

    criteria.def_property_readonly("name", &Criteria::getName, "The name of this criteria (its unique id).")
        .def_property_readonly("is_read_only", &Criteria::isReadOnly, R"doc(
    True if this criteria is read only.

    If read only, scoreboards with this criteria cannot have their scores changed.
)doc")
        .def_property_readonly("default_render_type", &Criteria::getDefaultRenderType,
                               "The RenderType used by default for this criteria.");

    py::class_<Score>(m, "Score", "Represents a score for an objective on a scoreboard.")
        .def_property_readonly("entry", &Score::getEntry, "The entry being tracked by this Score.",
                               py::return_value_policy::reference_internal)
        .def_property("value", &Score::getValue, &Score::setValue, "The current score.")
        .def_property_readonly("is_score_set", &Score::isScoreSet,
                               "True if this score has been set at any point in time.")
        .def_property_readonly("objective", &Score::getObjective, "The Objective being tracked by this Score.",
                               py::return_value_policy::reference)
        .def_property_readonly("scoreboard", &Score::getScoreboard, "The scoreboard for the associated objective.",
                               py::return_value_policy::reference);

    objective.def_property_readonly("name", &Objective::getName, "The name of this Objective.")
        .def_property("display_name", &Objective::getDisplayName, &Objective::setDisplayName,
                      "The name displayed to players for this objective.")
        .def_property_readonly("criteria", &Objective::getCriteria, "The criteria this objective tracks.")
        .def_property_readonly("is_modifiable", &Objective::isModifiable,
                               "True if the objective's scores can be modified directly by a plugin.")
        .def_property_readonly("scoreboard", &Objective::getScoreboard,
                               "The scoreboard to which this objective is attached.",
                               py::return_value_policy::reference)
        .def("unregister", &Objective::unregister, "Unregisters this objective from the associated Scoreboard.")
        .def_property_readonly("is_displayed", &Objective::isDisplayed,
                               "True if the objective is currently displayed in a slot.")
        .def_property("display_slot", &Objective::getDisplaySlot, &Objective::setDisplaySlot,
                      "The display slot this objective is displayed at.")
        .def_property("sort_order", &Objective::getSortOrder, &Objective::setSortOrder,
                      "The sort order for this objective.")
        .def("set_display", &Objective::setDisplay, py::arg("slot"),
             py::arg("order") = ObjectiveSortOrder::Ascending, R"doc(
    Sets the display slot and sort order for this objective.

    This will remove it from any other display slot.

    Args:
        slot: The display slot where this objective should be displayed.
        order: The sort order for this objective in the display slot.
)doc")
        .def_property_readonly("render_type", &Objective::getRenderType,
                               "The manner in which this objective will be rendered.")
        .def("get_score", &Objective::getScore, py::arg("entry"), R"doc(
    Gets an entry's Score for this objective.

    Args:
        entry: Entry for the Score.

    Returns:
        Score tracking the Objective and entry specified.
)doc")
        .def(py::self == py::self)
        .def(py::self != py::self);

    scoreboard
        .def(
            "add_objective",
            [](Scoreboard &self, const std::string &name, Criteria::Type criteria,
               const std::optional<std::string> &display_name, RenderType render_type) {
                return self.addObjective(name, criteria, display_name.value_or(name), render_type);
            },
            py::arg("name"), py::arg("criteria"), py::arg("display_name") = py::none(),
            py::arg_v("render_type", RenderType::Integer), R"doc(
    Registers an Objective on this Scoreboard.

    Args:
        name: Name of the Objective.
        criteria: Criteria type for the Objective.
        display_name: Name displayed to players for the Objective. Defaults to ``name``.
        render_type: Manner of rendering the Objective.

    Returns:
        The newly registered Objective.
)doc")
        .def("get_objective", py::overload_cast<std::string>(&Scoreboard::getObjective, py::const_),
             py::arg("name").noconvert(), R"doc(
    Gets an Objective on this Scoreboard by name.

    Args:
        name: Name of the Objective.

    Returns:
        The Objective, or None if it does not exist.
)doc")
        .def("get_objective", py::overload_cast<DisplaySlot>(&Scoreboard::getObjective, py::const_),
             py::arg("slot").noconvert(), R"doc(
    Gets the Objective currently displayed in a DisplaySlot on this Scoreboard.

    Args:
        slot: The DisplaySlot.

    Returns:
        The Objective currently displayed, or None if nothing is displayed in that DisplaySlot.
)doc")
        .def_property_readonly("objectives", &Scoreboard::getObjectives, "A list of all Objectives on this Scoreboard.")
        .def("get_objectives_by_criteria", &Scoreboard::getObjectivesByCriteria, py::arg("criteria"), R"doc(
    Gets all Objectives of a Criteria on the Scoreboard.

    Args:
        criteria: Criteria type to search by.

    Returns:
        A list of Objectives using the specified Criteria.
)doc")
        .def("get_scores", &Scoreboard::getScores, py::arg("entry"), R"doc(
    Gets all scores for an entry on this Scoreboard.

    Args:
        entry: The entry whose scores are being retrieved.

    Returns:
        A list of all scores tracked for the entry.
)doc")
        .def("reset_scores", &Scoreboard::resetScores, py::arg("entry"), R"doc(
    Removes all scores for an entry on this Scoreboard.

    Args:
        entry: The entry to drop all current scores for.
)doc")
        .def_property_readonly("entries", &Scoreboard::getEntries, "A list of all entries tracked by this Scoreboard.",
                               py::return_value_policy::reference_internal)
        .def("clear_slot", &Scoreboard::clearSlot, py::arg("slot"), R"doc(
    Clears any objective in the specified slot.

    Args:
        slot: The slot to remove objectives from.
)doc");
}

}  // namespace endstone::python
