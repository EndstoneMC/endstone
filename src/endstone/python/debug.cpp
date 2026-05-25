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

void init_debug(py::module_ &m)
{
    py::class_<DebugBox>(m, "DebugBox", "Represents a debug box (cuboid) shape.")
        .def(py::init<>())
        .def_property("color", &DebugBox::getColor, &DebugBox::setColor, "The color of this shape.")
        .def_property("scale", &DebugBox::getScale, &DebugBox::setScale, "The uniform scale factor.")
        .def_property("rotation", &DebugBox::getRotation, &DebugBox::setRotation,
                      "The rotation of this shape as Euler angles (pitch, yaw, roll).")
        .def_property("bound", &DebugBox::getBound, &DebugBox::setBound,
                      "The bounding size of the box. Final size = bound * scale.");

    py::class_<DebugSphere>(m, "DebugSphere", "Represents a debug sphere shape. Radius is controlled via scale.")
        .def(py::init<>())
        .def_property("color", &DebugSphere::getColor, &DebugSphere::setColor, "The color of this shape.")
        .def_property("scale", &DebugSphere::getScale, &DebugSphere::setScale, "The uniform scale factor.")
        .def_property("rotation", &DebugSphere::getRotation, &DebugSphere::setRotation,
                      "The rotation of this shape as Euler angles (pitch, yaw, roll).");

    py::class_<DebugCircle>(m, "DebugCircle", "Represents a debug 2D circle shape. Radius is controlled via scale.")
        .def(py::init<>())
        .def_property("color", &DebugCircle::getColor, &DebugCircle::setColor, "The color of this shape.")
        .def_property("scale", &DebugCircle::getScale, &DebugCircle::setScale, "The uniform scale factor.")
        .def_property("rotation", &DebugCircle::getRotation, &DebugCircle::setRotation,
                      "The rotation of this shape as Euler angles (pitch, yaw, roll).");

    py::class_<DebugLine>(m, "DebugLine", "Represents a debug line segment shape.")
        .def(py::init<>())
        .def_property("color", &DebugLine::getColor, &DebugLine::setColor, "The color of this shape.")
        .def_property("scale", &DebugLine::getScale, &DebugLine::setScale, "The uniform scale factor.")
        .def_property("rotation", &DebugLine::getRotation, &DebugLine::setRotation,
                      "The rotation of this shape as Euler angles (pitch, yaw, roll).")
        .def_property("length", &DebugLine::getLength, &DebugLine::setLength, "The length of the line segment.");

    py::class_<DebugArrow>(m, "DebugArrow", "Represents a debug arrow shape.")
        .def(py::init<>())
        .def_property("color", &DebugArrow::getColor, &DebugArrow::setColor, "The color of this shape.")
        .def_property("scale", &DebugArrow::getScale, &DebugArrow::setScale, "The uniform scale factor.")
        .def_property("rotation", &DebugArrow::getRotation, &DebugArrow::setRotation,
                      "The rotation of this shape as Euler angles (pitch, yaw, roll).")
        .def_property("length", &DebugArrow::getLength, &DebugArrow::setLength, "The length of the arrow.")
        .def_property("head_length", &DebugArrow::getHeadLength, &DebugArrow::setHeadLength,
                      "The length of the arrow's head.")
        .def_property("head_radius", &DebugArrow::getHeadRadius, &DebugArrow::setHeadRadius,
                      "The radius of the arrow's head.")
        .def_property("head_segments", &DebugArrow::getHeadSegments, &DebugArrow::setHeadSegments,
                      "The number of segments for the arrow head's base circle.");

    py::class_<DebugText>(m, "DebugText", "Represents a debug billboard text label.")
        .def(py::init<>())
        .def_property("color", &DebugText::getColor, &DebugText::setColor, "The color of this shape.")
        .def_property("scale", &DebugText::getScale, &DebugText::setScale, "The uniform scale factor.")
        .def_property("rotation", &DebugText::getRotation, &DebugText::setRotation,
                      "The rotation of this shape as Euler angles (pitch, yaw, roll).")
        .def_property("text", &DebugText::getText, &DebugText::setText, "The text displayed by this label.");
}

}  // namespace endstone::python
