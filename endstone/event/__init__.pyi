"""
Classes relating to handling triggered code executions.
"""

import enum

from endstone import GameMode, Player, Skin
from endstone.actor import Actor, Item, Mob
from endstone.block import Block, BlockFace, BlockState
from endstone.command import CommandSender
from endstone.damage import DamageSource
from endstone.inventory import EquipmentSlot, ItemStack
from endstone.lang import Translatable
from endstone.level import Chunk, Dimension, Level, Location
from endstone.map import MapView
from endstone.plugin import Plugin
from endstone.util import SocketAddress, Vector

__all__ = [
    "ActorDamageEvent",
    "ActorDeathEvent",
    "ActorEvent",
    "ActorExplodeEvent",
    "ActorKnockbackEvent",
    "ActorRemoveEvent",
    "ActorSpawnEvent",
    "ActorTeleportEvent",
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
    "PlayerBedEnterEvent",
    "PlayerBedLeaveEvent",
    "PlayerChatEvent",
    "PlayerCommandEvent",
    "PlayerDeathEvent",
    "PlayerDimensionChangeEvent",
    "PlayerDropItemEvent",
    "PlayerEmoteEvent",
    "PlayerEvent",
    "PlayerGameModeChangeEvent",
    "PlayerInteractActorEvent",
    "PlayerInteractEvent",
    "PlayerItemConsumeEvent",
    "PlayerItemHeldEvent",
    "PlayerJoinEvent",
    "PlayerJumpEvent",
    "PlayerKickEvent",
    "PlayerLoginEvent",
    "PlayerMoveEvent",
    "PlayerPickupItemEvent",
    "PlayerPortalEvent",
    "PlayerQuitEvent",
    "PlayerRespawnEvent",
    "PlayerSkinChangeEvent",
    "PlayerTeleportEvent",
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

class EventPriority(enum.IntEnum):
    """
    Listeners are called in following order: LOWEST -> LOW -> NORMAL -> HIGH -> HIGHEST -> MONITOR
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
    @property
    def event_name(self) -> str:
        """
        Gets a user-friendly identifier for this event.
        """
        ...
    @property
    def is_asynchronous(self) -> bool:
        """
        Whether the event fires asynchronously.
        """
        ...

class EventResult(enum.Enum):
    DENY = 0
    DEFAULT = 1
    ALLOW = 2

class Cancellable:
    """
    Represents an event that may be cancelled by a plugin or the server.
    """
    @property
    def cancelled(self) -> bool:
        """
        Gets or sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins. [Warning] Deprecated: Use is_cancelled instead.
        """
        ...
    @cancelled.setter
    def cancelled(self, arg1: bool) -> None: ...
    @property
    def is_cancelled(self) -> bool:
        """
        Gets or sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins.
        """
        ...
    @is_cancelled.setter
    def is_cancelled(self, arg1: bool) -> None: ...
    def cancel(self) -> None:
        """
        Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins.
        """
        ...

class ActorEvent(Event):
    """
    Represents an Actor-related event.
    """
    @property
    def actor(self) -> Actor:
        """
        Returns the Actor involved in this event
        """
        ...

class MobEvent(Event):
    """
    Represents an Mob-related event.
    """
    @property
    def actor(self) -> Mob:
        """
        Returns the Mob involved in this event
        """
        ...

class ActorDamageEvent(MobEvent, Cancellable):
    """
    Called when an Actor is damaged.
    """
    @property
    def damage(self) -> float:
        """
        Gets or sets the amount of damage caused by the event
        """
        ...
    @damage.setter
    def damage(self, arg1: float) -> None: ...
    @property
    def damage_source(self) -> DamageSource:
        """
        Gets the source of damage.
        """
        ...

class ActorDeathEvent(MobEvent):
    """
    Called when an Actor dies.
    """
    @property
    def damage_source(self) -> DamageSource:
        """
        Gets the source of damage which caused the death.
        """
        ...

class PlayerDeathEvent(ActorDeathEvent):
    """
    Called when a player dies
    """
    @property
    def player(self) -> Player:
        """
        Gets the Player that is breaking the block involved in this event.
        """
        ...
    @property
    def death_message(self) -> str | Translatable | None:
        """
        Gets or sets the death message that will appear to everyone on the server.
        """
        ...
    @death_message.setter
    def death_message(self, arg1: str | Translatable | None) -> None: ...

class ActorExplodeEvent(ActorEvent, Cancellable):
    """
    Called when an Actor explodes.
    """
    @property
    def location(self) -> Location:
        """
        Returns the location where the explosion happened.
        """
        ...
    @property
    def block_list(self) -> list[Block]:
        """
        Gets or sets the list of blocks that would have been removed or were removed from the explosion event.
        """
        ...
    @block_list.setter
    def block_list(self, arg1: list[Block]) -> None: ...

class ActorKnockbackEvent(MobEvent, Cancellable):
    """
    Called when a living entity receives knockback.
    """
    @property
    def source(self) -> Actor:
        """
        Get the source actor that has caused knockback to the defender, if exists.
        """
        ...
    @property
    def knockback(self) -> Vector:
        """
        Gets or sets the knockback that will be applied to the entity.
        """
        ...
    @knockback.setter
    def knockback(self, arg1: Vector) -> None: ...

class ActorRemoveEvent(ActorEvent):
    """
    Called when an Actor is removed.
    """

class ActorSpawnEvent(ActorEvent, Cancellable):
    """
    Called when an Actor is spawned into a world.
    """

class ActorTeleportEvent(ActorEvent, Cancellable):
    """
    Called when a non-player entity is teleported from one location to another.
    """
    @property
    def from_location(self) -> Location:
        """
        Gets or sets the location that this actor moved from.
        """
        ...
    @from_location.setter
    def from_location(self, arg1: Location) -> None: ...
    @property
    def to_location(self) -> Location:
        """
        Gets or sets the location that this actor moved to.
        """
        ...
    @to_location.setter
    def to_location(self, arg1: Location) -> None: ...

class BlockEvent(Event):
    """
    Represents an Block-related event
    """
    @property
    def block(self) -> Block:
        """
        Gets the block involved in this event.
        """
        ...

class BlockBreakEvent(BlockEvent, Cancellable):
    """
    Called when a block is broken by a player.
    """
    @property
    def player(self) -> Player:
        """
        Gets the Player that is breaking the block involved in this event.
        """
        ...

class BlockExplodeEvent(BlockEvent, Cancellable):
    """
    Called when a block explodes.
    """
    @property
    def block_list(self) -> list[Block]:
        """
        Gets or sets the list of blocks that would have been removed or were removed from the explosion event.
        """
        ...
    @block_list.setter
    def block_list(self, arg1: list[Block]) -> None: ...

class BlockCookEvent(BlockEvent, Cancellable):
    """
    Called when an ItemStack is successfully cooked in a block.
    """
    @property
    def source(self) -> ItemStack:
        """
        Gets the smelted ItemStack for this event
        """
        ...
    @property
    def result(self) -> ItemStack:
        """
        Gets or sets the resultant ItemStack for this event
        """
        ...
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
        Gets the state of the block where it will form or spread to.
        """
        ...

class BlockFormEvent(BlockGrowEvent):
    """
    Called when a block is formed or spreads based on world conditions.
    If a Block Form event is cancelled, the block will not be formed.
    """

class BlockFromToEvent(BlockEvent, Cancellable):
    """
    Represents events with a source block and a destination block, currently only applies to liquid (lava and water) and teleporting dragon eggs.
    If a Block From To event is cancelled, the block will not move (the liquid will not flow).
    """
    @property
    def to_block(self) -> Block:
        """
        Gets the faced Block.
        """
        ...

class BlockPistonEvent(BlockEvent, Cancellable):
    """
    Called when a piston block is triggered
    """
    @property
    def direction(self) -> BlockFace:
        """
        Return the direction in which the piston will operate.
        """
        ...

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
    """
    @property
    def player(self) -> Player:
        """
        Gets the player who placed the block involved in this event.
        """
        ...
    @property
    def block_placed_state(self) -> BlockState:
        """
        Gets the BlockState for the block which was placed.
        """
        ...
    @property
    def block_replaced(self) -> Block:
        """
        Gets the block which was replaced.
        """
        ...
    @property
    def block_against(self) -> Block:
        """
        Gets the block that this block was placed against
        """
        ...

class LeavesDecayEvent(BlockEvent, Cancellable):
    """
    Called when leaves are decaying naturally.
    If a Leaves Decay event is cancelled, the leaves will not decay.
    """

class LevelEvent(Event):
    """
    Represents events within a level
    """
    @property
    def level(self) -> Level:
        """
        Gets the level primarily involved with this event
        """
        ...

class DimensionEvent(LevelEvent):
    """
    Represents events within a dimension
    """
    @property
    def dimension(self) -> Dimension:
        """
        Gets the dimension primarily involved with this event
        """
        ...

class ChunkEvent(DimensionEvent):
    """
    Represents a Chunk related event
    """
    @property
    def chunk(self) -> Chunk:
        """
        Gets the chunk being loaded/unloaded
        """
        ...

class ChunkLoadEvent(ChunkEvent):
    """
    Called when a chunk is loaded
    """

class ChunkUnloadEvent(ChunkEvent):
    """
    Called when a chunk is unloaded
    """

class PlayerEvent(Event):
    """
    Represents a player related event
    """
    @property
    def player(self) -> Player:
        """
        Returns the player involved in this event.
        """
        ...

class PlayerBedEnterEvent(PlayerEvent, Cancellable):
    """
    Called when a player is almost about to enter the bed.
    """
    @property
    def bed(self) -> Block:
        """
        Returns the bed block involved in this event.
        """
        ...

class PlayerBedLeaveEvent(PlayerEvent):
    """
    Called when a player is leaving a bed.
    """
    @property
    def bed(self) -> Block:
        """
        Returns the bed block involved in this event.
        """
        ...

class PlayerChatEvent(PlayerEvent, Cancellable):
    """
    Called when a player sends a chat message.
    """
    @property
    def message(self) -> str:
        """
        Gets or sets the message that the player will send.
        """
        ...
    @message.setter
    def message(self, arg1: str) -> None: ...
    @property
    def player(self) -> Player:
        """
        Gets or sets the player that this message will display as
        """
        ...
    @player.setter
    def player(self, arg1: Player) -> None: ...
    @property
    def format(self) -> str:
        """
        Sets the format to use to display this chat message
        """
        ...
    @format.setter
    def format(self, arg1: str) -> None: ...
    @property
    def recipients(self) -> list[Player]:
        """
        Gets a set of recipients that this chat message will be displayed to
        """
        ...

class PlayerCommandEvent(PlayerEvent, Cancellable):
    """
    Called whenever a player runs a command.
    """
    @property
    def command(self) -> str:
        """
        Gets or sets the command that the player will send.
        """
        ...
    @command.setter
    def command(self, arg1: str) -> None: ...

class PlayerDimensionChangeEvent(PlayerEvent):
    """
    Called when a player switches to another dimension.
    """
    @property
    def from_dimension(self) -> Dimension:
        """
        Gets the dimension the player is switching from.
        """
        ...
    @property
    def to_dimension(self) -> Dimension:
        """
        Gets the dimension the player is switching to.
        """
        ...

class PlayerDropItemEvent(PlayerEvent, Cancellable):
    """
    Called when a player drops an item from their inventory
    """
    @property
    def item(self) -> ItemStack:
        """
        Gets the ItemStack dropped by the player
        """
        ...

class PlayerEmoteEvent(PlayerEvent, Cancellable):
    """
    Called when a player uses and emote
    """
    @property
    def emote_id(self) -> str:
        """
        Gets the emote piece ID
        """
        ...
    @property
    def is_muted(self) -> bool:
        """
        Gets or sets the muted state for the emote.
        """
        ...
    @is_muted.setter
    def is_muted(self, arg1: bool) -> None: ...

class PlayerGameModeChangeEvent(PlayerEvent, Cancellable):
    """
    Called when the GameMode of the player is changed.
    """
    @property
    def new_game_mode(self) -> GameMode:
        """
        Gets the GameMode the player is switched to.
        """
        ...

class PlayerInteractEvent(PlayerEvent, Cancellable):
    """
    Represents an event that is called when a player right-clicks a block.
    """
    class Action(enum.Enum):
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
        Returns the action type of interaction
        """
        ...
    @property
    def has_item(self) -> bool:
        """
        Check if this event involved an item
        """
        ...
    @property
    def item(self) -> ItemStack | None:
        """
        Returns the item in hand represented by this event
        """
        ...
    @property
    def has_block(self) -> bool:
        """
        Check if this event involved a block
        """
        ...
    @property
    def block(self) -> Block:
        """
        Returns the clicked block
        """
        ...
    @property
    def block_face(self) -> BlockFace:
        """
        Returns the face of the block that was clicked
        """
        ...
    @property
    def clicked_position(self) -> Vector | None:
        """
        Gets the exact position on the block the player interacted with.
        """
        ...

class PlayerInteractActorEvent(PlayerEvent, Cancellable):
    """
    Represents an event that is called when a player right-clicks an actor.
    """
    @property
    def actor(self) -> Actor:
        """
        Gets the actor that was right-clicked by the player.
        """
        ...

class PlayerItemConsumeEvent(PlayerEvent, Cancellable):
    """
    Called when a player is finishing consuming an item (food, potion, milk bucket).
    """
    @property
    def item(self) -> ItemStack:
        """
        Gets the item that is being consumed.
        """
        ...
    @property
    def hand(self) -> EquipmentSlot:
        """
        Get the hand used to consume the item.
        """
        ...

class PlayerItemHeldEvent(PlayerEvent, Cancellable):
    """
    Called when a player changes their currently held item.
    """
    @property
    def new_slot(self) -> int:
        """
        Gets the new held slot index
        """
        ...
    @property
    def previous_slot(self) -> int:
        """
        Gets the previous held slot index.
        """
        ...

class PlayerJoinEvent(PlayerEvent):
    """
    Called when a player joins a server
    """
    @property
    def join_message(self) -> str | Translatable | None:
        """
        Gets or sets the join message to send to all online players.
        """
        ...
    @join_message.setter
    def join_message(self, arg1: str | Translatable | None) -> None: ...

class PlayerKickEvent(PlayerEvent, Cancellable):
    """
    Called when a player gets kicked from the server
    """
    @property
    def reason(self) -> str:
        """
        Gets or sets the reason why the player is getting kicked
        """
        ...
    @reason.setter
    def reason(self, arg1: str) -> None: ...

class PlayerLoginEvent(PlayerEvent, Cancellable):
    """
    Called when a player attempts to login in.
    """
    @property
    def kick_message(self) -> str:
        """
        Gets or sets kick message to display if event is cancelled
        """
        ...
    @kick_message.setter
    def kick_message(self, arg1: str) -> None: ...

class PlayerMoveEvent(PlayerEvent, Cancellable):
    """
    Called when a player moves.
    """
    @property
    def from_location(self) -> Location:
        """
        Gets or sets the location that this player moved from.
        """
        ...
    @from_location.setter
    def from_location(self, arg1: Location) -> None: ...
    @property
    def to_location(self) -> Location:
        """
        Gets or sets the location that this player moved to.
        """
        ...
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
        Gets or sets the quit message to send to all online players.
        """
        ...
    @quit_message.setter
    def quit_message(self, arg1: str | Translatable | None) -> None: ...

class PlayerRespawnEvent(PlayerEvent):
    """
    Called when a player respawns.
    """

class PlayerSkinChangeEvent(PlayerEvent, Cancellable):
    """
    Called when a player changes their skin.
    """
    @property
    def new_skin(self) -> Skin:
        """
        Gets the player's new skin.
        """
        ...
    @property
    def skin_change_message(self) -> str | Translatable | None:
        """
        Gets or sets the message to send to all online players for this skin change.
        """
        ...
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

class PlayerPickupItemEvent(PlayerEvent, Cancellable):
    """
    Called when a player picks an item up from the ground.
    """
    @property
    def item(self) -> Item:
        """
        Gets the Item picked up by the entity.
        """
        ...

class ServerEvent(Event):
    """
    Represents a server-related event
    """

class BroadcastMessageEvent(ServerEvent, Cancellable):
    """
    Event triggered for server broadcast messages such as from Server.broadcast
    """
    @property
    def message(self) -> str | Translatable:
        """
        Gets or sets the message to broadcast.
        """
        ...
    @message.setter
    def message(self, arg1: str | Translatable) -> None: ...
    @property
    def recipients(self) -> set[CommandSender]:
        """
        Gets a set of recipients that this broadcast message will be displayed to.
        """
        ...

class MapInitializeEvent(ServerEvent):
    """
    Called when a map is initialized.
    """
    @property
    def map(self) -> MapView:
        """
        Gets the map initialized in this event.
        """
        ...

class PacketReceiveEvent(ServerEvent, Cancellable):
    """
    Called when the server receives a packet from a connected client.
    """
    @property
    def packet_id(self) -> int:
        """
        Gets the ID of the packet.
        """
        ...
    @property
    def payload(self) -> bytes:
        """
        Gets or sets the raw packet data **excluding** the header.
        """
        ...
    @payload.setter
    def payload(self, arg1: bytes) -> None: ...
    @property
    def player(self) -> Player:
        """
        Gets the player involved in this event
        NOTE: This may return None if the packet is sent before the player completes the login process.
        """
        ...
    @property
    def address(self) -> SocketAddress:
        """
        Gets the network address to which this packet is being sent.
        """
        ...
    @property
    def sub_client_id(self) -> int:
        """
        Gets the SubClient ID (0 = primary client; 1-3 = split-screen clients).
        """
        ...

class PacketSendEvent(ServerEvent, Cancellable):
    """
    Called when the server sends a packet to a connected client.
    """
    @property
    def packet_id(self) -> int:
        """
        Gets the ID of the packet.
        """
        ...
    @property
    def payload(self) -> bytes:
        """
        Gets or sets the raw packet data **excluding** the header.
        """
        ...
    @payload.setter
    def payload(self, arg1: bytes) -> None: ...
    @property
    def player(self) -> Player:
        """
        Gets the player involved in this event
        NOTE: This may return None if the packet is sent before the player completes the login process.
        """
        ...
    @property
    def address(self) -> SocketAddress:
        """
        Gets the network address to which this packet is being sent.
        """
        ...
    @property
    def sub_client_id(self) -> int:
        """
        Gets the SubClient ID (0 = primary client; 1-3 = split-screen clients).
        """
        ...

class PluginEnableEvent(ServerEvent):
    """
    Called when a plugin is enabled.
    """
    @property
    def plugin(self) -> Plugin: ...

class PluginDisableEvent(ServerEvent):
    """
    Called when a plugin is disabled.
    """
    @property
    def plugin(self) -> Plugin: ...

class ScriptMessageEvent(ServerEvent, Cancellable):
    """
    Called when a message is sent by `/scriptevent` command
    """
    @property
    def message_id(self) -> str:
        """
        Get the message id to send.
        """
        ...
    @property
    def message(self) -> str:
        """
        Get the message to send.
        """
        ...
    @property
    def sender(self) -> CommandSender:
        """
        Gets the command sender who initiated the command.
        """
        ...

class ServerCommandEvent(ServerEvent, Cancellable):
    """
    Called when the console runs a command, early in the process.
    """
    @property
    def sender(self) -> CommandSender:
        """
        Get the command sender.
        """
        ...
    @property
    def command(self) -> str:
        """
        Gets or sets the command that the server will execute
        """
        ...
    @command.setter
    def command(self, arg1: str) -> None: ...

class ServerListPingEvent(ServerEvent, Cancellable):
    """
    Called when a server ping is coming in.
    """
    @property
    def address(self) -> SocketAddress:
        """
        Get the address the ping is coming from.
        """
        ...
    @property
    def server_guid(self) -> str:
        """
        Get or set the unique identifier of the server.
        """
        ...
    @server_guid.setter
    def server_guid(self, arg1: str) -> None: ...
    @property
    def local_port(self) -> int:
        """
        Get or set the local port of the server.
        """
        ...
    @local_port.setter
    def local_port(self, arg1: int) -> None: ...
    @property
    def local_port_v6(self) -> int:
        """
        Get or set the local port of the server for IPv6 support
        """
        ...
    @local_port_v6.setter
    def local_port_v6(self, arg1: int) -> None: ...
    @property
    def motd(self) -> str:
        """
        Gets or sets the message of the day message.
        """
        ...
    @motd.setter
    def motd(self, arg1: str) -> None: ...
    @property
    def network_protocol_version(self) -> int:
        """
        Get the network protocol version of this server
        """
        ...
    @property
    def minecraft_version_network(self) -> str:
        """
        Gets or sets the network version of Minecraft that is supported by this server
        """
        ...
    @minecraft_version_network.setter
    def minecraft_version_network(self, arg1: str) -> None: ...
    @property
    def num_players(self) -> int:
        """
        Gets or sets the number of players online.
        """
        ...
    @num_players.setter
    def num_players(self, arg1: int) -> None: ...
    @property
    def max_players(self) -> int:
        """
        Gets or sets the maximum number of players allowed.
        """
        ...
    @max_players.setter
    def max_players(self, arg1: int) -> None: ...
    @property
    def level_name(self) -> str:
        """
        Gets or sets the level name.
        """
        ...
    @level_name.setter
    def level_name(self, arg1: str) -> None: ...
    @property
    def game_mode(self) -> GameMode:
        """
        Gets or sets the current game mode.
        """
        ...
    @game_mode.setter
    def game_mode(self, arg1: GameMode) -> None: ...

class ServerLoadEvent(Event):
    """
    Called when either the server startup or reload has completed.
    """
    class LoadType(enum.Enum):
        STARTUP = 0

    STARTUP = LoadType.STARTUP
    @property
    def type(self) -> LoadType: ...

class WeatherEvent(Event):
    """
    Represents a weather-related event
    """
    @property
    def level(self) -> Level:
        """
        Returns the Level where this event is occurring
        """
        ...

class ThunderChangeEvent(WeatherEvent, Cancellable):
    """
    Called when the thunder state in a world is changing.
    """
    @property
    def to_thunder_state(self) -> bool:
        """
        Gets the state of thunder that the world is being set to
        """
        ...

class WeatherChangeEvent(WeatherEvent, Cancellable):
    """
    Called when the weather (rain) state in a world is changing.
    """
    @property
    def to_weather_state(self) -> bool:
        """
        Gets the state of weather that the world is being set to
        """
        ...

def event_handler(
    func=None, *, priority: EventPriority = EventPriority.NORMAL, ignore_cancelled: bool = False
) -> None: ...
