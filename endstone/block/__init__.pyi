"""
Classes relating to the blocks in a world, including special states.
"""

import enum
import typing

from endstone import Identifier, Rotation
from endstone.actor import ActorType
from endstone.inventory import Inventory, ItemStack
from endstone.level import Dimension, Location

__all__ = [
    "Biome",
    "Block",
    "BlockData",
    "BlockFace",
    "BlockState",
    "BlockType",
    "Campfire",
    "Container",
    "CreatureSpawner",
    "Furnace",
    "ItemFrame",
    "Lectern",
    "Sign",
    "SignSide",
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

    def set_type(self, type: Identifier[BlockType] | str, apply_physics: bool = True) -> None:
        """
        Sets the type of this block.

        Args:
            type: New type for this block (e.g. `minecraft:stone`).
            apply_physics: `False` to cancel physics on the changed block.
        """

    @property
    def data(self) -> BlockData:
        """
        The complete block data for this block.
        """

    def set_data(self, data: BlockData, apply_physics: bool = True) -> None:
        """
        Sets the complete data for this block.

        Args:
            data: New block-specific data.
            apply_physics: `False` to cancel physics on the changed block.
        """

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

    @property
    def dimension(self) -> Dimension:
        """
        The dimension which contains this `Block`.
        """

    @property
    def biome(self) -> Biome:
        """
        The biome that this block resides in.
        """

    @property
    def x(self) -> int:
        """
        X-coordinate of this block.
        """

    @property
    def y(self) -> int:
        """
        Y-coordinate of this block.
        """

    @property
    def z(self) -> int:
        """
        Z-coordinate of this block.
        """

    @property
    def location(self) -> Location:
        """
        The location of this block.
        """

    def capture_state(self) -> BlockState:
        """
        Captures the current state of this block.

        The returned object will never be updated, and you are not guaranteed that (for example) a
        sign is still a sign after you capture its state.

        Returns:
            A `BlockState` snapshot of the current state of this block.
        """

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
    def __hash__(self) -> int: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...

    AIR: Identifier[BlockType] = "minecraft:air"
    @property
    def id(self) -> Identifier[BlockType]:
        """
        The identifier of this block type.
        """

    @property
    def translation_key(self) -> str:
        """
        The translation key, suitable for use in a translation component.
        """

    @property
    def has_item_type(self) -> bool:
        """
        `True` if this `BlockType` has a corresponding `ItemType`.
        """

    def create_block_data(self) -> BlockData:
        """
        Creates a new `BlockData` instance for this block type, with all properties initialized to defaults.

        Returns:
            A new `BlockData` instance.
        """

    @staticmethod
    def get(name: Identifier[BlockType] | str) -> BlockType:
        """
        Attempts to get the `BlockType` with the given name.

        Args:
            name: The identifier of the block type (e.g. `minecraft:stone`).

        Returns:
            The `BlockType`, or `None` if no block type with that name exists.
        """

class BlockData:
    """
    Represents the data related to a live block.
    """
    @property
    def type(self) -> BlockType:
        """
        The block type represented by this block data.
        """

    @property
    def translation_key(self) -> str:
        """
        The translation key for this block.
        """

    @property
    def block_states(self) -> dict[str, bool | str | int]:
        """
        The block states for this block.

        When passed into `Server.create_block_data(type, block_states)` these
        will unambiguously recreate this instance.
        """

    @property
    def runtime_id(self) -> int:
        """
        The runtime id for this block.
        """

class Biome:
    """
    Represents a biome.
    """
    def __hash__(self) -> int: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def id(self) -> Identifier[Biome]:
        """
        The identifier of this biome.
        """

    @property
    def translation_key(self) -> str:
        """
        Raises, as Bedrock does not localize biome names.
        """

    @staticmethod
    def get(name: Identifier[Biome] | str) -> Biome:
        """
        Attempts to get the `Biome` with the given name.

        Args:
            name: The identifier of the biome (e.g. `minecraft:plains`).

        Returns:
            The `Biome`, or `None` if no biome with that name exists.
        """

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

    @property
    def type(self) -> BlockType:
        """
        The type of this block state.
        """

    @type.setter
    def type(self, arg1: Identifier[BlockType] | str) -> None: ...
    @property
    def data(self) -> BlockData:
        """
        The data for this block state.
        """

    @data.setter
    def data(self, arg1: BlockData) -> None: ...
    @property
    def dimension(self) -> Dimension:
        """
        The dimension which contains the block represented by this block state.
        """

    @property
    def x(self) -> int:
        """
        X-coordinate of this block state.
        """

    @property
    def y(self) -> int:
        """
        Y-coordinate of this block state.
        """

    @property
    def z(self) -> int:
        """
        Z-coordinate of this block state.
        """

    @property
    def location(self) -> Location:
        """
        The location of this block state.
        """

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

class Container(BlockState):
    """
    Represents a captured state of a container block, such as a chest.
    """
    @property
    def inventory(self) -> Inventory:
        """
        The inventory of the block represented by this block state.

        If the block was changed to a different type in the meantime, the returned inventory might no
        longer be valid.
        """

class Campfire(BlockState):
    """
    Represents a captured state of a campfire.
    """
    @property
    def size(self) -> int:
        """
        The number of items this campfire can cook at once.
        """

    def get_item(self, index: int) -> ItemStack | None:
        """
        Gets the item currently cooking in the given slot.

        Args:
            index: The slot, between 0 and `size` - 1.

        Returns:
            The item, or `None` if the slot is empty.
        """

    def set_item(self, index: int, item: ItemStack | None) -> None:
        """
        Sets the item currently cooking in the given slot.

        Args:
            index: The slot, between 0 and `size` - 1.
            item: The item, or `None` to empty the slot.
        """

    def get_cook_time(self, index: int) -> int:
        """
        Gets how long the item in the given slot has been cooking for, in ticks.

        Args:
            index: The slot, between 0 and `size` - 1.

        Returns:
            The cook time, in ticks.
        """

    def set_cook_time(self, index: int, cook_time: int) -> None:
        """
        Sets how long the item in the given slot has been cooking for.

        Args:
            index: The slot, between 0 and `size` - 1.
            cook_time: The cook time, in ticks.
        """

class CreatureSpawner(BlockState):
    """
    Represents a captured state of a creature spawner.
    """
    @property
    def spawned_type(self) -> ActorType:
        """
        The type of actor this spawner will spawn.
        """

    @spawned_type.setter
    def spawned_type(self, arg1: Identifier[ActorType] | str) -> None: ...
    @property
    def delay(self) -> int:
        """
        The delay until the spawner spawns the next batch of actors, in ticks.
        """

    @delay.setter
    def delay(self, arg1: int) -> None: ...
    @property
    def min_spawn_delay(self) -> int:
        """
        The minimum delay the spawner will wait between spawns, in ticks.
        """

    @min_spawn_delay.setter
    def min_spawn_delay(self, arg1: int) -> None: ...
    @property
    def max_spawn_delay(self) -> int:
        """
        The maximum delay the spawner will wait between spawns, in ticks.
        """

    @max_spawn_delay.setter
    def max_spawn_delay(self, arg1: int) -> None: ...
    @property
    def spawn_count(self) -> int:
        """
        How many actors the spawner attempts to spawn at a time.
        """

    @spawn_count.setter
    def spawn_count(self, arg1: int) -> None: ...
    @property
    def max_nearby_entities(self) -> int:
        """
        The maximum number of similar actors allowed nearby before the spawner stops spawning.
        """

    @max_nearby_entities.setter
    def max_nearby_entities(self, arg1: int) -> None: ...
    @property
    def required_player_range(self) -> int:
        """
        How far away a player must be for the spawner to be active, in blocks.
        """

    @required_player_range.setter
    def required_player_range(self, arg1: int) -> None: ...
    @property
    def spawn_range(self) -> int:
        """
        The radius around the spawner in which actors are spawned, in blocks.
        """

    @spawn_range.setter
    def spawn_range(self, arg1: int) -> None: ...

class Lectern(Container):
    """
    Represents a captured state of a lectern.
    """
    @property
    def page(self) -> int:
        """
        The page currently displayed on the lectern.

        The page is 0-indexed, and is clamped to the number of pages in the book the lectern holds.
        """

    @page.setter
    def page(self, arg1: int) -> None: ...

class Furnace(Container):
    """
    Represents a captured state of a furnace.
    """
    @property
    def burn_time(self) -> int:
        """
        The burn time.

        This is the number of ticks the current fuel keeps the furnace lit for. A burn time greater
        than 0 will cause this block to be lit, whilst a time less than 0 will extinguish it.
        """

    @burn_time.setter
    def burn_time(self, arg1: int) -> None: ...
    @property
    def cook_time(self) -> int:
        """
        The cook time.

        This is the amount of time the item has been cooking for.
        """

    @cook_time.setter
    def cook_time(self, arg1: int) -> None: ...

class ItemFrame(BlockState):
    """
    Represents a captured state of an item frame.
    """
    @property
    def item(self) -> ItemStack | None:
        """
        The item in this frame.

        Reading this returns a defensive copy of the item, or `None` if the frame is empty. Assign
        `None` to empty the frame.

        Assigning resets the rotation of the frame, as placing an item in a frame does in-game.
        """

    @item.setter
    def item(self, arg1: ItemStack | None) -> None: ...
    @property
    def item_drop_chance(self) -> float:
        """
        The chance of the item being dropped upon this frame's destruction.

        A drop chance of 0.0 will never drop, a drop chance of 1.0 will always drop.
        """

    @item_drop_chance.setter
    def item_drop_chance(self, arg1: float) -> None: ...
    @property
    def rotation(self) -> Rotation:
        """
        The rotation of the frame's item.
        """

    @rotation.setter
    def rotation(self, arg1: Rotation) -> None: ...

class SignSide:
    """
    Represents a side of a sign.
    """
    @property
    def lines(self) -> list[str]:
        """
        All the lines of text currently on this side of the sign.
        """

    def get_line(self, index: int) -> str:
        """
        Gets the line of text at the specified index on this side of the sign.

        For example, `get_line(0)` will return the first line of text.

        Args:
            index: The index of the line to get, between 0 and 3.

        Returns:
            The text of the line, empty if the line is blank.
        """

    def set_line(self, index: int, line: str) -> None:
        """
        Sets the line of text at the specified index on this side of the sign.

        For example, `set_line(0, "Line One")` will set the first line to "Line One".

        Args:
            index: The index of the line to set, between 0 and 3.
            line: The new text to set.
        """

    @property
    def glowing_text(self) -> bool:
        """
        Whether this side of the sign has glowing text.
        """

    @glowing_text.setter
    def glowing_text(self, arg1: bool) -> None: ...
    @property
    def color(self) -> tuple[int, ...]:
        """
        The color of this side of the sign.

        Bedrock stores the text color of a sign as a color rather than as one of the dyes, so this is a
        `Color` where Bukkit has a `DyeColor`.
        """

    @color.setter
    def color(self, arg1: tuple[int, ...]) -> None: ...

class Sign(BlockState):
    """
    Represents a captured state of a sign.
    """
    class Side(enum.Enum):
        """
        Represents a side of a sign.
        """

        FRONT = 0
        BACK = 1

    def get_side(self, side: Side) -> SignSide:
        """
        Gets the side of this sign.

        Args:
            side: The side of the sign.

        Returns:
            The given side of the sign.
        """

    @property
    def waxed(self) -> bool:
        """
        Whether this sign is waxed.

        A waxed sign cannot be edited by players.
        """

    @waxed.setter
    def waxed(self, arg1: bool) -> None: ...
