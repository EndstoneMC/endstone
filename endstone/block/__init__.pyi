"""
Classes relating to the blocks in a world, including special states.
"""

from __future__ import annotations

import enum
import typing

from endstone.level import Dimension, Location

__all__: list[str] = ["Block", "BlockData", "BlockFace", "BlockState"]

class Block:
    """
    Represents a block.
    """

    def __str__(self) -> str: ...
    def capture_state(self) -> BlockState:
        """
        Captures the current state of this block. The returned object will never be updated, and you are not guaranteed that (for example) a sign is still a sign after you capture its state.
        """

    @typing.overload
    def get_relative(self, offset_x: int, offset_y: int, offset_z: int) -> Block:
        """
        Gets the block at the given offsets
        """

    @typing.overload
    def get_relative(self, face: BlockFace, distance: int = 1) -> Block:
        """
        Gets the block at the given distance of the given face
        """

    def set_data(self, data: BlockData, apply_physics: bool = True) -> None:
        """
        Sets the complete data for this block
        """

    def set_type(self, type: str, apply_physics: bool = True) -> None:
        """
        Sets the type of this block
        """

    @property
    def data(self) -> BlockData:
        """
        Gets the complete data for this block
        """

    @property
    def dimension(self) -> Dimension:
        """
        Gets the dimension which contains this Block
        """

    @property
    def location(self) -> Location:
        """
        Gets the Location of the block
        """

    @property
    def type(self) -> str:
        """
        Gets or sets the type of the block.
        """

    @property
    def x(self) -> int:
        """
        Gets the x-coordinate of this block
        """

    @property
    def y(self) -> int:
        """
        Gets the y-coordinate of this block
        """

    @property
    def z(self) -> int:
        """
        Gets the z-coordinate of this block
        """

class BlockData:
    """
    Represents the data related to a live block
    """

    def __str__(self) -> str: ...
    @property
    def block_states(self) -> dict[str, bool | str | int]:
        """
        Gets the block states for this block.
        """

    @property
    def runtime_id(self) -> int:
        """
        Gets the runtime id for this block.
        """

    @property
    def type(self) -> str:
        """
        Get the block type represented by this block data.
        """

class BlockFace(enum.Enum):
    DOWN: typing.ClassVar[BlockFace]  # value = <BlockFace.DOWN: 0>
    EAST: typing.ClassVar[BlockFace]  # value = <BlockFace.EAST: 5>
    NORTH: typing.ClassVar[BlockFace]  # value = <BlockFace.NORTH: 2>
    SOUTH: typing.ClassVar[BlockFace]  # value = <BlockFace.SOUTH: 3>
    UP: typing.ClassVar[BlockFace]  # value = <BlockFace.UP: 1>
    WEST: typing.ClassVar[BlockFace]  # value = <BlockFace.WEST: 4>

class BlockState:
    """
    Represents a captured state of a block, which will not update automatically.
    """

    def __str__(self) -> str: ...
    def update(self, force: bool = False, apply_physics: bool = True) -> bool:
        """
        Attempts to update the block represented by this state.
        """

    @property
    def block(self) -> Block:
        """
        Gets the block represented by this block state.
        """

    @property
    def data(self) -> BlockData:
        """
        Gets or sets the data for this block state.
        """

    @data.setter
    def data(self, arg1: BlockData) -> None: ...
    @property
    def dimension(self) -> Dimension:
        """
        Gets the dimension which contains the block represented by this block state.
        """

    @property
    def location(self) -> Location:
        """
        Gets the location of this block state.
        """

    @property
    def type(self) -> str:
        """
        Gets or sets the type of this block state.
        """

    @type.setter
    def type(self, arg1: str) -> None: ...
    @property
    def x(self) -> int:
        """
        Gets the x-coordinate of this block state.
        """

    @property
    def y(self) -> int:
        """
        Gets the y-coordinate of this block state.
        """

    @property
    def z(self) -> int:
        """
        Gets the z-coordinate of this block state.
        """
