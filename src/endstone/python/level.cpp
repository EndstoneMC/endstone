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
Location create_location(Dimension &dimension, float x, float y, float z, float pitch, float yaw)
{
    return {dimension, x, y, z, pitch, yaw};
}
}  // namespace

void init_level(py::module_ &m, py::class_<Level> &level, py::class_<Dimension> &dimension,
                py::class_<Location> &location)
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
             py::return_value_policy::reference, "Drops an item at the specified Location")
        .def("spawn_actor", &Dimension::spawnActor, py::arg("location"), py::arg("type"),
             py::return_value_policy::reference, "Creates an actor at the given Location")
        .def_property_readonly("actors", &Dimension::getActors, py::return_value_policy::reference_internal,
                               "Get a list of all actors in this dimension");

    level.def_property_readonly("name", &Level::getName, "Gets the unique name of this level")
        .def_property_readonly("actors", &Level::getActors, "Get a list of all actors in this level",
                               py::return_value_policy::reference_internal)
        .def_property("time", &Level::getTime, &Level::setTime, "Gets and sets the relative in-game time on the server")
        .def_property_readonly("dimensions", &Level::getDimensions, "Gets a list of all dimensions within this level.",
                               py::return_value_policy::reference_internal)
        .def("get_dimension", &Level::getDimension, py::arg("name"), "Gets the dimension with the given name.",
             py::return_value_policy::reference)
        .def_property_readonly("seed", &Level::getSeed, "Gets the Seed for this level.");

    location
        .def(py::init(&create_location), py::arg("dimension"), py::arg("x"), py::arg("y"), py::arg("z"),
             py::arg("pitch") = 0.0, py::arg("yaw") = 0.0)
        .def_property("dimension", &Location::getDimension, &Location::setDimension, py::return_value_policy::reference,
                      "The Dimension that contains this position")
        .def_property_readonly("block", &Location::getBlock, "Gets the block at the represented location")
        .def_property("pitch", &Location::getPitch, &Location::setPitch,
                      "The pitch of this location, measured in degrees.")
        .def_property("yaw", &Location::getYaw, &Location::setYaw, "The yaw of this location, measured in degrees.")
        .def_property(
            "x", &Location::getX, [](Location &self, float x) { self.setX(x); }, "The x-coordinate of this location")
        .def_property(
            "y", &Location::getY, [](Location &self, float y) { self.setY(y); }, "The y-coordinate of this location")
        .def_property(
            "z", &Location::getZ, [](Location &self, float z) { self.setZ(z); }, "The z-coordinate of this location")
        .def_property_readonly(
            "block_x", &Location::getBlockX,
            "Gets the floored value of the X component, indicating the block that this location is contained with.")
        .def_property_readonly(
            "block_y", &Location::getBlockY,
            "Gets the floored value of the Y component, indicating the block that this location is contained with.")
        .def_property_readonly(
            "block_z", &Location::getBlockZ,
            "Gets the floored value of the Z component, indicating the block that this location is contained with.")
        .def_property("direction", &Location::getDirection, &Location::setDirection,
                      "Gets or sets a vector of yaw and pitch that points in the direction of the vector")
        .def_property_readonly("length", &Location::length, "The magnitude of the Location")
        .def_property_readonly("length_squared", &Location::lengthSquared, "The squared magnitude of the Location")
        .def("distance", &Location::distance, py::arg("other"), "The distance between this Location and another")
        .def("distance_squared", &Location::distanceSquared, py::arg("other"),
             "The squared distance between this Location and another")
        .def(py::self += py::self)
        .def(py::self += Vector())
        .def(py::self -= py::self)
        .def(py::self -= Vector())
        .def(py::self *= float())
        .def("zero", &Location::zero, "Zero this vector's components.", py::return_value_policy::reference)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__repr__", [](const Location &self) { return fmt::format("{}", self); })
        .def("__str__", [](const Location &self) { return fmt::format("{}", self); })
        .def_static("normalize_yaw", &Location::normalizeYaw, py::arg("yaw"),
                    "Normalizes the given yaw angle to a value between `+/-180` degrees.")
        .def_static("normalize_pitch", &Location::normalizePitch, py::arg("pitch"),
                    "Normalizes the given pitch angle to a value between `+/-90` degrees.");
}
}  // namespace endstone::python
