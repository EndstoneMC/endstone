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

void init_debug_shape(py::module_ &m)
{
    py::class_<DebugArrow>(m, "DebugArrow", "Represents a debug arrow.")
        .def(py::init<>())
        .def_property_readonly("id", &DebugArrow::getId, "Gets the id of the debug arrow.")
        .def_property("position", &DebugArrow::getPosition, &DebugArrow::setPosition,
                      "Gets or sets the position of the debug arrow.", py::return_value_policy::reference)
        .def_property("color", &DebugArrow::getColor, &DebugArrow::setColor,
                      "Gets or sets the color of the debug arrow.", py::return_value_policy::reference)
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

    py::class_<DebugBox>(m, "DebugBox", "Represents a debug box.")
        .def(py::init<>())
        .def_property_readonly("id", &DebugBox::getId, "Gets the id of the debug box.")
        .def_property("position", &DebugBox::getPosition, &DebugBox::setPosition,
                      "Gets or sets the position of the debug box.", py::return_value_policy::reference)
        .def_property("color", &DebugBox::getColor, &DebugBox::setColor,
                      "Gets or sets the color of the debug box.", py::return_value_policy::reference)
        .def_property("scale", &DebugBox::getScale, &DebugBox::setScale,
                      "Gets or sets the scale of the box.", py::return_value_policy::reference)
        .def_property("box_bounds", &DebugBox::getBoxBounds, &DebugBox::setBoxBounds,
                      "Gets or sets the bounds of the box.", py::return_value_policy::reference);

    py::class_<DebugCircle>(m, "DebugCircle", "Represents a debug circle.")
        .def(py::init<>())
        .def_property_readonly("id", &DebugCircle::getId, "Gets the id of the debug circle.")
        .def_property("position", &DebugCircle::getPosition, &DebugCircle::setPosition,
                      "Gets or sets the position of the debug circle.", py::return_value_policy::reference)
        .def_property("color", &DebugCircle::getColor, &DebugCircle::setColor,
                      "Gets or sets the color of the debug circle.", py::return_value_policy::reference)
        .def_property("scale", &DebugCircle::getScale, &DebugCircle::setScale,
                      "Gets or sets the scale of the circle.", py::return_value_policy::reference)
        .def_property("segments", &DebugCircle::getSegments, &DebugCircle::setSegments,
                      "Gets or sets the number of segments used to render the circle.", py::return_value_policy::reference);

    py::class_<DebugLine>(m, "DebugLine", "Represents a debug line.")
        .def(py::init<>())
        .def_property_readonly("id", &DebugLine::getId, "Gets the id of the debug line.")
        .def_property("position", &DebugLine::getPosition, &DebugLine::setPosition,
                      "Gets or sets the position of the debug line.", py::return_value_policy::reference)
        .def_property("color", &DebugLine::getColor, &DebugLine::setColor,
                      "Gets or sets the color of the debug line.", py::return_value_policy::reference)
        .def_property("end_position", &DebugLine::getEndPosition, &DebugLine::setEndPosition,
                      "Gets or sets the end position of the line.", py::return_value_policy::reference);

    py::class_<DebugSphere>(m, "DebugSphere", "Represents a debug sphere.")
        .def(py::init<>())
        .def_property_readonly("id", &DebugSphere::getId, "Gets the id of the debug sphere.")
        .def_property("position", &DebugSphere::getPosition, &DebugSphere::setPosition,
                      "Gets or sets the position of the debug sphere.", py::return_value_policy::reference)
        .def_property("color", &DebugSphere::getColor, &DebugSphere::setColor,
                      "Gets or sets the color of the debug sphere.", py::return_value_policy::reference)
        .def_property("scale", &DebugSphere::getScale, &DebugSphere::setScale,
                      "Gets or sets the scale of the sphere.", py::return_value_policy::reference)
        .def_property("segments", &DebugSphere::getSegments, &DebugSphere::setSegments,
                      "Gets or sets the number of segments used to render the sphere.", py::return_value_policy::reference);

    py::class_<DebugText>(m, "DebugText", "Represents a debug text.")
        .def(py::init<>())
        .def_property_readonly("id", &DebugText::getId, "Gets the id of the debug text.")
        .def_property("position", &DebugText::getPosition, &DebugText::setPosition,
                      "Gets or sets the position of the debug text.", py::return_value_policy::reference)
        .def_property("color", &DebugText::getColor, &DebugText::setColor,
                      "Gets or sets the color of the debug text.", py::return_value_policy::reference)
        .def_property("text", &DebugText::getText, &DebugText::setText,
                      "Gets or sets the text of the debug text.", py::return_value_policy::reference);
}

}  // namespace endstone::python