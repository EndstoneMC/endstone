"""
Classes relating to handling triggered code executions.
"""

from __future__ import annotations

import collections.abc
import enum
import typing

from endstone import GameMode, Player, Skin
from endstone.actor import Actor, Item, Mob
from endstone.block import Block, BlockFace, BlockState
from endstone.command import CommandSender
from endstone.damage import DamageSource
from endstone.inventory import EquipmentSlot, ItemStack
from endstone.lang import Translatable
from endstone.level import Chunk, Dimension, Level, Location
from endstone.plugin import Plugin
from endstone.util import SocketAddress, Vector

__all__: list[str] = [
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
    "MobEvent",
    "PacketReceiveEvent",
    "PacketSendEvent",
    "PlayerBedEnterEvent",
    "PlayerBedLeaveEvent",
    "PlayerChatEvent",
    "PlayerCommandEvent",
    "PlayerDeathEvent",
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

class ActorDamageEvent(MobEvent, Cancellable):
    """
    Called when an Actor is damaged.
    """

    @property
    def damage(self) -> float:
        """
        Gets or sets the amount of damage caused by the event
        """

    @damage.setter
    def damage(self, arg1: typing.SupportsFloat) -> None: ...
    @property
    def damage_source(self) -> DamageSource:
        """
        Gets the source of damage.
        """

class ActorDeathEvent(MobEvent):
    """
    Called when an Actor dies.
    """

    @property
    def damage_source(self) -> DamageSource:
        """
        Gets the source of damage which caused the death.
        """

class ActorEvent(Event):
    """
    Represents an Actor-related event.
    """

    @property
    def actor(self) -> Actor:
        """
        Returns the Actor involved in this event
        """

class ActorExplodeEvent(ActorEvent, Cancellable):
    """
    Called when an Actor explodes.
    """

    @property
    def block_list(self) -> list[Block]:
        """
        Gets or sets the list of blocks that would have been removed or were removed from the explosion event.
        """

    @block_list.setter
    def block_list(self, arg1: collections.abc.Sequence[Block]) -> None: ...
    @property
    def location(self) -> Location:
        """
        Returns the location where the explosion happened.
        """

class ActorKnockbackEvent(MobEvent, Cancellable):
    """
    Called when a living entity receives knockback.
    """

    @property
    def knockback(self) -> Vector:
        """
        Gets or sets the knockback that will be applied to the entity.
        """

    @knockback.setter
    def knockback(self, arg1: Vector) -> None: ...
    @property
    def source(self) -> Actor:
        """
        Get the source actor that has caused knockback to the defender, if exists.
        """

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

    @from_location.setter
    def from_location(self, arg1: Location) -> None: ...
    @property
    def to_location(self) -> Location:
        """
        Gets or sets the location that this actor moved to.
        """

    @to_location.setter
    def to_location(self, arg1: Location) -> None: ...

class BlockBreakEvent(BlockEvent, Cancellable):
    """
    Called when a block is broken by a player.
    """

    @property
    def player(self) -> Player:
        """
        Gets the Player that is breaking the block involved in this event.
        """

class BlockCookEvent(BlockEvent, Cancellable):
    """
    Called when an ItemStack is successfully cooked in a block.
    """

    @property
    def result(self) -> ItemStack:
        """
        Gets or sets the resultant ItemStack for this event
        """

    @result.setter
    def result(self, arg1: ItemStack) -> None: ...
    @property
    def source(self) -> ItemStack:
        """
        Gets the smelted ItemStack for this event
        """

class BlockEvent(Event):
    """
    Represents an Block-related event
    """

    @property
    def block(self) -> Block:
        """
        Gets the block involved in this event.
        """

class BlockPistonEvent(BlockEvent, Cancellable):
    """
    Called when a piston block is triggered
    """

    @property
    def direction(self) -> BlockFace:
        """
        Return the direction in which the piston will operate.
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
    """

    @property
    def block_against(self) -> Block:
        """
        Gets the block that this block was placed against
        """

    @property
    def block_placed_state(self) -> BlockState:
        """
        Gets the BlockState for the block which was placed.
        """

    @property
    def block_replaced(self) -> Block:
        """
        Gets the block which was replaced.
        """

    @property
    def player(self) -> Player:
        """
        Gets the player who placed the block involved in this event.
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

    @message.setter
    def message(self, arg1: str | Translatable) -> None: ...
    @property
    def recipients(self) -> set[CommandSender]:
        """
        Gets a set of recipients that this broadcast message will be displayed to.
        """

class Cancellable:
    """
    Represents an event that may be cancelled by a plugin or the server.
    """

    def cancel(self) -> None:
        """
        Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins.
        """

    @property
    def cancelled(self) -> bool:
        """
        Gets or sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins. [Warning] Deprecated: Use is_cancelled instead.
        """

    @cancelled.setter
    def cancelled(self, arg1: bool) -> None: ...
    @property
    def is_cancelled(self) -> bool:
        """
        Gets or sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins.
        """

    @is_cancelled.setter
    def is_cancelled(self, arg1: bool) -> None: ...

class ChunkEvent(DimensionEvent):
    """
    Represents a Chunk related event
    """

    @property
    def chunk(self) -> Chunk:
        """
        Gets the chunk being loaded/unloaded
        """

class ChunkLoadEvent(ChunkEvent):
    """
    Called when a chunk is loaded
    """

class ChunkUnloadEvent(ChunkEvent):
    """
    Called when a chunk is unloaded
    """

class DimensionEvent(LevelEvent):
    """
    Represents events within a dimension
    """

    @property
    def dimension(self) -> Dimension:
        """
        Gets the dimension primarily involved with this event
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

    @property
    def is_asynchronous(self) -> bool:
        """
        Whether the event fires asynchronously.
        """

class EventPriority(enum.IntEnum):
    """
    Listeners are called in following order: LOWEST -> LOW -> NORMAL -> HIGH -> HIGHEST -> MONITOR
    """

    HIGH: typing.ClassVar[EventPriority]  # value = <EventPriority.HIGH: 3>
    HIGHEST: typing.ClassVar[EventPriority]  # value = <EventPriority.HIGHEST: 4>
    LOW: typing.ClassVar[EventPriority]  # value = <EventPriority.LOW: 1>
    LOWEST: typing.ClassVar[EventPriority]  # value = <EventPriority.LOWEST: 0>
    MONITOR: typing.ClassVar[EventPriority]  # value = <EventPriority.MONITOR: 5>
    NORMAL: typing.ClassVar[EventPriority]  # value = <EventPriority.NORMAL: 2>

    @classmethod
    def __new__(cls, value): ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """

class EventResult(enum.Enum):
    ALLOW: typing.ClassVar[EventResult]  # value = <EventResult.ALLOW: 2>
    DEFAULT: typing.ClassVar[EventResult]  # value = <EventResult.DEFAULT: 1>
    DENY: typing.ClassVar[EventResult]  # value = <EventResult.DENY: 0>

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

class MobEvent(Event):
    """
    Represents an Mob-related event.
    """

    @property
    def actor(self) -> Mob:
        """
        Returns the Mob involved in this event
        """

class PacketReceiveEvent(ServerEvent, Cancellable):
    """
    Called when the server receives a packet from a connected client.
    """

    @property
    def address(self) -> SocketAddress:
        """
        Gets the network address to which this packet is being sent.
        """

    @property
    def packet_id(self) -> int:
        """
        Gets the ID of the packet.
        """

    @property
    def payload(self) -> bytes:
        """
        Gets or sets the raw packet data **excluding** the header.
        """

    @payload.setter
    def payload(self, arg1: bytes) -> None: ...
    @property
    def player(self) -> Player:
        """
        Gets the player involved in this event
        NOTE: This may return None if the packet is sent before the player completes the login process.
        """

    @property
    def sub_client_id(self) -> int:
        """
        Gets the SubClient ID (0 = primary client; 1-3 = split-screen clients).
        """

class PacketSendEvent(ServerEvent, Cancellable):
    """
    Called when the server sends a packet to a connected client.
    """

    @property
    def address(self) -> SocketAddress:
        """
        Gets the network address to which this packet is being sent.
        """

    @property
    def packet_id(self) -> int:
        """
        Gets the ID of the packet.
        """

    @property
    def payload(self) -> bytes:
        """
        Gets or sets the raw packet data **excluding** the header.
        """

    @payload.setter
    def payload(self, arg1: bytes) -> None: ...
    @property
    def player(self) -> Player:
        """
        Gets the player involved in this event
        NOTE: This may return None if the packet is sent before the player completes the login process.
        """

    @property
    def sub_client_id(self) -> int:
        """
        Gets the SubClient ID (0 = primary client; 1-3 = split-screen clients).
        """

class PlayerBedEnterEvent(PlayerEvent, Cancellable):
    """
    Called when a player is almost about to enter the bed.
    """

    @property
    def bed(self) -> Block:
        """
        Returns the bed block involved in this event.
        """

class PlayerBedLeaveEvent(PlayerEvent):
    """
    Called when a player is leaving a bed.
    """

    @property
    def bed(self) -> Block:
        """
        Returns the bed block involved in this event.
        """

class PlayerChatEvent(PlayerEvent, Cancellable):
    """
    Called when a player sends a chat message.
    """

    @property
    def format(self) -> str:
        """
        Sets the format to use to display this chat message
        """

    @format.setter
    def format(self, arg1: str) -> None: ...
    @property
    def message(self) -> str:
        """
        Gets or sets the message that the player will send.
        """

    @message.setter
    def message(self, arg1: str) -> None: ...
    @property
    def player(self) -> Player:
        """
        Gets or sets the player that this message will display as
        """

    @player.setter
    def player(self, arg1: Player) -> None: ...
    @property
    def recipients(self) -> list[Player]:
        """
        Gets a set of recipients that this chat message will be displayed to
        """

class PlayerCommandEvent(PlayerEvent, Cancellable):
    """
    Called whenever a player runs a command.
    """

    @property
    def command(self) -> str:
        """
        Gets or sets the command that the player will send.
        """

    @command.setter
    def command(self, arg1: str) -> None: ...

class PlayerDeathEvent(ActorDeathEvent, PlayerEvent):
    """
    Called when a player dies
    """

    @property
    def death_message(self) -> str | Translatable | None:
        """
        Gets or sets the death message that will appear to everyone on the server.
        """

    @death_message.setter
    def death_message(self, arg1: str | Translatable | None) -> None: ...

class PlayerDropItemEvent(PlayerEvent, Cancellable):
    """
    Called when a player drops an item from their inventory
    """

    @property
    def item(self) -> ItemStack:
        """
        Gets the ItemStack dropped by the player
        """

class PlayerEmoteEvent(PlayerEvent, Cancellable):
    """
    Called when a player uses and emote
    """

    @property
    def emote_id(self) -> str:
        """
        Gets the emote piece ID
        """

    @property
    def is_muted(self) -> bool:
        """
        Gets or sets the muted state for the emote.
        """

    @is_muted.setter
    def is_muted(self, arg1: bool) -> None: ...

class PlayerEvent(Event):
    """
    Represents a player related event
    """

    @property
    def player(self) -> Player:
        """
        Returns the player involved in this event.
        """

class PlayerGameModeChangeEvent(PlayerEvent, Cancellable):
    """
    Called when the GameMode of the player is changed.
    """

    @property
    def new_game_mode(self) -> GameMode:
        """
        Gets the GameMode the player is switched to.
        """

class PlayerInteractActorEvent(PlayerEvent, Cancellable):
    """
    Represents an event that is called when a player right-clicks an actor.
    """

    @property
    def actor(self) -> Actor:
        """
        Gets the actor that was right-clicked by the player.
        """

class PlayerInteractEvent(PlayerEvent, Cancellable):
    """
    Represents an event that is called when a player right-clicks a block.
    """

    class Action(enum.Enum):
        LEFT_CLICK_AIR: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.LEFT_CLICK_AIR: 2>
        LEFT_CLICK_BLOCK: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.LEFT_CLICK_BLOCK: 0>
        RIGHT_CLICK_AIR: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.RIGHT_CLICK_AIR: 3>
        RIGHT_CLICK_BLOCK: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.RIGHT_CLICK_BLOCK: 1>

    LEFT_CLICK_AIR: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.LEFT_CLICK_AIR: 2>
    LEFT_CLICK_BLOCK: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.LEFT_CLICK_BLOCK: 0>
    RIGHT_CLICK_AIR: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.RIGHT_CLICK_AIR: 3>
    RIGHT_CLICK_BLOCK: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.RIGHT_CLICK_BLOCK: 1>

    @property
    def action(self) -> PlayerInteractEvent.Action:
        """
        Returns the action type of interaction
        """

    @property
    def block(self) -> Block:
        """
        Returns the clicked block
        """

    @property
    def block_face(self) -> BlockFace:
        """
        Returns the face of the block that was clicked
        """

    @property
    def clicked_position(self) -> Vector | None:
        """
        Gets the exact position on the block the player interacted with.
        """

    @property
    def has_block(self) -> bool:
        """
        Check if this event involved a block
        """

    @property
    def has_item(self) -> bool:
        """
        Check if this event involved an item
        """

    @property
    def item(self) -> ItemStack:
        """
        Returns the item in hand represented by this event
        """

class PlayerItemConsumeEvent(PlayerEvent, Cancellable):
    """
    Called when a player is finishing consuming an item (food, potion, milk bucket).
    """

    @property
    def hand(self) -> EquipmentSlot:
        """
        Get the hand used to consume the item.
        """

    @property
    def item(self) -> ItemStack:
        """
        Gets or sets the item that is being consumed.
        """

class PlayerItemHeldEvent(PlayerEvent, Cancellable):
    """
    Called when a player changes their currently held item.
    """

    @property
    def new_slot(self) -> int:
        """
        Gets the new held slot index
        """

    @property
    def previous_slot(self) -> int:
        """
        Gets the previous held slot index.
        """

class PlayerJoinEvent(PlayerEvent):
    """
    Called when a player joins a server
    """

    @property
    def join_message(self) -> str | Translatable | None:
        """
        Gets or sets the join message to send to all online players.
        """

    @join_message.setter
    def join_message(self, arg1: str | Translatable | None) -> None: ...

class PlayerJumpEvent(PlayerMoveEvent):
    """
    Called when a player jumps.
    """

class PlayerKickEvent(PlayerEvent, Cancellable):
    """
    Called when a player gets kicked from the server
    """

    @property
    def reason(self) -> str:
        """
        Gets or sets the reason why the player is getting kicked
        """

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

    @from_location.setter
    def from_location(self, arg1: Location) -> None: ...
    @property
    def to_location(self) -> Location:
        """
        Gets or sets the location that this player moved to.
        """

    @to_location.setter
    def to_location(self, arg1: Location) -> None: ...

class PlayerPickupItemEvent(PlayerEvent, Cancellable):
    """
    Called when a player picks an item up from the ground.
    """

    @property
    def item(self) -> Item:
        """
        Gets the Item picked up by the entity.
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

    @property
    def skin_change_message(self) -> str | Translatable | None:
        """
        Gets or sets the message to send to all online players for this skin change.
        """

    @skin_change_message.setter
    def skin_change_message(self, arg1: str | Translatable | None) -> None: ...

class PlayerTeleportEvent(PlayerMoveEvent):
    """
    Called when a player is teleported from one location to another.
    """

class PluginDisableEvent(ServerEvent):
    """
    Called when a plugin is disabled.
    """

    @property
    def plugin(self) -> Plugin: ...

class PluginEnableEvent(ServerEvent):
    """
    Called when a plugin is enabled.
    """

    @property
    def plugin(self) -> Plugin: ...

class ScriptMessageEvent(ServerEvent, Cancellable):
    """
    Called when a message is sent by `/scriptevent` command
    """

    @property
    def message(self) -> str:
        """
        Get the message to send.
        """

    @property
    def message_id(self) -> str:
        """
        Get the message id to send.
        """

    @property
    def sender(self) -> CommandSender:
        """
        Gets the command sender who initiated the command.
        """

class ServerCommandEvent(ServerEvent, Cancellable):
    """
    Called when the console runs a command, early in the process.
    """

    @property
    def command(self) -> str:
        """
        Gets or sets the command that the server will execute
        """

    @command.setter
    def command(self, arg1: str) -> None: ...
    @property
    def sender(self) -> CommandSender:
        """
        Get the command sender.
        """

class ServerEvent(Event):
    """
    Represents a server-related event
    """

class ServerListPingEvent(ServerEvent, Cancellable):
    """
    Called when a server ping is coming in.
    """

    @property
    def game_mode(self) -> GameMode:
        """
        Gets or sets the current game mode.
        """

    @game_mode.setter
    def game_mode(self, arg1: GameMode) -> None: ...
    @property
    def level_name(self) -> str:
        """
        Gets or sets the level name.
        """

    @level_name.setter
    def level_name(self, arg1: str) -> None: ...
    @property
    def local_port(self) -> int:
        """
        Get or set the local port of the server.
        """

    @local_port.setter
    def local_port(self, arg1: typing.SupportsInt) -> None: ...
    @property
    def local_port_v6(self) -> int:
        """
        Get or set the local port of the server for IPv6 support
        """

    @local_port_v6.setter
    def local_port_v6(self, arg1: typing.SupportsInt) -> None: ...
    @property
    def max_players(self) -> int:
        """
        Gets or sets the maximum number of players allowed.
        """

    @max_players.setter
    def max_players(self, arg1: typing.SupportsInt) -> None: ...
    @property
    def minecraft_version_network(self) -> str:
        """
        Gets or sets the network version of Minecraft that is supported by this server
        """

    @minecraft_version_network.setter
    def minecraft_version_network(self, arg1: str) -> None: ...
    @property
    def motd(self) -> str:
        """
        Gets or sets the message of the day message.
        """

    @motd.setter
    def motd(self, arg1: str) -> None: ...
    @property
    def network_protocol_version(self) -> int:
        """
        Get the network protocol version of this server
        """

    @property
    def num_players(self) -> int:
        """
        Gets or sets the number of players online.
        """

    @num_players.setter
    def num_players(self, arg1: typing.SupportsInt) -> None: ...
    @property
    def remote_host(self) -> str:
        """
        Get the host the ping is coming from.
        """

    @property
    def remote_port(self) -> int:
        """
        Get the port the ping is coming from.
        """

    @property
    def server_guid(self) -> str:
        """
        Get or set the unique identifier of the server.
        """

    @server_guid.setter
    def server_guid(self, arg1: str) -> None: ...

class ServerLoadEvent(Event):
    """
    Called when either the server startup or reload has completed.
    """

    class LoadType(enum.Enum):
        STARTUP: typing.ClassVar[ServerLoadEvent.LoadType]  # value = <LoadType.STARTUP: 0>

    STARTUP: typing.ClassVar[ServerLoadEvent.LoadType]  # value = <LoadType.STARTUP: 0>

    @property
    def type(self) -> ServerLoadEvent.LoadType: ...

class ThunderChangeEvent(WeatherEvent, Cancellable):
    """
    Called when the thunder state in a world is changing.
    """

    @property
    def to_thunder_state(self) -> bool:
        """
        Gets the state of thunder that the world is being set to
        """

class WeatherChangeEvent(WeatherEvent, Cancellable):
    """
    Called when the weather (rain) state in a world is changing.
    """

    @property
    def to_weather_state(self) -> bool:
        """
        Gets the state of weather that the world is being set to
        """

class WeatherEvent(Event):
    """
    Represents a weather-related event
    """

    @property
    def level(self) -> Level:
        """
        Returns the Level where this event is occurring
        """

def event_handler(func=None, *, priority: EventPriority = EventPriority.NORMAL, ignore_cancelled: bool = False): ...
