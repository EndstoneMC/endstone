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

    auto block_type = py::class_<BlockType>(m, "BlockType", "Represents a block type.");

    py::class_<BlockData>(m, "BlockData", "Represents the data related to a live block.")
        .def_property_readonly("type", &BlockData::getType, "The block type represented by this block data.")
        .def_property_readonly("translation_key", &BlockData::getTranslationKey,
                               "The translation key for this block.")
        .def_property_readonly("block_states", &BlockData::getBlockStates, R"doc(
    The block states for this block.

    When passed into ``Server.create_block_data(type, block_states)`` these
    will unambiguously recreate this instance.
)doc")
        .def_property_readonly("runtime_id", &BlockData::getRuntimeId, "The runtime id for this block.")
        .def("__str__", [](const BlockData &self) { return fmt::format("{}", self); });

    block_type.def_property_readonly("id", &BlockType::getId, "The identifier of this block type.")
        .def_property_readonly("translation_key", &BlockType::getTranslationKey,
                               "The translation key, suitable for use in a translation component.")
        .def_property_readonly("has_item_type", &BlockType::hasItemType,
                               "True if this BlockType has a corresponding ItemType.")
        .def("create_block_data", &BlockType::createBlockData, R"doc(
    Creates a new BlockData instance for this block type, with all properties initialized to defaults.

    Returns:
        A new BlockData instance.
)doc")
        .def_static("get", &BlockType::get, py::arg("name"), R"doc(
    Attempts to get the BlockType with the given name.

    Args:
        name: The identifier of the block type (e.g. ``minecraft:stone``).

    Returns:
        The BlockType, or None if no block type with that name exists.
)doc", py::return_value_policy::reference)
        .def("__str__", [](const BlockType &self) { return std::string(self.getId()); })
        .def("__repr__", [](const BlockType &self) { return fmt::format("BlockType({})", self.getId()); })
        .def("__hash__", [](const BlockType &self) { return std::hash<BlockTypeId>{}(self.getId()); })
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self == std::string_view())
        .def(py::self != std::string_view());

    py::class_<BlockState>(m, "BlockState", R"doc(
    Represents a captured state of a block, which will not update automatically.

    Unlike Block, which only one object can exist per coordinate, BlockState can
    exist multiple times for any given Block. Note that another plugin may change
    the state of the block, and you will not know, or they may change the block to
    another type entirely, causing your BlockState to become invalid.
)doc")
        .def_property_readonly("block", &BlockState::getBlock, "The block represented by this block state.")
        .def_property("type", &BlockState::getType, &BlockState::setType, "The type of this block state.")
        .def_property("data", &BlockState::getData, &BlockState::setData, "The data for this block state.")
        .def_property_readonly("dimension", &BlockState::getDimension, py::return_value_policy::reference,
                               "The dimension which contains the block represented by this block state.")
        .def_property_readonly("x", &BlockState::getX, "X-coordinate of this block state.")
        .def_property_readonly("y", &BlockState::getY, "Y-coordinate of this block state.")
        .def_property_readonly("z", &BlockState::getZ, "Z-coordinate of this block state.")
        .def_property_readonly("location", &BlockState::getLocation, "The location of this block state.")
        .def("update", py::overload_cast<bool, bool>(&BlockState::update), py::arg("force") = false,
             py::arg("apply_physics") = true, R"doc(
    Attempts to update the block represented by this state, setting it to the new values defined by this state.

    Unless ``force`` is True, this will not modify the state of a block if it is no longer the
    same type as it was when this state was taken; in that case it returns False.

    If ``force`` is True, the block type is set to match the new state, the state data is applied,
    and True is returned.

    If ``apply_physics`` is True, a physics update is triggered on surrounding blocks, which
    could cause them to update or disappear.

    Args:
        force: True to forcefully set the state.
        apply_physics: False to cancel updating physics on surrounding blocks.

    Returns:
        True if the update was successful, False otherwise.
)doc")
        .def("__str__", [](const BlockState &self) { return fmt::format("{}", self); });

    block.def_property_readonly("type", &Block::getType,
                                "The type of the block, as a string (e.g. ``minecraft:acacia_stairs``).")
        .def("set_type", py::overload_cast<std::string, bool>(&Block::setType), py::arg("type"),
             py::arg("apply_physics") = true, R"doc(
    Sets the type of this block.

    Args:
        type: New type for this block (e.g. ``minecraft:stone``).
        apply_physics: False to cancel physics on the changed block.
)doc")
        .def_property_readonly("data", &Block::getData, "The complete block data for this block.")
        .def("set_data", py::overload_cast<const BlockData &, bool>(&Block::setData), py::arg("data"),
             py::arg("apply_physics") = true, R"doc(
    Sets the complete data for this block.

    Args:
        data: New block-specific data.
        apply_physics: False to cancel physics on the changed block.
)doc")
        .def("get_relative", py::overload_cast<int, int, int>(&Block::getRelative), py::arg("offset_x"),
             py::arg("offset_y"), py::arg("offset_z"), R"doc(
    Gets the block at the given offsets.

    Args:
        offset_x: X-coordinate offset.
        offset_y: Y-coordinate offset.
        offset_z: Z-coordinate offset.

    Returns:
        Block at the given offsets.
)doc")
        .def("get_relative", py::overload_cast<BlockFace, int>(&Block::getRelative), py::arg("face"),
             py::arg("distance") = 1, R"doc(
    Gets the block at the given distance of the given face.

    Args:
        face: Face of this block to return.
        distance: Distance from this block to get the block at.

    Returns:
        Block at the given face.
)doc")
        .def_property_readonly("dimension", &Block::getDimension, "The dimension which contains this Block.",
                               py::return_value_policy::reference)
        .def_property_readonly("x", &Block::getX, "X-coordinate of this block.")
        .def_property_readonly("y", &Block::getY, "Y-coordinate of this block.")
        .def_property_readonly("z", &Block::getZ, "Z-coordinate of this block.")
        .def_property_readonly("location", &Block::getLocation, "The location of this block.")
        .def("capture_state", &Block::captureState, R"doc(
    Captures the current state of this block.

    The returned object will never be updated, and you are not guaranteed that (for example) a
    sign is still a sign after you capture its state.

    Returns:
        A BlockState snapshot of the current state of this block.
)doc")
        .def("__str__", [](const Block &self) { return fmt::format("{}", self); });
}

}  // namespace endstone::python
