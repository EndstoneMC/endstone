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
        .def_property_readonly("x", &Chunk::getX, "X-coordinate of this chunk.")
        .def_property_readonly("z", &Chunk::getZ, "Z-coordinate of this chunk.")
        .def_property_readonly("level", &Chunk::getLevel, "The level containing this chunk.",
                               py::return_value_policy::reference)
        .def_property_readonly("dimension", &Chunk::getDimension, "The dimension containing this chunk.",
                               py::return_value_policy::reference)
        .def("__repr__", [](const Chunk &self) { return std::format("{}", self); })
        .def("__str__", [](const Chunk &self) { return std::format("{}", self); });

    dimension
        .def_property_readonly_static("OVERWORLD", [](const py::object &) { return Dimension::Overworld; })
        .def_property_readonly_static("NETHER", [](const py::object &) { return Dimension::Nether; })
        .def_property_readonly_static("THE_END", [](const py::object &) { return Dimension::TheEnd; })
        .def_property_readonly("id", &Dimension::getId, "The identifier of this dimension.")
        .def_property_readonly("translation_key", &Dimension::getTranslationKey,
                               "The translation key, suitable for use in a translation component.")
        .def_property_readonly("level", &Dimension::getLevel, "The level to which this dimension belongs.",
                               py::return_value_policy::reference)
        .def("get_block_at", py::overload_cast<Location>(&Dimension::getBlockAt, py::const_),
             py::arg("location").noconvert(), R"doc(
    Gets the Block at the given Location.

    Args:
        location: Location of the block.

    Returns:
        Block at the given coordinates.
)doc")
        .def("get_block_at", py::overload_cast<int, int, int>(&Dimension::getBlockAt, py::const_), py::arg("x"),
             py::arg("y"), py::arg("z"), R"doc(
    Gets the Block at the given coordinates.

    Args:
        x: X-coordinate of the block.
        y: Y-coordinate of the block.
        z: Z-coordinate of the block.

    Returns:
        Block at the given coordinates.
)doc")
        .def("get_highest_block_y_at", &Dimension::getHighestBlockYAt, py::arg("x"), py::arg("z"), R"doc(
    Gets the highest non-empty (impassable) coordinate at the given coordinates.

    Args:
        x: X-coordinate of the blocks.
        z: Z-coordinate of the blocks.

    Returns:
        Y-coordinate of the highest non-empty block.
)doc")
        .def("get_highest_block_at", py::overload_cast<Location>(&Dimension::getHighestBlockAt, py::const_),
             py::arg("location").noconvert(), R"doc(
    Gets the highest non-empty (impassable) block at the given Location.

    Args:
        location: Coordinates to get the highest block.

    Returns:
        Highest non-empty block.
)doc")
        .def("get_highest_block_at", py::overload_cast<int, int>(&Dimension::getHighestBlockAt, py::const_),
             py::arg("x"), py::arg("z"), R"doc(
    Gets the highest non-empty (impassable) block at the given coordinates.

    Args:
        x: X-coordinate of the block.
        z: Z-coordinate of the block.

    Returns:
        Highest non-empty block.
)doc")
        .def_property_readonly("loaded_chunks", &Dimension::getLoadedChunks, "A list of all loaded Chunks.")
        .def("drop_item", &Dimension::dropItem, py::arg("location"), py::arg("item"),
             py::return_value_policy::reference, R"doc(
    Drops an item at the specified Location.

    Args:
        location: Location to drop the item.
        item: ItemStack to drop.

    Returns:
        Item entity created as a result of this method.
)doc")
        .def("spawn_actor", &Dimension::spawnActor, py::arg("location"), py::arg("type"),
             py::return_value_policy::reference, R"doc(
    Creates an actor at the given Location.

    Args:
        location: The location to spawn the actor.
        type: The actor type to spawn.

    Returns:
        Resulting Actor of this method.
)doc")
        .def_property_readonly("actors", &Dimension::getActors, py::return_value_policy::reference_internal,
                               "A list of all actors currently residing in this dimension.");

    level.def_property_readonly("name", &Level::getName, "The unique name of this level.")
        .def_property_readonly("actors", &Level::getActors, "A list of all actors currently residing in this level.",
                               py::return_value_policy::reference_internal)
        .def_property("time", &Level::getTime, &Level::setTime, "The relative in-game time of this level.")
        .def_property_readonly("dimensions", &Level::getDimensions, "A list of all dimensions within this level.",
                               py::return_value_policy::reference_internal)
        .def("get_dimension", &Level::getDimension, py::arg("id"), R"doc(
    Gets the dimension with the given id.

    Args:
        id: The id of the dimension to retrieve.

    Returns:
        The Dimension with the given id, or None if none exists.
)doc",
             py::return_value_policy::reference)
        .def_property_readonly("seed", &Level::getSeed, "The Seed for this level.");

    location
        .def(py::init(&create_location), py::arg("dimension"), py::arg("x"), py::arg("y"), py::arg("z"),
             py::arg("pitch") = 0.0, py::arg("yaw") = 0.0)
        .def_property("dimension", &Location::getDimension, &Location::setDimension, py::return_value_policy::reference,
                      "The Dimension that contains this position.")
        .def_property_readonly("block", &Location::getBlock, "The block at the represented location.")
        .def_property("pitch", &Location::getPitch, &Location::setPitch,
                      "The pitch of this location, measured in degrees.")
        .def_property("yaw", &Location::getYaw, &Location::setYaw, "The yaw of this location, measured in degrees.")
        .def_property(
            "x", &Location::getX, [](Location &self, float x) { self.setX(x); }, "The x-coordinate of this location.")
        .def_property(
            "y", &Location::getY, [](Location &self, float y) { self.setY(y); }, "The y-coordinate of this location.")
        .def_property(
            "z", &Location::getZ, [](Location &self, float z) { self.setZ(z); }, "The z-coordinate of this location.")
        .def_property_readonly(
            "block_x", &Location::getBlockX,
            "The floored value of the X component, indicating the block that this location is contained with.")
        .def_property_readonly(
            "block_y", &Location::getBlockY,
            "The floored value of the Y component, indicating the block that this location is contained with.")
        .def_property_readonly(
            "block_z", &Location::getBlockZ,
            "The floored value of the Z component, indicating the block that this location is contained with.")
        .def_property("direction", &Location::getDirection, &Location::setDirection,
                      "A unit-vector pointing in the direction that this Location is facing.")
        .def_property_readonly("length", &Location::length, R"doc(
    The magnitude of the location, defined as ``sqrt(x^2+y^2+z^2)``.

    Not world-aware and orientation independent.
)doc")
        .def_property_readonly("length_squared", &Location::lengthSquared, R"doc(
    The magnitude of the location squared.

    Not world-aware and orientation independent.
)doc")
        .def("distance", &Location::distance, py::arg("other"), R"doc(
    Get the distance between this location and another.

    Args:
        other: The other location.

    Returns:
        The distance.
)doc")
        .def("distance_squared", &Location::distanceSquared, py::arg("other"), R"doc(
    Get the squared distance between this location and another.

    Args:
        other: The other location.

    Returns:
        The distance.
)doc")
        .def(py::self += py::self)
        .def(py::self += Vector())
        .def(py::self -= py::self)
        .def(py::self -= Vector())
        .def(py::self *= float())
        .def("zero", &Location::zero, "Zero this location's components. Not world-aware.",
             py::return_value_policy::reference)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__repr__", [](const Location &self) { return std::format("{}", self); })
        .def("__str__", [](const Location &self) { return std::format("{}", self); })
        .def_static("normalize_yaw", &Location::normalizeYaw, py::arg("yaw"), R"doc(
    Normalizes the given yaw angle to a value between ``+/-180`` degrees.

    Args:
        yaw: The yaw in degrees.

    Returns:
        The normalized yaw in degrees.
)doc")
        .def_static("normalize_pitch", &Location::normalizePitch, py::arg("pitch"), R"doc(
    Normalizes the given pitch angle to a value between ``+/-90`` degrees.

    Args:
        pitch: The pitch in degrees.

    Returns:
        The normalized pitch in degrees.
)doc");
}
}  // namespace endstone::python
