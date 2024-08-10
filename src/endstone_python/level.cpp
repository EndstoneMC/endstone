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
    auto level = py::class_<Level>(m, "Level");
    auto dimension = py::class_<Dimension>(m, "Dimension", "Represents a dimension within a Level.");

    py::enum_<Dimension::Type>(dimension, "Type", "Represents various dimension types.")
        .value("OVERWORLD", Dimension::Type::Overworld)
        .value("NETHER", Dimension::Type::Nether)
        .value("THE_END", Dimension::Type::TheEnd)
        .value("CUSTOM", Dimension::Type::Custom)
        .export_values();

    dimension.def_property_readonly("name", &Dimension::getName, "Gets the name of this dimension")
        .def_property_readonly("type", &Dimension::getType, "Gets the type of this dimension")
        .def_property_readonly("level", &Dimension::getLevel, "Gets the level to which this dimension belongs",
                               py::return_value_policy::reference)
        .def("get_block_at", &Dimension::getBlockAt, py::arg("x"), py::arg("y"), py::arg("z"),
             "Gets the Block at the given coordinates");

    level.def_property_readonly("name", &Level::getName, "Gets the unique name of this level")
        .def_property_readonly("actors", &Level::getActors, "Get a list of all actors in this level",
                               py::return_value_policy::reference_internal)
        .def_property("time", &Level::getTime, &Level::setTime, "Gets and sets the relative in-game time on the server")
        .def_property_readonly("dimensions", &Level::getDimensions, "Gets a list of all dimensions within this level.",
                               py::return_value_policy::reference_internal)
        .def("get_dimension", &Level::getDimension, py::arg("name"), "Gets the dimension with the given name.",
             py::return_value_policy::reference);

    auto position_to_string = [](const Position &p) {
        return fmt::format("Position(dimension={}, x={}, y={}, z={})",
                           p.getDimension() ? p.getDimension()->getName() : "None", p.getX(), p.getY(), p.getZ());
    };
    py::class_<Position, Vector<float>>(m, "Position",
                                        "Represents a 3-dimensional position in a dimension within a level.")
        .def(py::init<Dimension *, float, float, float>(), py::arg("dimension"), py::arg("x"), py::arg("y"),
             py::arg("z"))
        .def_property("dimension", &Position::getDimension, &Position::setDimension, py::return_value_policy::reference,
                      "The Dimension that contains this position")
        .def("__repr__", position_to_string)
        .def("__str__", position_to_string);

    auto location_to_string = [](const Location &l) {
        return fmt::format("Location(dimension={}, x={}, y={}, z={}, pitch={}, yaw={})",
                           l.getDimension() ? l.getDimension()->getName() : "None", l.getX(), l.getY(), l.getZ(),
                           l.getPitch(), l.getYaw());
    };
    py::class_<Location, Position>(m, "Location", "Represents a 3-dimensional location in a dimension within a level.")
        .def(py::init<Dimension *, float, float, float, float, float>(), py::arg("dimension"), py::arg("x"),
             py::arg("y"), py::arg("z"), py::arg("pitch") = 0.0, py::arg("yaw") = 0.0)
        .def_property("pitch", &Location::getPitch, &Location::setPitch,
                      "The pitch of this location, measured in degrees.")
        .def_property("yaw", &Location::getYaw, &Location::setYaw, "The yaw of this location, measured in degrees.")
        .def("__repr__", location_to_string)
        .def("__str__", location_to_string);
}

}  // namespace endstone::detail
