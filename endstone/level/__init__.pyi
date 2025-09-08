from __future__ import annotations

import enum
import typing

import endstone._python.actor
import endstone._python.block
import endstone._python.inventory
import endstone._python.util

__all__: list[str] = ["Chunk", "Dimension", "Level"]

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
    def drop_item(
        self, location: endstone._python.util.Location, item: endstone._python.inventory.ItemStack
    ) -> endstone._python.actor.Item:
        """
        Drops an item at the specified Location
        """
    @typing.overload
    def get_block_at(self, location: endstone._python.util.Location) -> endstone._python.block.Block:
        """
        Gets the Block at the given Location
        """
    @typing.overload
    def get_block_at(
        self, x: typing.SupportsInt, y: typing.SupportsInt, z: typing.SupportsInt
    ) -> endstone._python.block.Block:
        """
        Gets the Block at the given coordinates
        """
    @typing.overload
    def get_highest_block_at(self, location: endstone._python.util.Location) -> endstone._python.block.Block:
        """
        Gets the highest non-empty (impassable) block at the given Location.
        """
    @typing.overload
    def get_highest_block_at(self, x: typing.SupportsInt, z: typing.SupportsInt) -> endstone._python.block.Block:
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
    def actors(self) -> list[endstone._python.actor.Actor]:
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
