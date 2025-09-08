from __future__ import annotations

import enum
import typing

from endstone.actor import Actor, Item
from endstone.block import Block
from endstone.inventory import ItemStack
from endstone.util import Vector

__all__: list[str] = ["Chunk", "Dimension", "Level", "Location"]

class Chunk:
    """
    Represents a chunk of blocks.
    """

    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @property
    def dimension(self) -> Dimension:
        """
        Gets the dimension containing this chunk
        """

    @property
    def level(self) -> Level:
        """
        Gets the level containing this chunk
        """

    @property
    def x(self) -> int:
        """
        Gets the X-coordinate of this chunk
        """

    @property
    def z(self) -> int:
        """
        Gets the Z-coordinate of this chunk
        """

class Dimension:
    """
    Represents a dimension within a Level.
    """

    class Type(enum.Enum):
        """
        Represents various dimension types.
        """

        CUSTOM: typing.ClassVar[Dimension.Type]  # value = <Type.CUSTOM: 999>
        NETHER: typing.ClassVar[Dimension.Type]  # value = <Type.NETHER: 1>
        OVERWORLD: typing.ClassVar[Dimension.Type]  # value = <Type.OVERWORLD: 0>
        THE_END: typing.ClassVar[Dimension.Type]  # value = <Type.THE_END: 2>

    CUSTOM: typing.ClassVar[Dimension.Type]  # value = <Type.CUSTOM: 999>
    NETHER: typing.ClassVar[Dimension.Type]  # value = <Type.NETHER: 1>
    OVERWORLD: typing.ClassVar[Dimension.Type]  # value = <Type.OVERWORLD: 0>
    THE_END: typing.ClassVar[Dimension.Type]  # value = <Type.THE_END: 2>

    def drop_item(self, location: Location, item: ItemStack) -> Item:
        """
        Drops an item at the specified Location
        """

    @typing.overload
    def get_block_at(self, location: Location) -> Block:
        """
        Gets the Block at the given Location
        """

    @typing.overload
    def get_block_at(self, x: typing.SupportsInt, y: typing.SupportsInt, z: typing.SupportsInt) -> Block:
        """
        Gets the Block at the given coordinates
        """

    @typing.overload
    def get_highest_block_at(self, location: Location) -> Block:
        """
        Gets the highest non-empty (impassable) block at the given Location.
        """

    @typing.overload
    def get_highest_block_at(self, x: typing.SupportsInt, z: typing.SupportsInt) -> Block:
        """
        Gets the highest non-empty (impassable) block at the given coordinates.
        """

    def get_highest_block_y_at(self, x: typing.SupportsInt, z: typing.SupportsInt) -> int:
        """
        Gets the highest non-empty (impassable) coordinate at the given coordinates.
        """

    @property
    def level(self) -> Level:
        """
        Gets the level to which this dimension belongs
        """

    @property
    def loaded_chunks(self) -> list[Chunk]:
        """
        Gets a list of all loaded Chunks
        """

    @property
    def name(self) -> str:
        """
        Gets the name of this dimension
        """

    @property
    def type(self) -> Dimension.Type:
        """
        Gets the type of this dimension
        """

class Level:
    def get_dimension(self, name: str) -> Dimension:
        """
        Gets the dimension with the given name.
        """

    @property
    def actors(self) -> list[Actor]:
        """
        Get a list of all actors in this level
        """

    @property
    def dimensions(self) -> list[Dimension]:
        """
        Gets a list of all dimensions within this level.
        """

    @property
    def name(self) -> str:
        """
        Gets the unique name of this level
        """

    @property
    def seed(self) -> int:
        """
        Gets the Seed for this level.
        """

    @property
    def time(self) -> int:
        """
        Gets and sets the relative in-game time on the server
        """

    @time.setter
    def time(self, arg1: typing.SupportsInt) -> None: ...

class Location(Vector):
    """
    Represents a 3-dimensional location in a dimension within a level.
    """

    def __init__(
        self,
        x: typing.SupportsFloat,
        y: typing.SupportsFloat,
        z: typing.SupportsFloat,
        pitch: typing.SupportsFloat = 0.0,
        yaw: typing.SupportsFloat = 0.0,
        dimension: Dimension = None,
    ) -> None: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @property
    def block_x(self) -> int:
        """
        Gets the floored value of the X component, indicating the block that this location is contained with.
        """

    @property
    def block_y(self) -> int:
        """
        Gets the floored value of the Y component, indicating the block that this location is contained with.
        """

    @property
    def block_z(self) -> int:
        """
        Gets the floored value of the Z component, indicating the block that this location is contained with.
        """

    @property
    def dimension(self) -> Dimension:
        """
        The Dimension that contains this position
        """

    @dimension.setter
    def dimension(self, arg1: Dimension) -> None: ...
    @property
    def pitch(self) -> float:
        """
        The pitch of this location, measured in degrees.
        """

    @pitch.setter
    def pitch(self, arg1: typing.SupportsFloat) -> None: ...
    @property
    def yaw(self) -> float:
        """
        The yaw of this location, measured in degrees.
        """

    @yaw.setter
    def yaw(self, arg1: typing.SupportsFloat) -> None: ...
