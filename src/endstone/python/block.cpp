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

#include <format>

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

void init_block(py::module_ &m, py::classh<Block> &block)
{
    py::native_enum<BlockFace>(m, "BlockFace", "enum.Enum")
        .value("DOWN", BlockFace::Down)
        .value("UP", BlockFace::Up)
        .value("NORTH", BlockFace::North)
        .value("SOUTH", BlockFace::South)
        .value("WEST", BlockFace::West)
        .value("EAST", BlockFace::East)
        .finalize();

    auto block_type = def_registry_type(py::class_<BlockType>(m, "BlockType", "Represents a block type."));

    py::classh<BlockData>(m, "BlockData", "Represents the data related to a live block.")
        .def_property_readonly("type", &BlockData::getType, py::return_value_policy::reference,
                               "The block type represented by this block data.")
        .def_property_readonly("translation_key", &BlockData::getTranslationKey,
                               "The translation key for this block.")
        .def_property_readonly("block_states", &BlockData::getBlockStates, R"doc(
    The block states for this block.

    When passed into `Server.create_block_data(type, block_states)` these
    will unambiguously recreate this instance.
)doc")
        .def_property_readonly("runtime_id", &BlockData::getRuntimeId, "The runtime id for this block.")
        .def("__str__", [](const BlockData &self) { return std::format("{}", self); });

    block_type.def_property_readonly_static("AIR", id(BlockType::Air), "The identifier of the air block type.")
        .def_property_readonly("id", &BlockType::getId, "The identifier of this block type.")
        .def_property_readonly("translation_key", &BlockType::getTranslationKey,
                               "The translation key, suitable for use in a translation component.")
        .def_property_readonly("has_item_type", &BlockType::hasItemType,
                               "`True` if this `BlockType` has a corresponding `ItemType`.")
        .def("create_block_data", &BlockType::createBlockData, R"doc(
    Creates a new `BlockData` instance for this block type, with all properties initialized to defaults.

    Returns:
        A new `BlockData` instance.
)doc")
        .def_static("get", &BlockType::get, py::arg("name"), R"doc(
    Attempts to get the `BlockType` with the given name.

    Args:
        name: The identifier of the block type (e.g. `minecraft:stone`).

    Returns:
        The `BlockType`, or `None` if no block type with that name exists.
)doc", py::return_value_policy::reference);

    def_registry_type(py::class_<Biome>(m, "Biome", "Represents a biome."))
        .def_property_readonly("id", &Biome::getId, "The identifier of this biome.")
        .def_property_readonly("translation_key", &Biome::getTranslationKey,
                               "Raises, as Bedrock does not localize biome names.")
        .def_static("get", &Biome::get, py::arg("name"), R"doc(
    Attempts to get the `Biome` with the given name.

    Args:
        name: The identifier of the biome (e.g. `minecraft:plains`).

    Returns:
        The `Biome`, or `None` if no biome with that name exists.
)doc", py::return_value_policy::reference);

    py::classh<BlockState>(m, "BlockState", R"doc(
    Represents a captured state of a block, which will not update automatically.

    Unlike `Block`, which only one object can exist per coordinate, `BlockState` can
    exist multiple times for any given `Block`. Note that another plugin may change
    the state of the block, and you will not know, or they may change the block to
    another type entirely, causing your `BlockState` to become invalid.
)doc")
        .def_property_readonly("block", &BlockState::getBlock, "The block represented by this block state.")
        .def_property("type", &BlockState::getType, &BlockState::setType, py::return_value_policy::reference,
                      "The type of this block state.")
        .def_property("data", &BlockState::getData, &BlockState::setData, "The data for this block state.")
        .def_property_readonly("dimension", &BlockState::getDimension,
                               "The dimension which contains the block represented by this block state.")
        .def_property_readonly("x", &BlockState::getX, "X-coordinate of this block state.")
        .def_property_readonly("y", &BlockState::getY, "Y-coordinate of this block state.")
        .def_property_readonly("z", &BlockState::getZ, "Z-coordinate of this block state.")
        .def_property_readonly("location", &BlockState::getLocation, "The location of this block state.")
        .def("update", py::overload_cast<bool, bool>(&BlockState::update), py::arg("force") = false,
             py::arg("apply_physics") = true, R"doc(
    Attempts to update the block represented by this state, setting it to the new values defined by this state.

    Unless `force` is `True`, this will not modify the state of a block if it is no longer the
    same type as it was when this state was taken; in that case it returns `False`.

    If `force` is `True`, the block type is set to match the new state, the state data is applied,
    and `True` is returned.

    If `apply_physics` is `True`, a physics update is triggered on surrounding blocks, which
    could cause them to update or disappear.

    Args:
        force: `True` to forcefully set the state.
        apply_physics: `False` to cancel updating physics on surrounding blocks.

    Returns:
        `True` if the update was successful, `False` otherwise.
)doc")
        .def("__str__", [](const BlockState &self) { return std::format("{}", self); });

    py::classh<Container, BlockState>(m, "Container", R"doc(
    Represents a captured state of a container block, such as a chest.
)doc")
        .def_property_readonly("inventory", &Container::getInventory, py::return_value_policy::reference_internal, R"doc(
    The inventory of the block represented by this block state.

    If the block was changed to a different type in the meantime, the returned inventory might no
    longer be valid.
)doc");

    py::classh<Campfire, BlockState>(m, "Campfire", "Represents a captured state of a campfire.")
        .def_property_readonly("size", &Campfire::getSize, "The number of items this campfire can cook at once.")
        .def("get_item", &Campfire::getItem, py::arg("index"), R"doc(
    Gets the item currently cooking in the given slot.

    Args:
        index: The slot, between 0 and `size` - 1.

    Returns:
        The item, or `None` if the slot is empty.
)doc")
        .def("set_item", &Campfire::setItem, py::arg("index"), py::arg("item"), R"doc(
    Sets the item currently cooking in the given slot.

    Args:
        index: The slot, between 0 and `size` - 1.
        item: The item, or `None` to empty the slot.
)doc")
        .def("get_cook_time", &Campfire::getCookTime, py::arg("index"), R"doc(
    Gets how long the item in the given slot has been cooking for, in ticks.

    Args:
        index: The slot, between 0 and `size` - 1.

    Returns:
        The cook time, in ticks.
)doc")
        .def("set_cook_time", &Campfire::setCookTime, py::arg("index"), py::arg("cook_time"), R"doc(
    Sets how long the item in the given slot has been cooking for.

    Args:
        index: The slot, between 0 and `size` - 1.
        cook_time: The cook time, in ticks.
)doc");

    py::classh<CreatureSpawner, BlockState>(m, "CreatureSpawner", "Represents a captured state of a creature spawner.")
        .def_property("spawned_type", &CreatureSpawner::getSpawnedType, &CreatureSpawner::setSpawnedType,
                      py::return_value_policy::reference, "The type of actor this spawner will spawn.")
        .def_property("delay", &CreatureSpawner::getDelay, &CreatureSpawner::setDelay,
                      "The delay until the spawner spawns the next batch of actors, in ticks.")
        .def_property("min_spawn_delay", &CreatureSpawner::getMinSpawnDelay, &CreatureSpawner::setMinSpawnDelay,
                      "The minimum delay the spawner will wait between spawns, in ticks.")
        .def_property("max_spawn_delay", &CreatureSpawner::getMaxSpawnDelay, &CreatureSpawner::setMaxSpawnDelay,
                      "The maximum delay the spawner will wait between spawns, in ticks.")
        .def_property("spawn_count", &CreatureSpawner::getSpawnCount, &CreatureSpawner::setSpawnCount,
                      "How many actors the spawner attempts to spawn at a time.")
        .def_property("max_nearby_entities", &CreatureSpawner::getMaxNearbyEntities,
                      &CreatureSpawner::setMaxNearbyEntities,
                      "The maximum number of similar actors allowed nearby before the spawner stops spawning.")
        .def_property("required_player_range", &CreatureSpawner::getRequiredPlayerRange,
                      &CreatureSpawner::setRequiredPlayerRange,
                      "How far away a player must be for the spawner to be active, in blocks.")
        .def_property("spawn_range", &CreatureSpawner::getSpawnRange, &CreatureSpawner::setSpawnRange,
                      "The radius around the spawner in which actors are spawned, in blocks.");

    py::classh<Lectern, Container>(m, "Lectern", "Represents a captured state of a lectern.")
        .def_property("page", &Lectern::getPage, &Lectern::setPage, R"doc(
    The page currently displayed on the lectern.

    The page is 0-indexed, and is clamped to the number of pages in the book the lectern holds.
)doc");

    py::classh<Furnace, Container>(m, "Furnace", R"doc(
    Represents a captured state of a furnace.
)doc")
        .def_property("burn_time", &Furnace::getBurnTime, &Furnace::setBurnTime, R"doc(
    The burn time.

    This is the number of ticks the current fuel keeps the furnace lit for. A burn time greater
    than 0 will cause this block to be lit, whilst a time less than 0 will extinguish it.
)doc")
        .def_property("cook_time", &Furnace::getCookTime, &Furnace::setCookTime, R"doc(
    The cook time.

    This is the amount of time the item has been cooking for.
)doc");

    py::classh<ItemFrame, BlockState>(m, "ItemFrame", R"doc(
    Represents a captured state of an item frame.
)doc")
        .def_property("item", &ItemFrame::getItem, &ItemFrame::setItem, R"doc(
    The item in this frame.

    Reading this returns a defensive copy of the item, or `None` if the frame is empty. Assign
    `None` to empty the frame.

    Assigning resets the rotation of the frame, as placing an item in a frame does in-game.
)doc")
        .def_property("item_drop_chance", &ItemFrame::getItemDropChance, &ItemFrame::setItemDropChance, R"doc(
    The chance of the item being dropped upon this frame's destruction.

    A drop chance of 0.0 will never drop, a drop chance of 1.0 will always drop.
)doc")
        .def_property("rotation", &ItemFrame::getRotation, &ItemFrame::setRotation,
                      "The rotation of the frame's item.");

    py::classh<SignSide>(m, "SignSide", "Represents a side of a sign.")
        .def_property_readonly("lines", &SignSide::getLines,
                               "All the lines of text currently on this side of the sign.")
        .def("get_line", &SignSide::getLine, py::arg("index"), R"doc(
    Gets the line of text at the specified index on this side of the sign.

    For example, `get_line(0)` will return the first line of text.

    Args:
        index: The index of the line to get, between 0 and 3.

    Returns:
        The text of the line, empty if the line is blank.
)doc")
        .def("set_line", &SignSide::setLine, py::arg("index"), py::arg("line"), R"doc(
    Sets the line of text at the specified index on this side of the sign.

    For example, `set_line(0, "Line One")` will set the first line to "Line One".

    Args:
        index: The index of the line to set, between 0 and 3.
        line: The new text to set.
)doc")
        .def_property("glowing_text", &SignSide::isGlowingText, &SignSide::setGlowingText,
                      "Whether this side of the sign has glowing text.")
        .def_property("color", &SignSide::getColor, &SignSide::setColor, R"doc(
    The color of this side of the sign.

    Bedrock stores the text color of a sign as a color rather than as one of the dyes, so this is a
    `Color` where Bukkit has a `DyeColor`.
)doc");

    auto sign = py::classh<Sign, BlockState>(m, "Sign", "Represents a captured state of a sign.");

    py::native_enum<Sign::Side>(sign, "Side", "enum.Enum", "Represents a side of a sign.")
        .value("FRONT", Sign::Side::Front)
        .value("BACK", Sign::Side::Back)
        .finalize();

    sign.def("get_side", &Sign::getSide, py::arg("side"), py::return_value_policy::reference_internal, R"doc(
    Gets the side of this sign.

    Args:
        side: The side of the sign.

    Returns:
        The given side of the sign.
)doc")
        .def_property("waxed", &Sign::isWaxed, &Sign::setWaxed, R"doc(
    Whether this sign is waxed.

    A waxed sign cannot be edited by players.
)doc");

    block.def_property_readonly("type", &Block::getType, py::return_value_policy::reference,
                                "The type of the block.")
        .def("set_type", py::overload_cast<BlockTypeId, bool>(&Block::setType), py::arg("type"),
             py::arg("apply_physics") = true, R"doc(
    Sets the type of this block.

    Args:
        type: New type for this block (e.g. `minecraft:stone`).
        apply_physics: `False` to cancel physics on the changed block.
)doc")
        .def_property_readonly("data", &Block::getData, "The complete block data for this block.")
        .def("set_data", py::overload_cast<const BlockData &, bool>(&Block::setData), py::arg("data"),
             py::arg("apply_physics") = true, R"doc(
    Sets the complete data for this block.

    Args:
        data: New block-specific data.
        apply_physics: `False` to cancel physics on the changed block.
)doc")
        .def("get_relative", py::overload_cast<int, int, int>(&Block::getRelative), py::arg("offset_x"),
             py::arg("offset_y"), py::arg("offset_z"), R"doc(
    Gets the block at the given offsets.

    Args:
        offset_x: X-coordinate offset.
        offset_y: Y-coordinate offset.
        offset_z: Z-coordinate offset.

    Returns:
        `Block` at the given offsets.
)doc")
        .def("get_relative", py::overload_cast<BlockFace, int>(&Block::getRelative), py::arg("face"),
             py::arg("distance") = 1, R"doc(
    Gets the block at the given distance of the given face.

    Args:
        face: Face of this block to return.
        distance: Distance from this block to get the block at.

    Returns:
        `Block` at the given face.
)doc")
        .def_property_readonly("dimension", &Block::getDimension, "The dimension which contains this `Block`.")
        .def_property_readonly("biome", &Block::getBiome, py::return_value_policy::reference,
                               "The biome that this block resides in.")
        .def_property_readonly("x", &Block::getX, "X-coordinate of this block.")
        .def_property_readonly("y", &Block::getY, "Y-coordinate of this block.")
        .def_property_readonly("z", &Block::getZ, "Z-coordinate of this block.")
        .def_property_readonly("location", &Block::getLocation, "The location of this block.")
        .def("capture_state", &Block::captureState, R"doc(
    Captures the current state of this block.

    The returned object will never be updated, and you are not guaranteed that (for example) a
    sign is still a sign after you capture its state.

    Returns:
        A `BlockState` snapshot of the current state of this block.
)doc")
        .def("__str__", [](const Block &self) { return std::format("{}", self); });
}

}  // namespace endstone::python
