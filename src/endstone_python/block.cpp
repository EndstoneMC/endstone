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

#include <algorithm>
#include <string>

#include <fmt/format.h>
#include <pybind11/pybind11.h>

#include "endstone/block/block_data.h"
#include "endstone/block/block_face.h"
#include "endstone/level/dimension.h"

namespace py = pybind11;

namespace endstone::detail {

void init_block(py::module_ &m, py::class_<Block> &block)
{
    py::enum_<BlockFace>(m, "BlockFace")
        .value("DOWN", BlockFace::Down)
        .value("UP", BlockFace::Up)
        .value("NORTH", BlockFace::North)
        .value("SOUTH", BlockFace::South)
        .value("WEST", BlockFace::West)
        .value("EAST", BlockFace::East);

    py::class_<BlockData, std::shared_ptr<BlockData>>(m, "BlockData", "Represents the data related to a live block")
        .def_property_readonly("type", &BlockData::getType, "Get the block type represented by this block data.")
        .def_property_readonly(
            "block_states",
            [](const BlockData &self) -> py::dict {
                auto states = self.getBlockStates();
                if (states.empty()) {
                    return py::dict{};
                }
                states.front() = '{';
                states.back() = '}';
                std::replace(states.begin(), states.end(), '=', ':');
                const auto json_module = pybind11::module::import("json");
                const auto json_loads = json_module.attr("loads");
                return json_loads(states);
            },
            "Gets the block states as a dict.")
        .def("__str__", [](const BlockData &self) {
            return fmt::format("BlockData(type={},block_states={})", self.getType(), self.getBlockStates());
        });

    block
        .def_property("type", &Block::getType, py::overload_cast<std::string>(&Block::setType),
                      "Gets or sets the type of the block.")
        .def("set_type", py::overload_cast<std::string, bool>(&Block::setType), py::arg("type"),
             py::arg("apply_physics") = true, "Sets the type of this block")
        .def_property("data", &Block::getData, py::overload_cast<std::shared_ptr<BlockData>>(&Block::setData),
                      "Gets or sets the complete data for this block")
        .def("set_data", py::overload_cast<std::shared_ptr<BlockData>, bool>(&Block::setData), py::arg("data"),
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
        .def("__str__", [](const Block &self) {
            return fmt::format("Block(pos=({},{},{}),type={},data={}{})", self.getX(), self.getY(), self.getZ(),
                               self.getType(), self.getData()->getType(), self.getData()->getBlockStates());
        });
}

}  // namespace endstone::detail
