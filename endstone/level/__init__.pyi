import typing

from endstone import GameRule, Identifier, Player
from endstone.actor import Actor, ActorType, Item, Mob
from endstone.block import Block
from endstone.inventory import ItemStack
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

    def load_chunk(self, x: int, z: int) -> bool:
        """
        Requests the `Chunk` at the given coordinates to be loaded, and keeps it loaded until unloaded again.

        Unlike Java Edition, Bedrock has no synchronous chunk load: this registers a plugin-owned ticket for the chunk,
        honoured on the next server tick (so the chunk may not be available right away). The chunk stays loaded until
        ``unload_chunk`` is called or the server restarts. Intended for keeping a handful of chunks resident, not for
        loading large regions.

        Args:
            x: X-coordinate of the chunk.
            z: Z-coordinate of the chunk.

        Returns:
            ``True`` if the ticket was registered (or already present), otherwise ``False``.
        """

    def unload_chunk(self, x: int, z: int) -> bool:
        """
        Releases the plugin-owned ticket that ``load_chunk`` placed on the `Chunk` at the given coordinates.

        This only removes Endstone's own ticket; the chunk is unloaded once nothing else keeps it loaded (a nearby player,
        the spawn area, etc.), so it is a no-op in effect while the chunk is still in use.

        Args:
            x: X-coordinate of the chunk.
            z: Z-coordinate of the chunk.

        Returns:
            ``True`` once the ticket has been released.
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
