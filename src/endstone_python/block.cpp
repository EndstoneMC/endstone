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

#include "endstone/block/block.h"

#include <pybind11/pybind11.h>

#include "endstone/block/block_face.h"
#include "endstone/level/dimension.h"

namespace py = pybind11;

namespace endstone::detail {

void init_block(py::module_ &m, py::class_<Block> &block)
{
    py::enum_<BlockFace>(m, "BlockFace")
        .value("SELF", BlockFace::Self)
        .value("NORTH", BlockFace::North)
        .value("EAST", BlockFace::East)
        .value("SOUTH", BlockFace::South)
        .value("WEST", BlockFace::West)
        .value("UP", BlockFace::Up)
        .value("DOWN", BlockFace::Down)
        .value("NORTH_EAST", BlockFace::NorthEast)
        .value("NORTH_WEST", BlockFace::NorthWest)
        .value("SOUTH_EAST", BlockFace::SouthEast)
        .value("SOUTH_WEST", BlockFace::SouthWest);

    block.def_property_readonly("type", &Block::getType, "Get the type of the block.")
        .def("get_relative", py::overload_cast<int, int, int>(&Block::getRelative), py::arg("offset_x"),
             py::arg("offset_y"), py::arg("offset_z"), "Gets the block at the given offsets")
        .def("get_relative", py::overload_cast<BlockFace, int>(&Block::getRelative), py::arg("face"),
             py::arg("distance") = 1, "Gets the block at the given distance of the given face")
        .def_property_readonly("dimension", &Block::getDimension, "Gets the dimension which contains this Block",
                               py::return_value_policy::reference)
        .def_property_readonly("x", &Block::getX, "Gets the x-coordinate of this block")
        .def_property_readonly("y", &Block::getY, "Gets the y-coordinate of this block")
        .def_property_readonly("z", &Block::getZ, "Gets the z-coordinate of this block")
        .def_property_readonly("location", &Block::getLocation, "Gets the Location of the block");
}

}  // namespace endstone::detail
