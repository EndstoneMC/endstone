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
Location create_location(const Nullable<Dimension> &dimension, float x, float y, float z, float pitch, float yaw)
{
    return {dimension, x, y, z, pitch, yaw};
}
}  // namespace

void init_level(py::module_ &m, py::classh<Level> &level, py::classh<Dimension> &dimension,
                py::class_<Location> &location)
{
    py::classh<Chunk>(m, "Chunk", "Represents a chunk of blocks.")
        .def_property_readonly("x", &Chunk::getX, "X-coordinate of this chunk.")
        .def_property_readonly("z", &Chunk::getZ, "Z-coordinate of this chunk.")
        .def_property_readonly("level", &Chunk::getLevel, "The level containing this chunk.",
                               py::return_value_policy::reference)
        .def_property_readonly("dimension", &Chunk::getDimension, "The dimension containing this chunk.")
        .def_property_readonly("is_loaded", &Chunk::isLoaded, "Whether this chunk is loaded.")
        .def("load", py::overload_cast<bool>(&Chunk::load), py::arg("generate") = true, R"doc(
    Requests this chunk to be loaded, and keeps it resident until it is released again.

    See `Dimension.load_chunk` for how the hold behaves.

    Args:
        generate: Whether to generate the chunk if it does not exist yet.

    Returns:
        ``False`` if ``generate`` is ``False`` and the chunk has not been generated, otherwise ``True``.
)doc")
        .def("unload", &Chunk::unload, R"doc(
    Releases the hold that ``load`` placed on this chunk, and unloads it if nothing else keeps it resident.

    Returns:
        ``True`` if the chunk is no longer loaded, otherwise ``False``.
)doc")
        .def("add_plugin_chunk_ticket", &Chunk::addPluginChunkTicket, py::arg("plugin"), R"doc(
    Adds a plugin ticket for this chunk, loading it if it is not already loaded.

    Args:
        plugin: `Plugin` taking the ticket.

    Returns:
        ``True`` if a plugin ticket was added, ``False`` if the plugin already holds one for this chunk.
)doc")
        .def("remove_plugin_chunk_ticket", &Chunk::removePluginChunkTicket, py::arg("plugin"), R"doc(
    Removes the given plugin's ticket for this chunk.

    Args:
        plugin: `Plugin` whose ticket to remove.

    Returns:
        ``True`` if a plugin ticket was removed, ``False`` if the plugin holds none for this chunk.
)doc")
        .def_property_readonly("plugin_chunk_tickets", &Chunk::getPluginChunkTickets,
                               "The `Plugin`s holding a ticket for this chunk.",
                               py::return_value_policy::reference)
        .def("__repr__", [](const Chunk &self) { return std::format("{}", self); })
        .def("__str__", [](const Chunk &self) { return std::format("{}", self); });

    dimension
        .def_property_readonly_static("OVERWORLD", id(Dimension::Overworld),
                                      "The identifier of the Overworld dimension.")
        .def_property_readonly_static("NETHER", id(Dimension::Nether), "The identifier of the Nether dimension.")
        .def_property_readonly_static("THE_END", id(Dimension::TheEnd), "The identifier of the End dimension.")
        .def_property_readonly("id", &Dimension::getId, "The identifier of this dimension.")
        .def_property_readonly("translation_key", &Dimension::getTranslationKey,
                               "The translation key, suitable for use in a translation component.")
        .def_property_readonly("level", &Dimension::getLevel, "The level to which this dimension belongs.",
                               py::return_value_policy::reference)
        .def_property_readonly("is_valid", &Dimension::isValid, "Whether this dimension is still valid (loaded).")
        .def("get_block_at", py::overload_cast<Location>(&Dimension::getBlockAt, py::const_),
             py::arg("location").noconvert(), R"doc(
    Gets the `Block` at the given `Location`.

    Args:
        location: `Location` of the block.

    Returns:
        `Block` at the given coordinates.
)doc")
        .def("get_block_at", py::overload_cast<int, int, int>(&Dimension::getBlockAt, py::const_), py::arg("x"),
             py::arg("y"), py::arg("z"), R"doc(
    Gets the `Block` at the given coordinates.

    Args:
        x: X-coordinate of the block.
        y: Y-coordinate of the block.
        z: Z-coordinate of the block.

    Returns:
        `Block` at the given coordinates.
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
    Gets the highest non-empty (impassable) block at the given `Location`.

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
        .def_property_readonly("loaded_chunks", &Dimension::getLoadedChunks, "A list of all loaded `Chunk`s.")
        .def("is_chunk_loaded", &Dimension::isChunkLoaded, py::arg("x"), py::arg("z"), R"doc(
    Checks if the `Chunk` at the given coordinates is loaded.

    Args:
        x: X-coordinate of the chunk.
        z: Z-coordinate of the chunk.

    Returns:
        ``True`` if the chunk is loaded, otherwise ``False``.
)doc")
        .def("is_chunk_generated", &Dimension::isChunkGenerated, py::arg("x"), py::arg("z"), R"doc(
    Checks if the `Chunk` at the given coordinates has been generated.

    A chunk counts as generated once it is loaded or has been written to the level's chunk storage.

    Args:
        x: X-coordinate of the chunk.
        z: Z-coordinate of the chunk.

    Returns:
        ``True`` if the chunk has been generated, otherwise ``False``.
)doc")
        .def("load_chunk", py::overload_cast<int, int, bool>(&Dimension::loadChunk), py::arg("x"), py::arg("z"),
             py::arg("generate") = true, R"doc(
    Requests the `Chunk` at the given coordinates to be loaded, and keeps it resident until it is released again.

    The chunk is held from the moment this returns until ``unload_chunk`` or ``unload_chunk_request`` releases it, or
    the server restarts. Unlike Java Edition, Bedrock has no synchronous chunk load: unless the chunk was already
    resident, the load finishes on a later tick, so ``is_chunk_loaded`` may still report ``False`` right afterwards. A
    chunk held this way stays in memory but is not ticked, and the hold never expires on its own. Intended for keeping
    a handful of chunks resident, not for loading large regions.

    The hold is not attributed to any plugin and survives that plugin being disabled. Use ``add_plugin_chunk_ticket``
    for a hold that is released automatically.

    Args:
        x: X-coordinate of the chunk.
        z: Z-coordinate of the chunk.
        generate: Whether to generate the chunk if it does not exist yet.

    Returns:
        ``False`` if ``generate`` is ``False`` and the chunk has not been generated, or if the coordinates lie outside
        the world limit, otherwise ``True``.

    Raises:
        RuntimeError: If called from a thread other than the server thread.
)doc")
        .def("unload_chunk", &Dimension::unloadChunk, py::arg("x"), py::arg("z"), R"doc(
    Releases the hold that ``load_chunk`` placed on the `Chunk` at the given coordinates, and unloads it if nothing
    else keeps it resident.

    A chunk kept alive by a nearby player, the spawn area, a ``/tickingarea`` or a plugin chunk ticket stays loaded,
    and this reports ``False``. Unloading a chunk saves it and fires a `ChunkUnloadEvent`, which handlers observe
    before this returns. It also completes any chunk unloads the dimension had pending, so calling it once per chunk
    over a large area is expensive; use ``unload_chunk_request`` when releasing many chunks at once.

    Args:
        x: X-coordinate of the chunk.
        z: Z-coordinate of the chunk.

    Returns:
        ``True`` if the chunk is no longer loaded, otherwise ``False``.

    Raises:
        RuntimeError: If called from a thread other than the server thread.
)doc")
        .def("unload_chunk_request", &Dimension::unloadChunkRequest, py::arg("x"), py::arg("z"), R"doc(
    Releases the hold that ``load_chunk`` placed on the `Chunk` at the given coordinates, without unloading it now.

    The chunk is unloaded on a later tick once nothing else keeps it resident.

    Args:
        x: X-coordinate of the chunk.
        z: Z-coordinate of the chunk.

    Returns:
        ``True``.

    Raises:
        RuntimeError: If called from a thread other than the server thread.
)doc")
        .def("add_plugin_chunk_ticket", &Dimension::addPluginChunkTicket, py::arg("x"), py::arg("z"),
             py::arg("plugin"), R"doc(
    Adds a plugin ticket for the `Chunk` at the given coordinates, loading it if it is not already loaded.

    A plugin ticket keeps the chunk resident until it is explicitly removed or the owning plugin is disabled. A plugin
    may only have one ticket per chunk, but each chunk can have multiple plugin tickets. ``unload_chunk`` does not
    remove plugin tickets.

    Args:
        x: X-coordinate of the chunk.
        z: Z-coordinate of the chunk.
        plugin: `Plugin` taking the ticket.

    Returns:
        ``True`` if a plugin ticket was added, ``False`` if the plugin already holds one for this chunk.

    Raises:
        RuntimeError: If the plugin is not enabled, or if called from a thread other than the server thread.
)doc")
        .def("remove_plugin_chunk_ticket", &Dimension::removePluginChunkTicket, py::arg("x"), py::arg("z"),
             py::arg("plugin"), R"doc(
    Removes the given plugin's ticket for the `Chunk` at the given coordinates.

    Args:
        x: X-coordinate of the chunk.
        z: Z-coordinate of the chunk.
        plugin: `Plugin` whose ticket to remove.

    Returns:
        ``True`` if a plugin ticket was removed, ``False`` if the plugin holds none for this chunk.
)doc")
        .def("remove_plugin_chunk_tickets", &Dimension::removePluginChunkTickets, py::arg("plugin"), R"doc(
    Removes every ticket the given plugin holds in this dimension.

    Args:
        plugin: `Plugin` whose tickets to remove.
)doc")
        .def("get_plugin_chunk_tickets",
             py::overload_cast<int, int>(&Dimension::getPluginChunkTickets, py::const_), py::arg("x"), py::arg("z"),
             py::return_value_policy::reference, R"doc(
    Gets which plugins hold a ticket for the `Chunk` at the given coordinates.

    The returned list is a snapshot; it does not track tickets added or removed afterwards.

    Args:
        x: X-coordinate of the chunk.
        z: Z-coordinate of the chunk.

    Returns:
        The `Plugin`s holding a ticket for the chunk.
)doc")
        .def_property_readonly("plugin_chunk_tickets",
                               py::overload_cast<>(&Dimension::getPluginChunkTickets, py::const_),
                               "The `Chunk`s each `Plugin` holds a ticket for, as a snapshot.",
                               py::return_value_policy::reference_internal)
        .def("drop_item", &Dimension::dropItem, py::arg("location"), py::arg("item"), R"doc(
    Drops an item at the specified `Location`.

    Args:
        location: `Location` to drop the item.
        item: `ItemStack` to drop.

    Returns:
        `Item` entity created as a result of this method.
)doc")
        .def("spawn_actor", &Dimension::spawnActor, py::arg("location"), py::arg("type"), R"doc(
    Creates an actor at the given `Location`.

    Args:
        location: The location to spawn the actor.
        type: The actor type to spawn.

    Returns:
        Resulting `Actor` of this method.
)doc")
        .def_property_readonly("actors", &Dimension::getActors,
                               "A list of all actors currently residing in this dimension.")
        .def_property_readonly("mobs", &Dimension::getMobs,
                               "A list of all mobs currently residing in this dimension.")
        .def_property_readonly("players", &Dimension::getPlayers,
                               "A list of all players currently residing in this dimension.");

    py::class_<DimensionCreator>(m, "DimensionCreator",
                                 "Represents the options that may be used to create a custom dimension.")
        .def(py::init<DimensionId>(), py::arg("id"), R"doc(
    Creates an instance of a DimensionCreator for the given dimension id.

    Args:
        id: The identifier of the dimension to create, e.g. `myplugin:void_realm`.
)doc")
        .def_property_readonly("id", &DimensionCreator::getId,
                               "The identifier of the dimension that is being created.");
    py::implicitly_convertible<DimensionId, DimensionCreator>();

    level.def_property_readonly("name", &Level::getName, "The unique name of this level.")
        .def_property_readonly("actors", &Level::getActors, "A list of all actors currently residing in this level.")
        .def_property("time", &Level::getTime, &Level::setTime, "The relative in-game time of this level.")
        .def_property_readonly("dimensions", &Level::getDimensions, "A list of all dimensions within this level.")
        .def_property_readonly("recipes", &Level::getRecipes, "The list of crafting recipes.")
        .def("get_dimension", &Level::getDimension, py::arg("id"), R"doc(
    Gets the dimension with the given id.

    Args:
        id: The id of the dimension to retrieve.

    Returns:
        The `Dimension` with the given id, or `None` if none exists.
)doc")
        .def("create_dimension", &Level::createDimension, py::arg("creator"), R"doc(
    Creates a new custom dimension within this level.

    Custom dimensions are empty (void) dimensions identified by a namespaced id, e.g. `myplugin:void_realm`;
    populate them with blocks, structures or actors afterward.

    The level remembers which id belongs to which name, so a dimension's terrain and actors survive a restart.
    The registration itself does not: call this again on every startup to get the same dimension back.

    If a dimension with the requested name already exists, that existing dimension is returned instead.

    Args:
        creator: The options to use when creating the dimension. An identifier or plain string is also accepted as the name.

    Returns:
        The newly created (or existing) `Dimension`, or `None` if it could not be created.
)doc")
        .def_property_readonly("seed", &Level::getSeed, "The Seed for this level.")
        .def("has_game_rule", &Level::_hasGameRule, py::arg("rule"), R"doc(
    Checks if a game rule exists.

    Args:
        rule: The Minecraft game rule to check.

    Returns:
        `True` if the game rule exists.
)doc")
        .def("get_game_rule", &Level::_getGameRule, py::arg("rule"), R"doc(
    Gets the value of a game rule.

    Args:
        rule: The Minecraft game rule to get.

    Returns:
        The current game rule value.

    Raises:
        IndexError: If the game rule does not exist.
)doc")
        .def(
            "set_game_rule",
            [](Level &self, Identifier<GameRule> rule, GameRuleValue value) {
                if (!self._setGameRule(rule, std::move(value))) {
                    throw py::value_error(std::format("Unable to set game rule {}.", rule));
                }
            },
            py::arg("rule"), py::arg("value"), R"doc(
    Sets the value of a game rule.

    Args:
        rule: The Minecraft game rule to set.
        value: The new value, which must match the type of the game rule.

    Raises:
        ValueError: If the game rule does not exist or the value has the wrong type.
)doc");

    location
        .def(py::init(&create_location), py::arg("dimension"), py::arg("x"), py::arg("y"), py::arg("z"),
             py::arg("pitch") = 0.0, py::arg("yaw") = 0.0)
        .def_property("dimension", &Location::getDimension, &Location::setDimension,
                      "The `Dimension` that contains this position, or `None` if it is not set.")
        .def_property_readonly("is_dimension_loaded", &Location::isDimensionLoaded,
                               "Whether the dimension of this location is present and still loaded.")
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
                      "A unit-vector pointing in the direction that this `Location` is facing.")
        .def_property_readonly("length", &Location::length, R"doc(
    The magnitude of the location, defined as `sqrt(x^2+y^2+z^2)`.

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
    Normalizes the given yaw angle to a value between `+/-180` degrees.

    Args:
        yaw: The yaw in degrees.

    Returns:
        The normalized yaw in degrees.
)doc")
        .def_static("normalize_pitch", &Location::normalizePitch, py::arg("pitch"), R"doc(
    Normalizes the given pitch angle to a value between `+/-90` degrees.

    Args:
        pitch: The pitch in degrees.

    Returns:
        The normalized pitch in degrees.
)doc");
}
}  // namespace endstone::python
