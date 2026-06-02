"""
Classes relating to the blocks in a world, including special states.
"""

import enum
import typing

from endstone import Identifier
from endstone.level import Dimension, Location

__all__ = [
    "Block",
    "BlockData",
    "BlockFace",
    "BlockState",
    "BlockType",
]

class Block:
    """
    Represents a block.
    """
    @property
    def type(self) -> BlockType:
        """
        The type of the block.
        """
        ...
    def set_type(self, type: Identifier[BlockType] | str, apply_physics: bool = True) -> None:
        """
        Sets the type of this block.

        Args:
            type: New type for this block (e.g. `minecraft:stone`).
            apply_physics: `False` to cancel physics on the changed block.
        """
        ...
    @property
    def data(self) -> BlockData:
        """
        The complete block data for this block.
        """
        ...
    def set_data(self, data: BlockData, apply_physics: bool = True) -> None:
        """
        Sets the complete data for this block.

        Args:
            data: New block-specific data.
            apply_physics: `False` to cancel physics on the changed block.
        """
        ...
    @typing.overload
    def get_relative(self, offset_x: int, offset_y: int, offset_z: int) -> Block:
        """
        Gets the block at the given offsets.

        Args:
            offset_x: X-coordinate offset.
            offset_y: Y-coordinate offset.
            offset_z: Z-coordinate offset.

        Returns:
            `Block` at the given offsets.
        """
        ...
    @typing.overload
    def get_relative(self, face: BlockFace, distance: int = 1) -> Block:
        """
        Gets the block at the given distance of the given face.

        Args:
            face: Face of this block to return.
            distance: Distance from this block to get the block at.

        Returns:
            `Block` at the given face.
        """
        ...
    @property
    def dimension(self) -> Dimension:
        """
        The dimension which contains this `Block`.
        """
        ...
    @property
    def x(self) -> int:
        """
        X-coordinate of this block.
        """
        ...
    @property
    def y(self) -> int:
        """
        Y-coordinate of this block.
        """
        ...
    @property
    def z(self) -> int:
        """
        Z-coordinate of this block.
        """
        ...
    @property
    def location(self) -> Location:
        """
        The location of this block.
        """
        ...
    def capture_state(self) -> BlockState:
        """
        Captures the current state of this block.

        The returned object will never be updated, and you are not guaranteed that (for example) a
        sign is still a sign after you capture its state.

        Returns:
            A `BlockState` snapshot of the current state of this block.
        """
        ...
    def __str__(self) -> str: ...

class BlockFace(enum.Enum):
    DOWN = 0
    UP = 1
    NORTH = 2
    SOUTH = 3
    WEST = 4
    EAST = 5

class BlockType:
    """
    Represents a block type.
    """
    @property
    def id(self) -> Identifier[BlockType]:
        """
        The identifier of this block type.
        """
        ...
    @property
    def translation_key(self) -> str:
        """
        The translation key, suitable for use in a translation component.
        """
        ...
    @property
    def has_item_type(self) -> bool:
        """
        `True` if this `BlockType` has a corresponding `ItemType`.
        """
        ...
    def create_block_data(self) -> BlockData:
        """
        Creates a new `BlockData` instance for this block type, with all properties initialized to defaults.

        Returns:
            A new `BlockData` instance.
        """
        ...
    @staticmethod
    def get(name: Identifier[BlockType] | str) -> BlockType:
        """
        Attempts to get the `BlockType` with the given name.

        Args:
            name: The identifier of the block type (e.g. `minecraft:stone`).

        Returns:
            The `BlockType`, or `None` if no block type with that name exists.
        """
        ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def __hash__(self) -> int: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...

class BlockData:
    """
    Represents the data related to a live block.
    """
    @property
    def type(self) -> BlockType:
        """
        The block type represented by this block data.
        """
        ...
    @property
    def translation_key(self) -> str:
        """
        The translation key for this block.
        """
        ...
    @property
    def block_states(self) -> dict[str, bool | str | int]:
        """
        The block states for this block.

        When passed into `Server.create_block_data(type, block_states)` these
        will unambiguously recreate this instance.
        """
        ...
    @property
    def runtime_id(self) -> int:
        """
        The runtime id for this block.
        """
        ...
    def __str__(self) -> str: ...

class BlockState:
    """
    Represents a captured state of a block, which will not update automatically.

    Unlike `Block`, which only one object can exist per coordinate, `BlockState` can
    exist multiple times for any given `Block`. Note that another plugin may change
    the state of the block, and you will not know, or they may change the block to
    another type entirely, causing your `BlockState` to become invalid.
    """
    @property
    def block(self) -> Block:
        """
        The block represented by this block state.
        """
        ...
    @property
    def type(self) -> BlockType:
        """
        The type of this block state.
        """
        ...
    @type.setter
    def type(self, arg1: Identifier[BlockType] | str) -> None: ...
    @property
    def data(self) -> BlockData:
        """
        The data for this block state.
        """
        ...
    @data.setter
    def data(self, arg1: BlockData) -> None: ...
    @property
    def dimension(self) -> Dimension:
        """
        The dimension which contains the block represented by this block state.
        """
        ...
    @property
    def x(self) -> int:
        """
        X-coordinate of this block state.
        """
        ...
    @property
    def y(self) -> int:
        """
        Y-coordinate of this block state.
        """
        ...
    @property
    def z(self) -> int:
        """
        Z-coordinate of this block state.
        """
        ...
    @property
    def location(self) -> Location:
        """
        The location of this block state.
        """
        ...
    def update(self, force: bool = False, apply_physics: bool = True) -> bool:
        """
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
        """
        ...
    def __str__(self) -> str: ...
