import typing

from endstone import GameRule, Identifier, Player
from endstone.actor import Actor, ActorType, Item, Mob
from endstone.block import Block
from endstone.inventory import ItemStack, Recipe
from endstone.plugin import Plugin
from endstone.util import Vector

__all__ = [
    "Chunk",
    "Dimension",
    "DimensionCreator",
    "Level",
    "Location",
]

_T = typing.TypeVar("_T")

class Level:
    """
    Represents a level, which may contain actors, chunks and blocks.
    """
    @property
    def name(self) -> str:
        """
        The unique name of this level.
        """

    @property
    def actors(self) -> list[Actor]:
        """
        A list of all actors currently residing in this level.
        """

    @property
    def time(self) -> int:
        """
        The relative in-game time of this level.
        """

    @time.setter
    def time(self, arg1: int) -> None: ...
    @property
    def dimensions(self) -> list[Dimension]:
        """
        A list of all dimensions within this level.
        """

    @property
    def recipes(self) -> list[Recipe]:
        """
        The list of crafting recipes.
        """

    def get_dimension(self, id: Identifier[Dimension] | str) -> Dimension | None:
        """
        Gets the dimension with the given id.

        Args:
            id: The id of the dimension to retrieve.

        Returns:
            The `Dimension` with the given id, or `None` if none exists.
        """

    def create_dimension(self, creator: DimensionCreator) -> Dimension | None:
        """
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
        """

    @property
    def seed(self) -> int:
        """
        The Seed for this level.
        """

    def has_game_rule(self, rule: Identifier[GameRule] | str) -> bool:
        """
        Checks if a game rule exists.

        Args:
            rule: The Minecraft game rule to check.

        Returns:
            `True` if the game rule exists.
        """

    @typing.overload
    def get_game_rule(self, rule: Identifier[GameRule[_T]]) -> _T: ...
    @typing.overload
    def get_game_rule(self, rule: str) -> bool | int | float:
        """
        Gets the value of a game rule.

        Args:
            rule: The Minecraft game rule to get.

        Returns:
            The current game rule value.

        Raises:
            IndexError: If the game rule does not exist.
        """

    @typing.overload
    def set_game_rule(self, rule: Identifier[GameRule[_T]], value: _T) -> None: ...
    @typing.overload
    def set_game_rule(self, rule: str, value: bool | float) -> None:
        """
        Sets the value of a game rule.

        Args:
            rule: The Minecraft game rule to set.
            value: The new value, which must match the type of the game rule.

        Raises:
            ValueError: If the game rule does not exist or the value has the wrong type.
        """

class Dimension:
    """
    Represents a dimension within a `Level`.
    """

    OVERWORLD: Identifier[Dimension] = "minecraft:overworld"
    NETHER: Identifier[Dimension] = "minecraft:nether"
    THE_END: Identifier[Dimension] = "minecraft:the_end"
    @property
    def id(self) -> Identifier[Dimension]:
        """
        The identifier of this dimension.
        """

    @property
    def translation_key(self) -> str:
        """
        The translation key, suitable for use in a translation component.
        """

    @property
    def level(self) -> Level:
        """
        The level to which this dimension belongs.
        """

    @property
    def is_valid(self) -> bool:
        """
        Whether this dimension is still valid (loaded).
        """

    @typing.overload
    def get_block_at(self, location: Location) -> Block:
        """
        Gets the `Block` at the given `Location`.

        Args:
            location: `Location` of the block.

        Returns:
            `Block` at the given coordinates.
        """

    @typing.overload
    def get_block_at(self, x: int, y: int, z: int) -> Block:
        """
        Gets the `Block` at the given coordinates.

        Args:
            x: X-coordinate of the block.
            y: Y-coordinate of the block.
            z: Z-coordinate of the block.

        Returns:
            `Block` at the given coordinates.
        """

    def get_highest_block_y_at(self, x: int, z: int) -> int:
        """
        Gets the highest non-empty (impassable) coordinate at the given coordinates.

        Args:
            x: X-coordinate of the blocks.
            z: Z-coordinate of the blocks.

        Returns:
            Y-coordinate of the highest non-empty block.
        """

    @typing.overload
    def get_highest_block_at(self, location: Location) -> Block:
        """
        Gets the highest non-empty (impassable) block at the given `Location`.

        Args:
            location: Coordinates to get the highest block.

        Returns:
            Highest non-empty block.
        """

    @typing.overload
    def get_highest_block_at(self, x: int, z: int) -> Block:
        """
        Gets the highest non-empty (impassable) block at the given coordinates.

        Args:
            x: X-coordinate of the block.
            z: Z-coordinate of the block.

        Returns:
            Highest non-empty block.
        """

    @property
    def loaded_chunks(self) -> list[Chunk]:
        """
        A list of all loaded `Chunk`s.
        """

    def is_chunk_loaded(self, x: int, z: int) -> bool:
        """
        Checks if the `Chunk` at the given coordinates is loaded.

        Args:
            x: X-coordinate of the chunk.
            z: Z-coordinate of the chunk.

        Returns:
            ``True`` if the chunk is loaded, otherwise ``False``.
        """

    def is_chunk_generated(self, x: int, z: int) -> bool:
        """
        Checks if the `Chunk` at the given coordinates has been generated.

        A chunk counts as generated once it is loaded or has been written to the level's chunk storage.

        Args:
            x: X-coordinate of the chunk.
            z: Z-coordinate of the chunk.

        Returns:
            ``True`` if the chunk has been generated, otherwise ``False``.
        """

    def load_chunk(self, x: int, z: int, generate: bool = True) -> bool:
        """
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
        """

    def unload_chunk(self, x: int, z: int) -> bool:
        """
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
        """

    def unload_chunk_request(self, x: int, z: int) -> bool:
        """
        Releases the hold that ``load_chunk`` placed on the `Chunk` at the given coordinates, without unloading it now.

        The chunk is unloaded on a later tick once nothing else keeps it resident.

        Args:
            x: X-coordinate of the chunk.
            z: Z-coordinate of the chunk.

        Returns:
            ``True``.

        Raises:
            RuntimeError: If called from a thread other than the server thread.
        """

    def add_plugin_chunk_ticket(self, x: int, z: int, plugin: Plugin) -> bool:
        """
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
        """

    def remove_plugin_chunk_ticket(self, x: int, z: int, plugin: Plugin) -> bool:
        """
        Removes the given plugin's ticket for the `Chunk` at the given coordinates.

        Args:
            x: X-coordinate of the chunk.
            z: Z-coordinate of the chunk.
            plugin: `Plugin` whose ticket to remove.

        Returns:
            ``True`` if a plugin ticket was removed, ``False`` if the plugin holds none for this chunk.
        """

    def remove_plugin_chunk_tickets(self, plugin: Plugin) -> None:
        """
        Removes every ticket the given plugin holds in this dimension.

        Args:
            plugin: `Plugin` whose tickets to remove.
        """

    def get_plugin_chunk_tickets(self, x: int, z: int) -> list[Plugin]:
        """
        Gets which plugins hold a ticket for the `Chunk` at the given coordinates.

        The returned list is a snapshot; it does not track tickets added or removed afterwards.

        Args:
            x: X-coordinate of the chunk.
            z: Z-coordinate of the chunk.

        Returns:
            The `Plugin`s holding a ticket for the chunk.
        """

    @property
    def plugin_chunk_tickets(self) -> dict[Plugin, list[Chunk]]:
        """
        The `Chunk`s each `Plugin` holds a ticket for, as a snapshot.
        """

    def drop_item(self, location: Location, item: ItemStack) -> Item:
        """
        Drops an item at the specified `Location`.

        Args:
            location: `Location` to drop the item.
            item: `ItemStack` to drop.

        Returns:
            `Item` entity created as a result of this method.
        """

    def spawn_actor(self, location: Location, type: Identifier[ActorType] | str) -> Actor | None:
        """
        Creates an actor at the given `Location`.

        Args:
            location: The location to spawn the actor.
            type: The actor type to spawn.

        Returns:
            Resulting `Actor` of this method.
        """

    @property
    def actors(self) -> list[Actor]:
        """
        A list of all actors currently residing in this dimension.
        """

    @property
    def mobs(self) -> list[Mob]:
        """
        A list of all mobs currently residing in this dimension.
        """

    @property
    def players(self) -> list[Player]:
        """
        A list of all players currently residing in this dimension.
        """

class Location:
    """
    Represents a 3-dimensional location in a dimension within a level.
    """
    def __init__(
        self, dimension: Dimension | None, x: float, y: float, z: float, pitch: float = 0.0, yaw: float = 0.0
    ) -> None: ...
    @property
    def dimension(self) -> Dimension | None:
        """
        The `Dimension` that contains this position, or `None` if it is not set.
        """

    @dimension.setter
    def dimension(self, arg1: Dimension | None) -> None: ...
    @property
    def is_dimension_loaded(self) -> bool:
        """
        Whether the dimension of this location is present and still loaded.
        """

    @property
    def block(self) -> Block:
        """
        The block at the represented location.
        """

    @property
    def pitch(self) -> float:
        """
        The pitch of this location, measured in degrees.
        """

    @pitch.setter
    def pitch(self, arg1: float) -> None: ...
    @property
    def yaw(self) -> float:
        """
        The yaw of this location, measured in degrees.
        """

    @yaw.setter
    def yaw(self, arg1: float) -> None: ...
    @property
    def x(self) -> float:
        """
        The x-coordinate of this location.
        """

    @x.setter
    def x(self, arg1: float) -> None: ...
    @property
    def y(self) -> float:
        """
        The y-coordinate of this location.
        """

    @y.setter
    def y(self, arg1: float) -> None: ...
    @property
    def z(self) -> float:
        """
        The z-coordinate of this location.
        """

    @z.setter
    def z(self, arg1: float) -> None: ...
    @property
    def block_x(self) -> int:
        """
        The floored value of the X component, indicating the block that this location is contained with.
        """

    @property
    def block_y(self) -> int:
        """
        The floored value of the Y component, indicating the block that this location is contained with.
        """

    @property
    def block_z(self) -> int:
        """
        The floored value of the Z component, indicating the block that this location is contained with.
        """

    @property
    def direction(self) -> Vector:
        """
        A unit-vector pointing in the direction that this `Location` is facing.
        """

    @direction.setter
    def direction(self, arg1: Vector) -> Location: ...
    @property
    def length(self) -> float:
        """
        The magnitude of the location, defined as `sqrt(x^2+y^2+z^2)`.

        Not world-aware and orientation independent.
        """

    @property
    def length_squared(self) -> float:
        """
        The magnitude of the location squared.

        Not world-aware and orientation independent.
        """

    def distance(self, other: Location) -> float:
        """
        Get the distance between this location and another.

        Args:
            other: The other location.

        Returns:
            The distance.
        """

    def distance_squared(self, other: Location) -> float:
        """
        Get the squared distance between this location and another.

        Args:
            other: The other location.

        Returns:
            The distance.
        """

    @typing.overload
    def __iadd__(self, arg0: Location) -> typing.Self: ...
    @typing.overload
    def __iadd__(self, arg0: Vector) -> typing.Self: ...
    @typing.overload
    def __isub__(self, arg0: Location) -> typing.Self: ...
    @typing.overload
    def __isub__(self, arg0: Vector) -> typing.Self: ...
    def __imul__(self, arg0: float) -> typing.Self: ...
    def zero(self) -> Location:
        """
        Zero this location's components. Not world-aware.
        """

    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    @staticmethod
    def normalize_yaw(yaw: float) -> float:
        """
        Normalizes the given yaw angle to a value between `+/-180` degrees.

        Args:
            yaw: The yaw in degrees.

        Returns:
            The normalized yaw in degrees.
        """

    @staticmethod
    def normalize_pitch(pitch: float) -> float:
        """
        Normalizes the given pitch angle to a value between `+/-90` degrees.

        Args:
            pitch: The pitch in degrees.

        Returns:
            The normalized pitch in degrees.
        """

class Chunk:
    """
    Represents a chunk of blocks.
    """
    @property
    def x(self) -> int:
        """
        X-coordinate of this chunk.
        """

    @property
    def z(self) -> int:
        """
        Z-coordinate of this chunk.
        """

    @property
    def level(self) -> Level:
        """
        The level containing this chunk.
        """

    @property
    def dimension(self) -> Dimension:
        """
        The dimension containing this chunk.
        """

    @property
    def is_loaded(self) -> bool:
        """
        Whether this chunk is loaded.
        """

    def load(self, generate: bool = True) -> bool:
        """
        Requests this chunk to be loaded, and keeps it resident until it is released again.

        See `Dimension.load_chunk` for how the hold behaves.

        Args:
            generate: Whether to generate the chunk if it does not exist yet.

        Returns:
            ``False`` if ``generate`` is ``False`` and the chunk has not been generated, otherwise ``True``.
        """

    def unload(self) -> bool:
        """
        Releases the hold that ``load`` placed on this chunk, and unloads it if nothing else keeps it resident.

        Returns:
            ``True`` if the chunk is no longer loaded, otherwise ``False``.
        """

    def add_plugin_chunk_ticket(self, plugin: Plugin) -> bool:
        """
        Adds a plugin ticket for this chunk, loading it if it is not already loaded.

        Args:
            plugin: `Plugin` taking the ticket.

        Returns:
            ``True`` if a plugin ticket was added, ``False`` if the plugin already holds one for this chunk.
        """

    def remove_plugin_chunk_ticket(self, plugin: Plugin) -> bool:
        """
        Removes the given plugin's ticket for this chunk.

        Args:
            plugin: `Plugin` whose ticket to remove.

        Returns:
            ``True`` if a plugin ticket was removed, ``False`` if the plugin holds none for this chunk.
        """

    @property
    def plugin_chunk_tickets(self) -> list[Plugin]:
        """
        The `Plugin`s holding a ticket for this chunk.
        """

class DimensionCreator:
    """
    Represents the options that may be used to create a custom dimension.
    """
    def __init__(self, id: Identifier[Dimension] | str) -> None:
        """
        Creates an instance of a DimensionCreator for the given dimension id.

        Args:
            id: The identifier of the dimension to create, e.g. `myplugin:void_realm`.
        """

    @property
    def id(self) -> Identifier[Dimension]:
        """
        The identifier of the dimension that is being created.
        """
