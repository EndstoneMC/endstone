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
namespace {
Location create_location(float x, float y, float z, float pitch, float yaw, Dimension *dimension)
{
    if (dimension == nullptr) {
        return Location(x, y, z, pitch, yaw);
    }
    return Location(x, y, z, pitch, yaw, *dimension);
}
}  // namespace

void init_level(py::module_ &m, py::class_<Level> &level, py::class_<Dimension> &dimension,
                py::class_<Location, Vector> &location)
{
    py::class_<Chunk>(m, "Chunk", "Represents a chunk of blocks.")
        .def_property_readonly("x", &Chunk::getX, "Gets the X-coordinate of this chunk")
        .def_property_readonly("z", &Chunk::getZ, "Gets the Z-coordinate of this chunk")
        .def_property_readonly("level", &Chunk::getLevel, "Gets the level containing this chunk",
                               py::return_value_policy::reference)
        .def_property_readonly("dimension", &Chunk::getDimension, "Gets the dimension containing this chunk",
                               py::return_value_policy::reference)
        .def("__repr__", [](const Chunk &self) { return fmt::format("{}", self); })
        .def("__str__", [](const Chunk &self) { return fmt::format("{}", self); });

    py::native_enum<Dimension::Type>(dimension, "Type", "enum.Enum", "Represents various dimension types.")
        .value("OVERWORLD", Dimension::Type::Overworld)
        .value("NETHER", Dimension::Type::Nether)
        .value("THE_END", Dimension::Type::TheEnd)
        .value("CUSTOM", Dimension::Type::Custom)
        .export_values()
        .finalize();

    dimension.def_property_readonly("name", &Dimension::getName, "Gets the name of this dimension")
        .def_property_readonly("type", &Dimension::getType, "Gets the type of this dimension")
        .def_property_readonly("level", &Dimension::getLevel, "Gets the level to which this dimension belongs",
                               py::return_value_policy::reference)
        .def("get_block_at", py::overload_cast<Location>(&Dimension::getBlockAt, py::const_),
             py::arg("location").noconvert(), "Gets the Block at the given Location")
        .def("get_block_at", py::overload_cast<int, int, int>(&Dimension::getBlockAt, py::const_), py::arg("x"),
             py::arg("y"), py::arg("z"), "Gets the Block at the given coordinates")
        .def("get_highest_block_y_at", &Dimension::getHighestBlockYAt, py::arg("x"), py::arg("z"),
             "Gets the highest non-empty (impassable) coordinate at the given coordinates.")
        .def("get_highest_block_at", py::overload_cast<Location>(&Dimension::getHighestBlockAt, py::const_),
             py::arg("location").noconvert(), "Gets the highest non-empty (impassable) block at the given Location.")
        .def("get_highest_block_at", py::overload_cast<int, int>(&Dimension::getHighestBlockAt, py::const_),
             py::arg("x"), py::arg("z"), "Gets the highest non-empty (impassable) block at the given coordinates.")
        .def_property_readonly("loaded_chunks", &Dimension::getLoadedChunks, "Gets a list of all loaded Chunks")
        .def("drop_item", &Dimension::dropItem, py::arg("location"), py::arg("item"),
             "Drops an item at the specified Location");

    level.def_property_readonly("name", &Level::getName, "Gets the unique name of this level")
        .def_property_readonly("actors", &Level::getActors, "Get a list of all actors in this level",
                               py::return_value_policy::reference_internal)
        .def_property("time", &Level::getTime, &Level::setTime, "Gets and sets the relative in-game time on the server")
        .def_property_readonly("dimensions", &Level::getDimensions, "Gets a list of all dimensions within this level.",
                               py::return_value_policy::reference_internal)
        .def("get_dimension", &Level::getDimension, py::arg("name"), "Gets the dimension with the given name.",
             py::return_value_policy::reference)
        .def_property_readonly("seed", &Level::getSeed, "Gets the Seed for this level.");

    auto location_to_string = [](const Location &l) {
        return fmt::format("Location(dimension={}, x={}, y={}, z={}, pitch={}, yaw={})",
                           l.getDimension() ? l.getDimension()->getName() : "None", l.getX(), l.getY(), l.getZ(),
                           l.getPitch(), l.getYaw());
    };
    location
        .def(py::init(&create_location), py::arg("x"), py::arg("y"), py::arg("z"), py::arg("pitch") = 0.0,
             py::arg("yaw") = 0.0, py::arg("dimension") = nullptr)
        .def_property("pitch", &Location::getPitch, &Location::setPitch,
                      "The pitch of this location, measured in degrees.")
        .def_property("yaw", &Location::getYaw, &Location::setYaw, "The yaw of this location, measured in degrees.")
        .def_property("dimension", &Location::getDimension, &Location::setDimension, py::return_value_policy::reference,
                      "The Dimension that contains this position")
        .def_property_readonly(
            "block_x", &Location::getBlockX,
            "Gets the floored value of the X component, indicating the block that this location is contained with.")
        .def_property_readonly(
            "block_y", &Location::getBlockY,
            "Gets the floored value of the Y component, indicating the block that this location is contained with.")
        .def_property_readonly(
            "block_z", &Location::getBlockZ,
            "Gets the floored value of the Z component, indicating the block that this location is contained with.")
        .def("__repr__", location_to_string)
        .def("__str__", location_to_string);
}

}  // namespace endstone::python
