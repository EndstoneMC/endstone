"""
Classes relating to handling triggered code executions.
"""

import enum
import typing

from endstone import GameMode, Input, Player, Skin
from endstone.actor import Actor, Item, Mob
from endstone.block import Block, BlockFace, BlockState
from endstone.command import CommandSender
from endstone.damage import DamageSource
from endstone.inventory import BookMeta, EquipmentSlot, ItemStack
from endstone.lang import Translatable
from endstone.level import Chunk, Dimension, Level, Location
from endstone.map import MapView
from endstone.plugin import Plugin
from endstone.potion import Effect
from endstone.util import SocketAddress, Vector

__all__ = [
    "ActorChangeBlockEvent",
    "ActorCollideWithActorEvent",
    "ActorDamageEvent",
    "ActorDeathEvent",
    "ActorDismountEvent",
    "ActorEffectEvent",
    "ActorEvent",
    "ActorExplodeEvent",
    "ActorKnockbackEvent",
    "ActorPickupItemEvent",
    "ActorRemoveEvent",
    "ActorSpawnEvent",
    "ActorTeleportEvent",
    "ActorToggleGlideEvent",
    "ActorToggleSwimEvent",
    "BlockBreakEvent",
    "BlockCookEvent",
    "BlockEvent",
    "BlockExplodeEvent",
    "BlockFormEvent",
    "BlockFromToEvent",
    "BlockGrowEvent",
    "BlockPistonEvent",
    "BlockPistonExtendEvent",
    "BlockPistonRetractEvent",
    "BlockPlaceEvent",
    "BroadcastMessageEvent",
    "Cancellable",
    "ChunkEvent",
    "ChunkLoadEvent",
    "ChunkUnloadEvent",
    "DimensionEvent",
    "Event",
    "EventPriority",
    "EventResult",
    "LeavesDecayEvent",
    "LevelEvent",
    "MapInitializeEvent",
    "MobEvent",
    "PacketReceiveEvent",
    "PacketSendEvent",
    "PlayerArmSwingEvent",
    "PlayerArmorStandManipulateEvent",
    "PlayerBedEnterEvent",
    "PlayerBedLeaveEvent",
    "PlayerBucketActorEvent",
    "PlayerChatEvent",
    "PlayerCommandEvent",
    "PlayerCraftItemEvent",
    "PlayerDeathEvent",
    "PlayerDimensionChangeEvent",
    "PlayerDropItemEvent",
    "PlayerEditBookEvent",
    "PlayerEmoteEvent",
    "PlayerEvent",
    "PlayerExpChangeEvent",
    "PlayerGameModeChangeEvent",
    "PlayerInputEvent",
    "PlayerInteractActorEvent",
    "PlayerInteractEvent",
    "PlayerItemConsumeEvent",
    "PlayerItemHeldEvent",
    "PlayerJoinEvent",
    "PlayerJumpEvent",
    "PlayerKickEvent",
    "PlayerLevelChangeEvent",
    "PlayerLoginEvent",
    "PlayerMoveEvent",
    "PlayerPickupArrowEvent",
    "PlayerPickupExperienceEvent",
    "PlayerPickupItemEvent",
    "PlayerPortalEvent",
    "PlayerQuitEvent",
    "PlayerRecipeBookClickEvent",
    "PlayerRecipeBookSettingsChangeEvent",
    "PlayerRespawnEvent",
    "PlayerRiptideEvent",
    "PlayerSetSpawnEvent",
    "PlayerShearActorEvent",
    "PlayerSkinChangeEvent",
    "PlayerTeleportEvent",
    "PlayerToggleCrawlEvent",
    "PlayerToggleFlightEvent",
    "PlayerToggleSneakEvent",
    "PlayerToggleSprintEvent",
    "PluginDisableEvent",
    "PluginEnableEvent",
    "ScriptMessageEvent",
    "ServerCommandEvent",
    "ServerEvent",
    "ServerListPingEvent",
    "ServerLoadEvent",
    "ThunderChangeEvent",
    "WeatherChangeEvent",
    "WeatherEvent",
    "event_handler",
]

_F = typing.TypeVar("_F", bound=(typing.Callable[..., None]))

class EventPriority(enum.IntEnum):
    """
    Listeners are called in following order: `LOWEST` -> `LOW` -> `NORMAL` -> `HIGH` -> `HIGHEST` -> `MONITOR`
    """

    LOWEST = 0
    """
    Event call is of very low importance and should be run first, to allow other plugins to further customise the outcome
    """

    LOW = 1
    """
    Event call is of low importance
    """

    NORMAL = 2
    """
    Event call is neither important nor unimportant, and may be run normally
    """

    HIGH = 3
    """
    Event call is of high importance
    """

    HIGHEST = 4
    """
    Event call is critical and must have the final say in what happens to the event
    """

    MONITOR = 5
    """
    Event is listened to purely for monitoring the outcome of an event. No modifications to the event should be made under this priority.
    """

class Event:
    """
    Represents an event.
    """
    def __init__(self, is_async: bool = False) -> None: ...
    @property
    def event_name(self) -> str:
        """
        A user-friendly identifier for this event.
        """

    @property
    def is_asynchronous(self) -> bool:
        """
        `False` by default, `True` if the event fires asynchronously.
        """

class EventResult(enum.Enum):
    """
    Represents the result a plugin can apply to an event whose default behaviour can be allowed, denied, or left to the server.
    """

    DENY = 0
    DEFAULT = 1
    ALLOW = 2

class Cancellable:
    """
    A type characterizing events that may be cancelled by a plugin or the server.
    """
    @property
    def is_cancelled(self) -> bool:
        """
        The cancellation state of this event.

        A cancelled event will not be executed in the server, but will still pass to other plugins.
        """

    @is_cancelled.setter
    def is_cancelled(self, arg1: bool) -> None: ...
    def cancel(self) -> None:
        """
        Cancel this event.

        A cancelled event will not be executed in the server, but will still pass to other plugins.
        """

class ActorEvent(Event):
    """
    Represents an `Actor`-related event.
    """
    @property
    def actor(self) -> Actor:
        """
        The `Actor` which is involved in this event.
        """

class MobEvent(Event):
    """
    Represents an `Mob`-related event.
    """
    @property
    def actor(self) -> Mob:
        """
        The `Mob` which is involved in this event.
        """

class ActorCollideWithActorEvent(Event, Cancellable):
    """
    Called when two actors collide with each other.

    If this event is cancelled, the actors will not be pushed away from each other. Cancelling also stops
    either actor from being pulled onto the other when the other is a rideable vehicle, so a listener that
    cancels every collision also stops boats and minecarts from being boarded by walking into them.

    The server fires this before it decides whether the collision leads to a push, so it is also called for
    pairs the server then leaves alone, and it is called more than once per tick for a pair that keeps
    overlapping.
    """
    @property
    def actors(self) -> list[Actor]:
        """
        The actors that are involved in this event.
        """

class ActorDamageEvent(MobEvent, Cancellable):
    """
    Called when an `Actor` is damaged.
    """
    @property
    def damage(self) -> float:
        """
        The raw amount of damage caused by the event.
        """

    @damage.setter
    def damage(self, arg1: float) -> None: ...
    @property
    def damage_source(self) -> DamageSource:
        """
        A `DamageSource` detailing the source of the damage.
        """

class ActorDeathEvent(MobEvent):
    """
    Called when an `Actor` dies.
    """
    @property
    def damage_source(self) -> DamageSource:
        """
        A `DamageSource` detailing the source of the damage for the death.
        """

class PlayerDeathEvent(ActorDeathEvent):
    """
    Called when a `Player` dies.
    """
    @property
    def player(self) -> Player:
        """
        The `Player` which is involved in this event.
        """

    @property
    def death_message(self) -> str | Translatable | None:
        """
        The death message that will appear to everyone on the server.
        """

    @death_message.setter
    def death_message(self, arg1: str | Translatable | None) -> None: ...

class ActorExplodeEvent(ActorEvent, Cancellable):
    """
    Called when an actor explodes.
    """
    @property
    def location(self) -> Location:
        """
        The location where the explosion happened.

        It is not possible to get this value from the `Entity` as the `Entity` no longer exists in the
        world.
        """

    @property
    def block_list(self) -> list[Block]:
        """
        The list of blocks that would have been removed or were removed from the explosion event.
        """

    @block_list.setter
    def block_list(self, arg1: list[Block]) -> None: ...

class ActorEffectEvent(MobEvent, Cancellable):
    """
    Called when an effect on a `Mob` changes.

    This is fired before the change is applied. Cancelling the event prevents it, and the effect may be
    replaced with a different one by assigning to `effect`.
    """
    class Action(enum.Enum):
        """
        An enum to specify how the effect changed.
        """

        ADDED = 0

    ADDED = Action.ADDED
    @property
    def action(self) -> Action:
        """
        How the effect changed.
        """

    @property
    def effect(self) -> Effect:
        """
        The effect involved in this event.
        """

    @effect.setter
    def effect(self, arg1: Effect) -> None: ...

class ActorDismountEvent(ActorEvent, Cancellable):
    """
    Called when an `Actor` stops riding another `Actor`.
    """
    @property
    def vehicle(self) -> Actor:
        """
        The actor that is being dismounted.
        """

class ActorChangeBlockEvent(ActorEvent, Cancellable):
    """
    Called when an `Actor` changes a block through its own behaviour, such as a creeper exploding, an
    enderman picking a block up, a ravager trampling crops or a zombie breaking a door.

    Unlike Bukkit's equivalent, this covers only the mob griefing paths. It is not called for falling
    blocks landing or for sheep eating grass, and the resulting block state is not available.
    """
    @property
    def block(self) -> Block:
        """
        The block that will be changed.
        """

class ActorKnockbackEvent(MobEvent, Cancellable):
    """
    Called when a living entity receives knockback.
    """
    @property
    def source(self) -> Actor | None:
        """
        The source actor that has caused knockback to the defender, or `None` if the knockback is not caused by an actor.
        """

    @property
    def knockback(self) -> Vector:
        """
        The knockback that will be applied to the entity.

        Note: the getter returns a copy; changes must be applied via the setter.
        """

    @knockback.setter
    def knockback(self, arg1: Vector) -> None: ...

class ActorPickupItemEvent(ActorEvent, Cancellable):
    """
    Called when an `Actor` picks an item up from the ground.

    This is not called for players; see `PlayerPickupItemEvent` instead.
    """
    @property
    def item(self) -> Item:
        """
        The Item picked up by the actor.
        """

    @property
    def amount(self) -> int:
        """
        The number of items that will be picked up from the stack.
        """

class ActorRemoveEvent(ActorEvent):
    """
    Called when an `Actor` is removed.

    This event should only be used for monitoring. Modifying the actor during or after this event
    leads to undefined behaviours. This event will not be called for `Player`s.
    """

class ActorSpawnEvent(ActorEvent, Cancellable):
    """
    Called when an `Actor` is spawned into a world.

    If an Actor Spawn event is cancelled, the actor will not spawn.
    """

class ActorTeleportEvent(ActorEvent, Cancellable):
    """
    Called when a non-player entity is teleported from one location to another.

    This may be as a result of natural causes (Enderman, Shulker), pathfinding (Wolf), or commands
    (/teleport).
    """
    @property
    def from_location(self) -> Location:
        """
        The location that this actor moved from.
        """

    @from_location.setter
    def from_location(self, arg1: Location) -> None: ...
    @property
    def to_location(self) -> Location:
        """
        The location that this actor moved to.
        """

    @to_location.setter
    def to_location(self, arg1: Location) -> None: ...

class ActorToggleGlideEvent(MobEvent):
    """
    Called when an `Actor`'s gliding state is toggled with an elytra.
    """
    @property
    def is_gliding(self) -> bool:
        """
        Whether the actor is now gliding or not.
        """

class ActorToggleSwimEvent(MobEvent):
    """
    Called when an `Actor`'s swimming state is toggled.
    """
    @property
    def is_swimming(self) -> bool:
        """
        Whether the actor is now swimming or not.
        """

class BlockEvent(Event):
    """
    Represents an `Block`-related event.
    """
    @property
    def block(self) -> Block:
        """
        The `Block` which is involved in this event.
        """

class BlockBreakEvent(BlockEvent, Cancellable):
    """
    Called when a block is broken by a player.

    If a `BlockBreakEvent` is cancelled, the block will not break and experience will not drop.
    """
    @property
    def player(self) -> Player:
        """
        The `Player` that is breaking the block involved in this event.
        """

class BlockExplodeEvent(BlockEvent, Cancellable):
    """
    Called when a block explodes (e.g. bed in the Nether, respawn anchor in the Overworld).

    If a `BlockExplodeEvent` is cancelled, the explosion will not occur.
    """
    @property
    def block_list(self) -> list[Block]:
        """
        The list of blocks that would have been removed or were removed from the explosion event.
        """

    @block_list.setter
    def block_list(self, arg1: list[Block]) -> None: ...

class BlockCookEvent(BlockEvent, Cancellable):
    """
    Called when an `ItemStack` is successfully cooked in a block.
    """
    @property
    def source(self) -> ItemStack:
        """
        The smelted (source) `ItemStack` for this event.
        """

    @property
    def result(self) -> ItemStack:
        """
        The resultant `ItemStack` for this event.
        """

    @result.setter
    def result(self, arg1: ItemStack) -> None: ...

class BlockGrowEvent(BlockEvent, Cancellable):
    """
    Called when a block grows naturally in the world.

    If a Block Grow event is cancelled, the block will not grow.
    """
    @property
    def new_state(self) -> BlockState:
        """
        The new state of the block after it has grown.
        """

class BlockFormEvent(BlockGrowEvent):
    """
    Called when a block is formed or spreads based on world conditions.

    Use `BlockSpreadEvent` to catch blocks that actually spread and don't just "randomly" form.

    Examples:
        - Snow forming due to a snow storm.
        - Ice forming in a snowy Biome like Taiga or Tundra.
        - Obsidian / Cobblestone forming due to contact with water.
        - Concrete forming due to mixing of concrete powder and water.

    If a Block Form event is cancelled, the block will not be formed.
    """

class BlockFromToEvent(BlockEvent, Cancellable):
    """
    Represents events with a source block and a destination block, currently only applies to liquid
    (lava and water) and teleporting dragon eggs.

    If a Block From To event is cancelled, the block will not move (the liquid will not flow).
    """
    @property
    def to_block(self) -> Block:
        """
        The faced `Block`.
        """

class BlockPistonEvent(BlockEvent, Cancellable):
    """
    Called when a piston block is triggered.
    """
    @property
    def direction(self) -> BlockFace:
        """
        The direction in which the piston will operate.
        """

class BlockPistonExtendEvent(BlockPistonEvent):
    """
    Called when a piston extends.
    """

class BlockPistonRetractEvent(BlockPistonEvent):
    """
    Called when a piston retracts.
    """

class BlockPlaceEvent(BlockEvent, Cancellable):
    """
    Called when a block is placed by a player.

    If a `BlockPlaceEvent` is cancelled, the block will not be placed.
    """
    @property
    def player(self) -> Player:
        """
        The `Player` who placed the block involved in this event.
        """

    @property
    def block_placed(self) -> Block:
        """
        The `Block` that was placed.
        """

    @property
    def block_replaced_state(self) -> BlockState:
        """
        The `BlockState` of the block that was replaced.
        """

    @property
    def block_against(self) -> Block:
        """
        The block that the new block was placed against.
        """

class LeavesDecayEvent(BlockEvent, Cancellable):
    """
    Called when leaves are decaying naturally.

    If a Leaves Decay event is cancelled, the leaves will not decay.
    """

class LevelEvent(Event):
    """
    Represents events within a level.
    """
    @property
    def level(self) -> Level:
        """
        The `Level` primarily involved with this event.
        """

class DimensionEvent(LevelEvent):
    """
    Represents events within a dimension.
    """
    @property
    def dimension(self) -> Dimension:
        """
        The `Dimension` primarily involved with this event.
        """

class ChunkEvent(DimensionEvent):
    """
    Represents a `Chunk` related event.
    """
    @property
    def chunk(self) -> Chunk:
        """
        The `Chunk` being loaded/unloaded.
        """

class ChunkLoadEvent(ChunkEvent):
    """
    Called when a chunk is loaded.
    """

class ChunkUnloadEvent(ChunkEvent):
    """
    Called when a chunk is unloaded.
    """

class PlayerEvent(Event):
    """
    Represents a player related event.
    """
    @property
    def player(self) -> Player:
        """
        The `Player` who is involved in this event.
        """

class PlayerArmSwingEvent(PlayerEvent):
    """
    Called when a player swings their arm.
    """
    @property
    def item(self) -> ItemStack | None:
        """
        The item the player was holding when they swung their arm.
        """

class PlayerBedEnterEvent(PlayerEvent, Cancellable):
    """
    Called when a player is almost about to enter the bed.
    """
    @property
    def bed(self) -> Block:
        """
        The bed block involved in this event.
        """

class PlayerBedLeaveEvent(PlayerEvent):
    """
    Called when a player is leaving a bed.
    """
    @property
    def bed(self) -> Block:
        """
        The bed block involved in this event.
        """

class PlayerBucketActorEvent(PlayerEvent, Cancellable):
    """
    Called when a player captures an actor in a bucket.
    """
    @property
    def actor(self) -> Actor:
        """
        The actor being captured.
        """

    @property
    def original_bucket(self) -> ItemStack:
        """
        The bucket used to capture the actor.
        """

    @property
    def hand(self) -> EquipmentSlot:
        """
        The hand used to capture the actor.
        """

class PlayerChatEvent(PlayerEvent, Cancellable):
    """
    Called when a player sends a chat message.
    """
    @property
    def message(self) -> str:
        """
        The message that the player is attempting to send.
        """

    @message.setter
    def message(self, arg1: str) -> None: ...
    @property
    def player(self) -> Player:
        """
        The player that this message will be displayed as being sent by.
        """

    @player.setter
    def player(self, arg1: Player) -> None: ...
    @property
    def format(self) -> str:
        """
        The format to use to display this chat message.

        See the format string syntax at https://en.cppreference.com/w/cpp/utility/format/spec.html.
        """

    @format.setter
    def format(self, arg1: str) -> None: ...
    @property
    def recipients(self) -> list[Player]:
        """
        The set of `Player`s who will see this chat message.
        """

class PlayerCommandEvent(PlayerEvent, Cancellable):
    """
    Called whenever a player runs a command.
    """
    @property
    def command(self) -> str:
        """
        The command that the player is attempting to send.
        """

    @command.setter
    def command(self, arg1: str) -> None: ...

class PlayerCraftItemEvent(PlayerEvent, Cancellable):
    """
    Called when a player crafts an item.

    If the event is cancelled the item will not be crafted and the ingredients will not be consumed.
    """
    @property
    def item(self) -> ItemStack:
        """
        An `ItemStack` for the item being crafted.
        """

    @property
    def recipe_id(self) -> str:
        """
        The identifier of the recipe used.
        """

    @property
    def amount(self) -> int:
        """
        The number of times the recipe is being crafted.
        """

class PlayerDimensionChangeEvent(PlayerEvent):
    """
    Called when a player switches to another dimension.
    """
    @property
    def from_dimension(self) -> Dimension:
        """
        The player's previous dimension.
        """

    @property
    def to_dimension(self) -> Dimension:
        """
        The player's new dimension.
        """

class PlayerDropItemEvent(PlayerEvent, Cancellable):
    """
    Called when a player drops an item from their inventory.
    """
    @property
    def item(self) -> ItemStack:
        """
        The `ItemStack` dropped by the player.
        """

class PlayerEditBookEvent(PlayerEvent, Cancellable):
    """
    Called when a player edits or signs a book.
    """
    @property
    def slot(self) -> int:
        """
        The inventory slot containing the book.
        """

    @property
    def previous_book_meta(self) -> BookMeta:
        """
        The book metadata before the edit.
        """

    @property
    def new_book_meta(self) -> BookMeta:
        """
        The book metadata that will be applied after the edit.
        """

    @new_book_meta.setter
    def new_book_meta(self, arg1: BookMeta) -> None: ...
    @property
    def is_signing(self) -> bool:
        """
        Whether the player is signing the book.
        """

    @is_signing.setter
    def is_signing(self, arg1: bool) -> None: ...

class PlayerEmoteEvent(PlayerEvent, Cancellable):
    """
    Called when a player uses an emote.
    """
    @property
    def emote_id(self) -> str:
        """
        The emote piece ID.
        """

    @property
    def is_muted(self) -> bool:
        """
        The muted state for the emote.

        When `True`, the emote is executed without sending a chat message about the emote.
        """

    @is_muted.setter
    def is_muted(self, arg1: bool) -> None: ...

class PlayerExpChangeEvent(PlayerEvent):
    """
    Called when a player's experience changes.
    """
    @property
    def amount(self) -> int:
        """
        The amount of experience the player will be given.
        """

    @amount.setter
    def amount(self, arg1: int) -> None: ...

class PlayerGameModeChangeEvent(PlayerEvent, Cancellable):
    """
    Called when the `GameMode` of the player is changed.
    """
    @property
    def new_game_mode(self) -> GameMode:
        """
        The `GameMode` the player is switched to.
        """

class PlayerInputEvent(PlayerEvent):
    """
    Called when a player sends updated input to the server.
    """
    @property
    def input(self) -> Input:
        """
        The new input received from this player.
        """

class PlayerInteractEvent(PlayerEvent, Cancellable):
    """
    Called when a player interacts with an object or air.
    """
    class Action(enum.Enum):
        """
        Represents the type of interaction that triggered this event.
        """

        LEFT_CLICK_BLOCK = 0
        RIGHT_CLICK_BLOCK = 1
        LEFT_CLICK_AIR = 2
        RIGHT_CLICK_AIR = 3

    LEFT_CLICK_BLOCK = Action.LEFT_CLICK_BLOCK
    RIGHT_CLICK_BLOCK = Action.RIGHT_CLICK_BLOCK
    LEFT_CLICK_AIR = Action.LEFT_CLICK_AIR
    RIGHT_CLICK_AIR = Action.RIGHT_CLICK_AIR
    @property
    def action(self) -> Action:
        """
        The action type of this interaction.
        """

    @property
    def has_item(self) -> bool:
        """
        `True` if this event involved an item.
        """

    @property
    def item(self) -> ItemStack | None:
        """
        The item in hand represented by this event, or `None` if no item.
        """

    @property
    def has_block(self) -> bool:
        """
        `True` if this event involved a block.
        """

    @property
    def block(self) -> Block | None:
        """
        The block clicked with this item.
        """

    @property
    def block_face(self) -> BlockFace:
        """
        The face of the block that was clicked.
        """

    @property
    def clicked_position(self) -> Vector | None:
        """
        The exact position on the block the player interacted with.

        This will be `None` outside of `Action.RIGHT_CLICK_BLOCK`. All vector components are between 0.0 and
        1.0 inclusive.
        """

class PlayerInteractActorEvent(PlayerEvent, Cancellable):
    """
    Called when a player right-clicks an actor.
    """
    @property
    def actor(self) -> Actor:
        """
        The actor that was right-clicked by the player.
        """

class PlayerArmorStandManipulateEvent(PlayerInteractActorEvent):
    """
    Called when a player interacts with an armor stand and will either swap, retrieve or place an item.
    """
    @property
    def armor_stand_item(self) -> ItemStack:
        """
        The item held by the armor stand in the affected slot.
        """

    @property
    def player_item(self) -> ItemStack:
        """
        The item held by the player during the interaction.
        """

    @property
    def slot(self) -> EquipmentSlot:
        """
        The armor stand equipment slot affected by the interaction.
        """

class PlayerItemConsumeEvent(PlayerEvent, Cancellable):
    """
    Called when a player is finishing consuming an item (food, potion, milk bucket).

    If the `ItemStack` is modified the server will use the effects of the new item and not remove the
    original one from the player's inventory.

    If the event is cancelled the effect will not be applied and the item will not be removed from
    the player's inventory.
    """
    @property
    def item(self) -> ItemStack:
        """
        An `ItemStack` for the item being consumed.
        """

    @property
    def hand(self) -> EquipmentSlot:
        """
        The hand used to consume the item.
        """

class PlayerItemHeldEvent(PlayerEvent, Cancellable):
    """
    Called when a player changes their currently held item.
    """
    @property
    def new_slot(self) -> int:
        """
        The new held slot index.
        """

    @property
    def previous_slot(self) -> int:
        """
        The previous held slot index.
        """

class PlayerToggleSneakEvent(PlayerEvent):
    """
    Called when a player toggles their sneaking state.
    """
    @property
    def is_sneaking(self) -> bool:
        """
        Whether the player is now sneaking or not.
        """

class PlayerToggleSprintEvent(PlayerEvent):
    """
    Called when a player toggles their sprinting state.
    """
    @property
    def is_sprinting(self) -> bool:
        """
        Whether the player is now sprinting or not.
        """

class PlayerToggleCrawlEvent(PlayerEvent):
    """
    Called when a player toggles their crawling state.
    """
    @property
    def is_crawling(self) -> bool:
        """
        Whether the player is now crawling or not.
        """

class PlayerToggleFlightEvent(PlayerEvent):
    """
    Called when a player toggles their flying state.
    """
    @property
    def is_flying(self) -> bool:
        """
        Whether the player is now flying or not.
        """

class PlayerJoinEvent(PlayerEvent):
    """
    Called when a player joins a server.
    """
    @property
    def join_message(self) -> str | Translatable | None:
        """
        The join message to send to all online players.
        """

    @join_message.setter
    def join_message(self, arg1: str | Translatable | None) -> None: ...

class PlayerKickEvent(PlayerEvent, Cancellable):
    """
    Called when a player gets kicked from the server.
    """
    @property
    def reason(self) -> str:
        """
        The reason why the player is getting kicked.
        """

    @reason.setter
    def reason(self, arg1: str) -> None: ...

class PlayerLevelChangeEvent(PlayerEvent):
    """
    Called when a player's level changes.
    """
    @property
    def old_level(self) -> int:
        """
        The player's level before the change.
        """

    @property
    def new_level(self) -> int:
        """
        The player's level after the change.
        """

class PlayerLoginEvent(PlayerEvent, Cancellable):
    """
    Called when a player attempts to login in.
    """
    @property
    def kick_message(self) -> str:
        """
        The kick message to display if the event is cancelled.
        """

    @kick_message.setter
    def kick_message(self, arg1: str) -> None: ...

class PlayerMoveEvent(PlayerEvent, Cancellable):
    """
    Called when a player moves.
    """
    @property
    def from_location(self) -> Location:
        """
        The location that this player moved from.
        """

    @from_location.setter
    def from_location(self, arg1: Location) -> None: ...
    @property
    def to_location(self) -> Location:
        """
        The location that this player moved to.
        """

    @to_location.setter
    def to_location(self, arg1: Location) -> None: ...

class PlayerJumpEvent(PlayerMoveEvent):
    """
    Called when a player jumps.
    """

class PlayerQuitEvent(PlayerEvent):
    """
    Called when a player leaves a server.
    """
    @property
    def quit_message(self) -> str | Translatable | None:
        """
        The quit message to send to all online players.
        """

    @quit_message.setter
    def quit_message(self, arg1: str | Translatable | None) -> None: ...

class PlayerRecipeBookClickEvent(PlayerEvent, Cancellable):
    """
    Called when a player clicks a recipe in the recipe book.

    If the event is cancelled the recipe will not be crafted and no ingredients will be consumed.
    """
    @property
    def recipe(self) -> str:
        """
        The identifier of the recipe clicked by the player.
        """

    @property
    def amount(self) -> int:
        """
        The number of times the recipe is being crafted.
        """

    @amount.setter
    def amount(self, arg1: int) -> None: ...

class PlayerRecipeBookSettingsChangeEvent(PlayerEvent):
    """
    Called when a player changes recipe book settings.
    """
    class RecipeBookType(enum.Enum):
        """
        The recipe book type.
        """

        CRAFTING = 0
        FURNACE = 1
        BLAST_FURNACE = 2
        SMOKER = 3

    CRAFTING = RecipeBookType.CRAFTING
    FURNACE = RecipeBookType.FURNACE
    BLAST_FURNACE = RecipeBookType.BLAST_FURNACE
    SMOKER = RecipeBookType.SMOKER
    @property
    def recipe_book_type(self) -> RecipeBookType:
        """
        The type of recipe book whose settings changed.
        """

    @property
    def is_filtering(self) -> bool:
        """
        Whether recipe filtering is enabled.
        """

    @property
    def is_open(self) -> bool:
        """
        Whether the recipe book is open.
        """

class PlayerRespawnEvent(PlayerEvent):
    """
    Called when a player respawns.
    """
    class RespawnReason(enum.Enum):
        """
        An enum to specify the reason a respawn occurred.
        """

        DEATH = 0
        END_PORTAL = 1

    DEATH = RespawnReason.DEATH
    END_PORTAL = RespawnReason.END_PORTAL
    @property
    def respawn_reason(self) -> RespawnReason:
        """
        The reason this respawn occurred.
        """

class PlayerRiptideEvent(PlayerEvent):
    """
    Called when a player activates the riptide enchantment, using their trident to propel them through the air.

    The riptide action is currently performed client side, so manipulating the player in this event may have
    undesired effects.
    """
    @property
    def item(self) -> ItemStack:
        """
        An `ItemStack` for the trident being used.
        """

class PlayerSetSpawnEvent(PlayerEvent, Cancellable):
    """
    Called when a player's spawn is set, either by themselves or otherwise.

    Assigning a new `location` redirects the spawn that is about to be written; cancelling leaves the respawn point
    untouched.

    Note:
        Only the location's block coordinates and dimension are written back; Bedrock does not persist yaw/pitch for
        a respawn point. Cancelling stops the respawn point from changing, but not the feedback around it:
        `/spawnpoint` still reports success and a respawn anchor still plays its sound and message, because neither
        consults the setter. The event is not fired when Bedrock clears a respawn point, so `/clearspawnpoint` and
        breaking the bed a player is bound to are both silent.
    """
    class Cause(enum.Enum):
        """
        The cause of the spawn change.
        """

        BED = 0
        RESPAWN_ANCHOR = 1
        COMMAND = 2
        PLUGIN = 3
        UNKNOWN = 4

    BED = Cause.BED
    RESPAWN_ANCHOR = Cause.RESPAWN_ANCHOR
    COMMAND = Cause.COMMAND
    PLUGIN = Cause.PLUGIN
    UNKNOWN = Cause.UNKNOWN
    @property
    def cause(self) -> Cause:
        """
        The cause of the spawn change.
        """

    @property
    def location(self) -> Location | None:
        """
        The spawn location, or `None` to remove the spawn location.
        """

    @location.setter
    def location(self, arg1: Location | None) -> None: ...

class PlayerShearActorEvent(PlayerEvent, Cancellable):
    """
    Called when a player shears an actor.
    """
    @property
    def actor(self) -> Actor:
        """
        The actor being sheared.
        """

    @property
    def item(self) -> ItemStack:
        """
        The shears used.
        """

    @property
    def hand(self) -> EquipmentSlot:
        """
        The hand used to shear the actor.
        """

class PlayerSkinChangeEvent(PlayerEvent, Cancellable):
    """
    Called when a player changes their skin.
    """
    @property
    def new_skin(self) -> Skin:
        """
        The skin that will be applied.
        """

    @property
    def skin_change_message(self) -> str | Translatable | None:
        """
        The message to send to all online players for this skin change.
        """

    @skin_change_message.setter
    def skin_change_message(self, arg1: str | Translatable | None) -> None: ...

class PlayerTeleportEvent(PlayerMoveEvent):
    """
    Called when a player is teleported from one location to another.
    """

class PlayerPortalEvent(PlayerTeleportEvent):
    """
    Called when a player is about to teleport because it is in contact with a portal.
    """

class PlayerPickupArrowEvent(PlayerEvent, Cancellable):
    """
    Called when a player picks up an arrow or a thrown trident from the ground.
    """
    @property
    def arrow(self) -> Actor:
        """
        The arrow picked up by the player.
        """

class PlayerPickupExperienceEvent(PlayerEvent, Cancellable):
    """
    Called when a player picks up an experience orb.

    Cancelling the event leaves the orb in the world.
    """
    @property
    def amount(self) -> int:
        """
        The amount of experience the orb is worth.
        """

class PlayerPickupItemEvent(PlayerEvent, Cancellable):
    """
    Called when a player picks an item up from the ground.
    """
    @property
    def item(self) -> Item:
        """
        The Item picked up by the entity.
        """

class ServerEvent(Event):
    """
    Represents a Server-related event.
    """

class BroadcastMessageEvent(ServerEvent, Cancellable):
    """
    Event triggered for server broadcast messages such as from `Server.broadcast`.

    This event should be async if fired from an async thread.
    """
    @property
    def message(self) -> str | Translatable:
        """
        The message to broadcast.
        """

    @message.setter
    def message(self, arg1: str | Translatable) -> None: ...
    @property
    def recipients(self) -> set[CommandSender]:
        """
        The set of `CommandSender`s who will see this broadcast message.
        """

class MapInitializeEvent(ServerEvent):
    """
    Called when a map is initialized.
    """
    @property
    def map(self) -> MapView:
        """
        The `Map` initialized in this event.
        """

class PacketReceiveEvent(ServerEvent, Cancellable):
    """
    Called when the server receives a packet from a connected client.
    """
    @property
    def packet_id(self) -> int:
        """
        The ID of the packet.
        """

    @property
    def payload(self) -> bytes:
        """
        The raw packet data, excluding the header.
        """

    @payload.setter
    def payload(self, arg1: bytes) -> None: ...
    @property
    def player(self) -> Player | None:
        """
        The `Player` who is involved in this event.

        This may return `None` if the packet is sent before the player completes the login process.
        """

    @property
    def address(self) -> SocketAddress:
        """
        The network address of the client that sent this packet.
        """

    @property
    def sub_client_id(self) -> int:
        """
        The SubClient ID.

        Range is 0 to 3 (0 = primary client; 1-3 = split-screen clients).
        """

class PacketSendEvent(ServerEvent, Cancellable):
    """
    Called when the server sends a packet to a connected client.
    """
    @property
    def packet_id(self) -> int:
        """
        The ID of the packet.
        """

    @property
    def payload(self) -> bytes:
        """
        The raw packet data, excluding the header.
        """

    @payload.setter
    def payload(self, arg1: bytes) -> None: ...
    @property
    def player(self) -> Player | None:
        """
        The `Player` who is involved in this event.

        This may return `None` if the packet is sent before the player completes the login process.
        """

    @property
    def address(self) -> SocketAddress:
        """
        The network address to which this packet is being sent.
        """

    @property
    def sub_client_id(self) -> int:
        """
        The SubClient ID.

        Range is 0 to 3 (0 = primary client; 1-3 = split-screen clients).
        """

class PluginEnableEvent(ServerEvent):
    """
    Called when a plugin is enabled.
    """
    @property
    def plugin(self) -> Plugin:
        """
        The `Plugin` involved in this event.
        """

class PluginDisableEvent(ServerEvent):
    """
    Called when a plugin is disabled.
    """
    @property
    def plugin(self) -> Plugin:
        """
        The `Plugin` involved in this event.
        """

class ScriptMessageEvent(ServerEvent, Cancellable):
    """
    Called when a message is sent by the `/scriptevent` command.
    """
    @property
    def message_id(self) -> str:
        """
        The message id to send.
        """

    @property
    def message(self) -> str:
        """
        The message to send.
        """

    @property
    def sender(self) -> CommandSender:
        """
        The command sender who sent the script message.
        """

class ServerCommandEvent(ServerEvent, Cancellable):
    """
    Called when a command is run by a non-player, early in the command handling process.

    You should not use this except for a few cases like logging commands, blocking commands on
    certain places, or applying modifiers.

    The command message contains a slash '/' at the start.
    """
    @property
    def sender(self) -> CommandSender:
        """
        The command sender.
        """

    @property
    def command(self) -> str:
        """
        The command that the server is attempting to execute from the console.
        """

    @command.setter
    def command(self, arg1: str) -> None: ...

class ServerListPingEvent(ServerEvent, Cancellable):
    """
    Called when a server ping is coming in.
    """
    @property
    def address(self) -> SocketAddress:
        """
        The address the ping is coming from.
        """

    @property
    def server_guid(self) -> str:
        """
        The unique identifier of the server.
        """

    @server_guid.setter
    def server_guid(self, arg1: str) -> None: ...
    @property
    def local_port(self) -> int:
        """
        The local port of the server.
        """

    @local_port.setter
    def local_port(self, arg1: int) -> None: ...
    @property
    def local_port_v6(self) -> int:
        """
        The local port of the server for IPv6 support.
        """

    @local_port_v6.setter
    def local_port_v6(self, arg1: int) -> None: ...
    @property
    def motd(self) -> str:
        """
        The message of the day.
        """

    @motd.setter
    def motd(self, arg1: str) -> None: ...
    @property
    def network_protocol_version(self) -> int:
        """
        The network protocol version of this server.
        """

    @property
    def minecraft_version_network(self) -> str:
        """
        The network version of Minecraft that is supported by this server.
        """

    @minecraft_version_network.setter
    def minecraft_version_network(self, arg1: str) -> None: ...
    @property
    def num_players(self) -> int:
        """
        The number of players online.
        """

    @num_players.setter
    def num_players(self, arg1: int) -> None: ...
    @property
    def max_players(self) -> int:
        """
        The maximum number of players allowed.
        """

    @max_players.setter
    def max_players(self, arg1: int) -> None: ...
    @property
    def level_name(self) -> str:
        """
        The level name.
        """

    @level_name.setter
    def level_name(self, arg1: str) -> None: ...
    @property
    def game_mode(self) -> GameMode:
        """
        The current game mode.
        """

    @game_mode.setter
    def game_mode(self, arg1: GameMode) -> None: ...

class ServerLoadEvent(ServerEvent):
    """
    Called when either the server startup or reload has completed.
    """
    class LoadType(enum.Enum):
        """
        Represents the context in which the server was loaded.
        """

        STARTUP = 0
        RELOAD = 1

    STARTUP = LoadType.STARTUP
    RELOAD = LoadType.RELOAD
    @property
    def type(self) -> LoadType:
        """
        The load type of this event.
        """

class WeatherEvent(Event):
    """
    Represents a Weather-related event.
    """
    @property
    def level(self) -> Level:
        """
        The `Level` where this event is occurring.
        """

class ThunderChangeEvent(WeatherEvent, Cancellable):
    """
    Called when the thunder state in a world is changing.
    """
    @property
    def to_thunder_state(self) -> bool:
        """
        `True` if the weather is being set to thundering, `False` otherwise.
        """

class WeatherChangeEvent(WeatherEvent, Cancellable):
    """
    Called when the weather (rain) state in a world is changing.
    """
    @property
    def to_weather_state(self) -> bool:
        """
        `True` if the weather is being set to raining, `False` otherwise.
        """

@typing.overload
def event_handler(func: _F, /) -> _F: ...
@typing.overload
def event_handler(*, priority: EventPriority = ..., ignore_cancelled: bool = ...) -> typing.Callable[[_F], _F]: ...
