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

#include <fmt/format.h>

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

void init_block(py::module_ &m, py::class_<Block> &block)
{
    py::native_enum<BlockFace>(m, "BlockFace", "enum.Enum")
        .value("DOWN", BlockFace::Down)
        .value("UP", BlockFace::Up)
        .value("NORTH", BlockFace::North)
        .value("SOUTH", BlockFace::South)
        .value("WEST", BlockFace::West)
        .value("EAST", BlockFace::East)
        .finalize();

    py::class_<BlockData>(m, "BlockData", "Represents the data related to a live block")
        .def_property_readonly("type", &BlockData::getType, "Get the block type represented by this block data.")
        .def_property_readonly("translation_key", &BlockData::getTranslationKey, "Gets the translation key for this block.")
        .def_property_readonly("block_states", &BlockData::getBlockStates, "Gets the block states for this block.")
        .def_property_readonly("runtime_id", &BlockData::getRuntimeId, "Gets the runtime id for this block.")
        .def("__str__", [](const BlockData &self) { return fmt::format("{}", self); });

    py::class_<BlockState>(m, "BlockState",
                           "Represents a captured state of a block, which will not update automatically.")
        .def_property_readonly("block", &BlockState::getBlock, "Gets the block represented by this block state.")
        .def_property("type", &BlockState::getType, &BlockState::setType, "Gets or sets the type of this block state.")
        .def_property("data", &BlockState::getData, &BlockState::setData, "Gets or sets the data for this block state.")
        .def_property_readonly("dimension", &BlockState::getDimension, py::return_value_policy::reference,
                               "Gets the dimension which contains the block represented by this block state.")
        .def_property_readonly("x", &BlockState::getX, "Gets the x-coordinate of this block state.")
        .def_property_readonly("y", &BlockState::getY, "Gets the y-coordinate of this block state.")
        .def_property_readonly("z", &BlockState::getZ, "Gets the z-coordinate of this block state.")
        .def_property_readonly("location", &BlockState::getLocation, "Gets the location of this block state.")
        .def("update", py::overload_cast<bool, bool>(&BlockState::update), py::arg("force") = false,
             py::arg("apply_physics") = true, "Attempts to update the block represented by this state.")
        .def("__str__", [](const BlockState &self) { return fmt::format("{}", self); });

    block.def_property_readonly("type", &Block::getType, "Gets or sets the type of the block.")
        .def("set_type", py::overload_cast<std::string, bool>(&Block::setType), py::arg("type"),
             py::arg("apply_physics") = true, "Sets the type of this block")
        .def_property_readonly("data", &Block::getData, "Gets the complete data for this block")
        .def("set_data", py::overload_cast<const BlockData &, bool>(&Block::setData), py::arg("data"),
             py::arg("apply_physics") = true, "Sets the complete data for this block")
        .def("get_relative", py::overload_cast<int, int, int>(&Block::getRelative), py::arg("offset_x"),
             py::arg("offset_y"), py::arg("offset_z"), "Gets the block at the given offsets")
        .def("get_relative", py::overload_cast<BlockFace, int>(&Block::getRelative), py::arg("face"),
             py::arg("distance") = 1, "Gets the block at the given distance of the given face")
        .def_property_readonly("dimension", &Block::getDimension, "Gets the dimension which contains this Block",
                               py::return_value_policy::reference)
        .def_property_readonly("x", &Block::getX, "Gets the x-coordinate of this block")
        .def_property_readonly("y", &Block::getY, "Gets the y-coordinate of this block")
        .def_property_readonly("z", &Block::getZ, "Gets the z-coordinate of this block")
        .def_property_readonly("location", &Block::getLocation, "Gets the Location of the block")
        .def("capture_state", &Block::captureState,
             "Captures the current state of this block. The returned object will never be updated, and you are not "
             "guaranteed that (for example) a sign is still a sign after you capture its state.")
        .def("__str__", [](const Block &self) { return fmt::format("{}", self); });
}

}  // namespace endstone::python
