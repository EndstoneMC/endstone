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

template <typename T>
void bind_debug_shape_common(py::class_<T> &cls) {
    cls.def(py::init<>())
        .def_property_readonly("id", &T::getId, "Gets the id of the debug shape.")
        .def_property_readonly("players", &T::getPlayers, "Gets the players of the debug shape.")
        .def_property("position", &T::getPosition, &T::setPosition,
                      "Gets or sets the position of the debug shape.", py::return_value_policy::reference)
        .def_property("color", &T::getColor, &T::setColor,
                      "Gets or sets the color of the debug shape.", py::return_value_policy::reference)
        .def("add_player", &T::addPlayer, "Adds a player to this debug shape causing it to display on the player's screen.",
             py::arg("player"), py::return_value_policy::reference)
        .def("remove_player", &T::removePlayer, "Removes a player from this debug shape causing it to be removed from the player's screen.",
             py::arg("player"), py::return_value_policy::reference)
        .def("remove_all", &T::removeAll, "Removes all players from this debug shape.", py::return_value_policy::reference);
}

void init_debug_shape(py::module_ &m)
{
    py::class_<DebugArrow> debug_arrow(m, "DebugArrow", "Represents a debug arrow.");
    bind_debug_shape_common(debug_arrow);
    debug_arrow
        .def(py::init<>())
        .def_property("end_position", &DebugArrow::getEndPosition, &DebugArrow::setEndPosition,
                      "Gets or sets the end position of the arrow.", py::return_value_policy::reference)
        .def_property("arrow_head_length", &DebugArrow::getArrowHeadLength, &DebugArrow::setArrowHeadLength,
                      "Gets or sets the length of the arrowhead.", py::return_value_policy::reference)
        .def_property("arrow_head_radius", &DebugArrow::getArrowHeadRadius, &DebugArrow::setArrowHeadRadius,
                      "Gets or sets the radius of the arrowhead.", py::return_value_policy::reference)
        .def_property("arrow_head_segments", &DebugArrow::getArrowHeadSegments, &DebugArrow::setArrowHeadSegments,
                      "Gets or sets the number of segments used to render the arrowhead.", py::return_value_policy::reference)
        .def_property("arrow_head_scale", &DebugArrow::getArrowHeadScale, &DebugArrow::setArrowHeadScale,
                      "Gets or sets the scale of the arrowhead.", py::return_value_policy::reference);

    py::class_<DebugBox> debug_box(m, "DebugBox", "Represents a debug box.");
    bind_debug_shape_common(debug_box);
    debug_box
        .def(py::init<>())
        .def_property("scale", &DebugBox::getScale, &DebugBox::setScale,
                      "Gets or sets the scale of the box.", py::return_value_policy::reference)
        .def_property("box_bounds", &DebugBox::getBoxBounds, &DebugBox::setBoxBounds,
                      "Gets or sets the bounds of the box.", py::return_value_policy::reference);

    py::class_<DebugCircle> debug_circle(m, "DebugCircle", "Represents a debug circle.");
    bind_debug_shape_common(debug_circle);
    debug_circle
        .def(py::init<>())
        .def_property("scale", &DebugCircle::getScale, &DebugCircle::setScale,
                      "Gets or sets the scale of the circle.", py::return_value_policy::reference)
        .def_property("segments", &DebugCircle::getSegments, &DebugCircle::setSegments,
                      "Gets or sets the number of segments used to render the circle.", py::return_value_policy::reference);

    py::class_<DebugLine> debug_line(m, "DebugLine", "Represents a debug line.");
    bind_debug_shape_common(debug_line);
    debug_line
        .def(py::init<>())
        .def_property("end_position", &DebugLine::getEndPosition, &DebugLine::setEndPosition,
                      "Gets or sets the end position of the line.", py::return_value_policy::reference);

    py::class_<DebugSphere> debug_sphere(m, "DebugSphere", "Represents a debug sphere.");
    bind_debug_shape_common(debug_sphere);
    debug_sphere
        .def(py::init<>())
        .def_property("scale", &DebugSphere::getScale, &DebugSphere::setScale,
                      "Gets or sets the scale of the sphere.", py::return_value_policy::reference)
        .def_property("segments", &DebugSphere::getSegments, &DebugSphere::setSegments,
                      "Gets or sets the number of segments used to render the sphere.", py::return_value_policy::reference);

    py::class_<DebugText> debug_text(m, "DebugText", "Represents a debug text.");
    bind_debug_shape_common(debug_text);
    debug_text
        .def(py::init<>())
        .def_property("text", &DebugText::getText, &DebugText::setText,
                      "Gets or sets the text of the debug text.", py::return_value_policy::reference);
}

}  // namespace endstone::python