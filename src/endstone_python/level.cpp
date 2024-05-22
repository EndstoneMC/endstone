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

#include "endstone/level/level.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/level/dimension.h"
#include "endstone/level/location.h"
#include "endstone/level/position.h"

namespace py = pybind11;

namespace endstone::detail {

void init_level(py::module_ &m)
{
    py::class_<Level>(m, "Level")
        .def_property_readonly("name", &Level::getName, "Gets the unique name of this level")
        .def_property_readonly("actors", &Level::getActors, "Get a list of all actors in this level")
        .def_property("time", &Level::getTime, &Level::setTime,
                      "Gets and sets the relative in-game time on the server");

    py::class_<Dimension>(m, "Dimension");

    py::class_<Position, Vector<float>>(m, "Position")
        .def(py::init<Dimension *, float, float, float>(), py::arg("dimension"), py::arg("x"), py::arg("y"),
             py::arg("z"))
        .def_property("dimension", &Position::getDimension, &Position::setDimension, py::return_value_policy::reference,
                      "The Dimension that contains this position");
    // TODO(fixme): add __repr__

    py::class_<Location, Position>(m, "Location")
        .def(py::init<Dimension *, float, float, float, float, float>(), py::arg("dimension"), py::arg("x"),
             py::arg("y"), py::arg("z"), py::arg("pitch") = 0.0, py::arg("yaw") = 0.0)
        .def_property("pitch", &Location::getPitch, &Location::setPitch,
                      "The pitch of this location, measured in degrees.")
        .def_property("yaw", &Location::getYaw, &Location::setYaw, "The yaw of this location, measured in degrees.");
    // TODO(fixme): add __repr__
}

}  // namespace endstone::detail
