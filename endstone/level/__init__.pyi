import enum
import typing

from endstone.actor import Actor, Item
from endstone.block import Block
from endstone.inventory import ItemStack

__all__ = ["Chunk", "Dimension", "Level", "Location"]

class Level:
    @property
    def name(self) -> str:
        """
        Gets the unique name of this level
        """
        ...
    @property
    def actors(self) -> list[Actor]:
        """
        Get a list of all actors in this level
        """
        ...
    @property
    def time(self) -> int:
        """
        Gets and sets the relative in-game time on the server
        """
        ...
    @time.setter
    def time(self, arg1: int) -> None: ...
    @property
    def dimensions(self) -> list[Dimension]:
        """
        Gets a list of all dimensions within this level.
        """
        ...
    def get_dimension(self, name: str) -> Dimension:
        """
        Gets the dimension with the given name.
        """
        ...
    @property
    def seed(self) -> int:
        """
        Gets the Seed for this level.
        """
        ...

class Dimension:
    """
    Represents a dimension within a Level.
    """
    class Type(enum.Enum):
        """
        Represents various dimension types.
        """

        OVERWORLD = 0
        NETHER = 1
        THE_END = 2
        CUSTOM = 999

    OVERWORLD = Type.OVERWORLD
    NETHER = Type.NETHER
    THE_END = Type.THE_END
    CUSTOM = Type.CUSTOM
    @property
    def name(self) -> str:
        """
        Gets the name of this dimension
        """
        ...
    @property
    def type(self) -> Type:
        """
        Gets the type of this dimension
        """
        ...
    @property
    def level(self) -> Level:
        """
        Gets the level to which this dimension belongs
        """
        ...
    @typing.overload
    def get_block_at(self, location: Location) -> Block:
        """
        Gets the Block at the given Location
        """
        ...
    @typing.overload
    def get_block_at(self, x: int, y: int, z: int) -> Block:
        """
        Gets the Block at the given coordinates
        """
        ...
    def get_highest_block_y_at(self, x: int, z: int) -> int:
        """
        Gets the highest non-empty (impassable) coordinate at the given coordinates.
        """
        ...
    @typing.overload
    def get_highest_block_at(self, location: Location) -> Block:
        """
        Gets the highest non-empty (impassable) block at the given Location.
        """
        ...
    @typing.overload
    def get_highest_block_at(self, x: int, z: int) -> Block:
        """
        Gets the highest non-empty (impassable) block at the given coordinates.
        """
        ...
    @property
    def loaded_chunks(self) -> list[Chunk]:
        """
        Gets a list of all loaded Chunks
        """
        ...
    def drop_item(self, location: Location, item: ItemStack) -> Item:
        """
        Drops an item at the specified Location
        """
        ...
    def spawn_actor(self, location: Location, type: str) -> Actor:
        """
        Creates an actor at the given Location
        """
        ...
    @property
    def actors(self) -> list[Actor]:
        """
        Get a list of all actors in this dimension
        """
        ...

class Location:
    """
    Represents a 3-dimensional location in a dimension within a level.
    """
    def __init__(
        self, dimension: Dimension, x: float, y: float, z: float, pitch: float = 0.0, yaw: float = 0.0
    ) -> None: ...
    @property
    def pitch(self) -> float:
        """
        The pitch of this location, measured in degrees.
        """
        ...
    @pitch.setter
    def pitch(self, arg1: float) -> None: ...
    @property
    def yaw(self) -> float:
        """
        The yaw of this location, measured in degrees.
        """
        ...
    @yaw.setter
    def yaw(self, arg1: float) -> None: ...
    @property
    def dimension(self) -> Dimension:
        """
        The Dimension that contains this position
        """
        ...
    @dimension.setter
    def dimension(self, arg1: Dimension) -> None: ...
    @property
    def block_x(self) -> int:
        """
        Gets the floored value of the X component, indicating the block that this location is contained with.
        """
        ...
    @property
    def block_y(self) -> int:
        """
        Gets the floored value of the Y component, indicating the block that this location is contained with.
        """
        ...
    @property
    def block_z(self) -> int:
        """
        Gets the floored value of the Z component, indicating the block that this location is contained with.
        """
        ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...

class Chunk:
    """
    Represents a chunk of blocks.
    """
    @property
    def x(self) -> int:
        """
        Gets the X-coordinate of this chunk
        """
        ...
    @property
    def z(self) -> int:
        """
        Gets the Z-coordinate of this chunk
        """
        ...
    @property
    def level(self) -> Level:
        """
        Gets the level containing this chunk
        """
        ...
    @property
    def dimension(self) -> Dimension:
        """
        Gets the dimension containing this chunk
        """
        ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
