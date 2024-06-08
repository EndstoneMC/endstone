from __future__ import annotations
import datetime
import os
import typing
import uuid
__all__ = ['Actor', 'ActorRemoveEvent', 'ActorSpawnEvent', 'BroadcastMessageEvent', 'ColorFormat', 'Command', 'CommandExecutor', 'CommandSender', 'Dimension', 'Event', 'EventPriority', 'GameMode', 'Inventory', 'Level', 'Location', 'Logger', 'Permissible', 'Permission', 'PermissionAttachment', 'PermissionAttachmentInfo', 'PermissionDefault', 'Player', 'PlayerChatEvent', 'PlayerCommandEvent', 'PlayerInventory', 'PlayerJoinEvent', 'PlayerLoginEvent', 'PlayerQuitEvent', 'Plugin', 'PluginCommand', 'PluginDescription', 'PluginDisableEvent', 'PluginEnableEvent', 'PluginLoadOrder', 'PluginLoader', 'PluginManager', 'Position', 'Scheduler', 'Server', 'ServerCommandEvent', 'ServerListPingEvent', 'ServerLoadEvent', 'SocketAddress', 'Task', 'ThunderChangeEvent', 'Vector', 'WeatherChangeEvent']
class Actor(CommandSender):
    """
    Represents a base actor in the level.
    """
    @property
    def is_in_lava(self) -> bool:
        """
        Returns true if the actor is in lava.
        """
    @property
    def is_in_water(self) -> bool:
        """
        Returns true if the actor is in water.
        """
    @property
    def is_on_ground(self) -> bool:
        """
        Returns true if the actor is supported by a block, i.e. on ground.
        """
    @property
    def level(self) -> Level:
        """
        Gets the current Level this actor resides in.
        """
    @property
    def location(self) -> Location:
        """
        Gets the actor's current position.
        """
    @property
    def runtime_id(self) -> int:
        """
        Returns the runtime id for this actor.
        """
    @property
    def velocity(self) -> Vector:
        """
        Gets this actor's current velocity.
        """
class ActorRemoveEvent(Event):
    """
    Called when an Actor is removed.
    """
    @property
    def actor(self) -> Actor:
        """
        Returns the Actor being removed
        """
class ActorSpawnEvent(Event):
    """
    Called when an Actor is spawned into a world.
    """
    @property
    def actor(self) -> Actor:
        """
        Returns the Actor being spawned
        """
class BroadcastMessageEvent(Event):
    """
    Event triggered for server broadcast messages such as from Server.broadcast
    """
    @property
    def message(self) -> str:
        """
        The message to broadcast.
        """
    @message.setter
    def message(self, arg1: str) -> None:
        ...
    @property
    def recipients(self) -> set[CommandSender]:
        """
        Gets a set of recipients that this broadcast message will be displayed to.
        """
class ColorFormat:
    """
    All supported color and format codes.
    """
    AQUA: typing.ClassVar[str] = '§b'
    BLACK: typing.ClassVar[str] = '§0'
    BLUE: typing.ClassVar[str] = '§9'
    BOLD: typing.ClassVar[str] = '§l'
    DARK_AQUA: typing.ClassVar[str] = '§3'
    DARK_BLUE: typing.ClassVar[str] = '§1'
    DARK_GRAY: typing.ClassVar[str] = '§8'
    DARK_GREEN: typing.ClassVar[str] = '§2'
    DARK_PURPLE: typing.ClassVar[str] = '§5'
    DARK_RED: typing.ClassVar[str] = '§4'
    GOLD: typing.ClassVar[str] = '§6'
    GRAY: typing.ClassVar[str] = '§7'
    GREEN: typing.ClassVar[str] = '§a'
    ITALIC: typing.ClassVar[str] = '§o'
    LIGHT_PURPLE: typing.ClassVar[str] = '§d'
    MATERIAL_AMETHYST: typing.ClassVar[str] = '§u'
    MATERIAL_COPPER: typing.ClassVar[str] = '§n'
    MATERIAL_DIAMOND: typing.ClassVar[str] = '§s'
    MATERIAL_EMERALD: typing.ClassVar[str] = '§q'
    MATERIAL_GOLD: typing.ClassVar[str] = '§p'
    MATERIAL_IRON: typing.ClassVar[str] = '§i'
    MATERIAL_LAPIS: typing.ClassVar[str] = '§t'
    MATERIAL_NETHERITE: typing.ClassVar[str] = '§j'
    MATERIAL_QUARTZ: typing.ClassVar[str] = '§h'
    MATERIAL_REDSTONE: typing.ClassVar[str] = '§m'
    MINECOIN_GOLD: typing.ClassVar[str] = '§g'
    OBFUSCATED: typing.ClassVar[str] = '§k'
    RED: typing.ClassVar[str] = '§c'
    RESET: typing.ClassVar[str] = '§r'
    WHITE: typing.ClassVar[str] = '§f'
    YELLOW: typing.ClassVar[str] = '§e'
class Command:
    """
    Represents a Command, which executes various tasks upon user input
    """
    def __init__(self, name: str, description: str | None = None, usages: list[str] | None = None, aliases: list[str] | None = None, permissions: list[str] | None = None, *args, **kwargs) -> None:
        ...
    def execute(self, sender: CommandSender, args: list[str]) -> bool:
        """
        Executes the command, returning its success
        """
    def test_permission(self, target: CommandSender) -> bool:
        """
        Tests the given CommandSender to see if they can perform this command.
        """
    def test_permission_silently(self, target: CommandSender) -> bool:
        """
        Tests the given CommandSender to see if they can perform this command. No error is sent to the sender.
        """
    @property
    def aliases(self) -> list[str]:
        """
        List of aliases of this command
        """
    @aliases.setter
    def aliases(self, arg1: list[str]) -> None:
        ...
    @property
    def description(self) -> str:
        """
        Brief description of this command
        """
    @description.setter
    def description(self, arg1: str) -> None:
        ...
    @property
    def name(self) -> str:
        """
        Name of this command.
        """
    @name.setter
    def name(self, arg1: str) -> None:
        ...
    @property
    def permissions(self) -> list[str]:
        """
        The permissions required by users to be able to perform this command
        """
    @permissions.setter
    def permissions(self, arg1: list[str]) -> None:
        ...
    @property
    def registered(self) -> bool:
        """
        Returns the current registered state of this command
        """
    @property
    def usages(self) -> list[str]:
        """
        List of usages of this command
        """
    @usages.setter
    def usages(self, arg1: list[str]) -> None:
        ...
class CommandExecutor:
    """
    Represents a class which contains a single method for executing commands
    """
    def __init__(self) -> None:
        ...
    def on_command(self, sender: CommandSender, command: Command, args: list[str]) -> bool:
        """
        Executes the given command, returning its success.
        """
class CommandSender(Permissible):
    """
    Represents a command sender.
    """
    def as_player(self) -> Player:
        """
        Cast to a Player instance
        """
    def send_error_message(self, message: str) -> None:
        """
        Sends this sender an error message
        """
    def send_message(self, message: str) -> None:
        """
        Sends this sender a message
        """
    @property
    def name(self) -> str:
        """
        Gets the name of this command sender
        """
    @property
    def server(self) -> Server:
        """
        Returns the server instance that this command is running on
        """
class Dimension:
    """
    Represents a dimension within a Level.
    """
    class Type:
        """
        Represents various dimension types.
        """
        CUSTOM: typing.ClassVar[Dimension.Type]  # value = <Type.CUSTOM: 999>
        NETHER: typing.ClassVar[Dimension.Type]  # value = <Type.NETHER: 1>
        OVERWORLD: typing.ClassVar[Dimension.Type]  # value = <Type.OVERWORLD: 0>
        THE_END: typing.ClassVar[Dimension.Type]  # value = <Type.THE_END: 2>
        __members__: typing.ClassVar[dict[str, Dimension.Type]]  # value = {'OVERWORLD': <Type.OVERWORLD: 0>, 'NETHER': <Type.NETHER: 1>, 'THE_END': <Type.THE_END: 2>, 'CUSTOM': <Type.CUSTOM: 999>}
        def __eq__(self, other: typing.Any) -> bool:
            ...
        def __getstate__(self) -> int:
            ...
        def __hash__(self) -> int:
            ...
        def __index__(self) -> int:
            ...
        def __init__(self, value: int) -> None:
            ...
        def __int__(self) -> int:
            ...
        def __ne__(self, other: typing.Any) -> bool:
            ...
        def __repr__(self) -> str:
            ...
        def __setstate__(self, state: int) -> None:
            ...
        def __str__(self) -> str:
            ...
        @property
        def name(self) -> str:
            ...
        @property
        def value(self) -> int:
            ...
    CUSTOM: typing.ClassVar[Dimension.Type]  # value = <Type.CUSTOM: 999>
    NETHER: typing.ClassVar[Dimension.Type]  # value = <Type.NETHER: 1>
    OVERWORLD: typing.ClassVar[Dimension.Type]  # value = <Type.OVERWORLD: 0>
    THE_END: typing.ClassVar[Dimension.Type]  # value = <Type.THE_END: 2>
    @property
    def level(self) -> Level:
        """
        Gets the level to which this dimension belongs
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
class Event:
    """
    Represents an event.
    """
    @property
    def asynchronous(self) -> bool:
        """
        Whether the event fires asynchronously.
        """
    @property
    def cancellable(self) -> bool:
        """
        Whether the event can be cancelled by a plugin or the server.
        """
    @property
    def cancelled(self) -> bool:
        """
        The cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins
        """
    @cancelled.setter
    def cancelled(self, arg1: bool) -> None:
        ...
    @property
    def event_name(self) -> str:
        """
        Gets a user-friendly identifier for this event.
        """
class EventPriority:
    """
    Listeners are called in following order: LOWEST -> LOW -> NORMAL -> HIGH -> HIGHEST -> MONITOR
    """
    HIGH: typing.ClassVar[EventPriority]  # value = <EventPriority.HIGH: 3>
    HIGHEST: typing.ClassVar[EventPriority]  # value = <EventPriority.HIGHEST: 4>
    LOW: typing.ClassVar[EventPriority]  # value = <EventPriority.LOW: 1>
    LOWEST: typing.ClassVar[EventPriority]  # value = <EventPriority.LOWEST: 0>
    MONITOR: typing.ClassVar[EventPriority]  # value = <EventPriority.MONITOR: 5>
    NORMAL: typing.ClassVar[EventPriority]  # value = <EventPriority.NORMAL: 2>
    __members__: typing.ClassVar[dict[str, EventPriority]]  # value = {'LOWEST': <EventPriority.LOWEST: 0>, 'LOW': <EventPriority.LOW: 1>, 'NORMAL': <EventPriority.NORMAL: 2>, 'HIGH': <EventPriority.HIGH: 3>, 'HIGHEST': <EventPriority.HIGHEST: 4>, 'MONITOR': <EventPriority.MONITOR: 5>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class GameMode:
    """
    Represents the various type of game modes that Players may have.
    """
    ADVENTURE: typing.ClassVar[GameMode]  # value = <GameMode.ADVENTURE: 2>
    CREATIVE: typing.ClassVar[GameMode]  # value = <GameMode.CREATIVE: 1>
    SPECTATOR: typing.ClassVar[GameMode]  # value = <GameMode.SPECTATOR: 3>
    SURVIVAL: typing.ClassVar[GameMode]  # value = <GameMode.SURVIVAL: 0>
    __members__: typing.ClassVar[dict[str, GameMode]]  # value = {'SURVIVAL': <GameMode.SURVIVAL: 0>, 'CREATIVE': <GameMode.CREATIVE: 1>, 'ADVENTURE': <GameMode.ADVENTURE: 2>, 'SPECTATOR': <GameMode.SPECTATOR: 3>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Inventory:
    """
    Interface to the various inventories.
    """
    @property
    def max_stack_size(self) -> int:
        """
        Returns the maximum stack size for an ItemStack in this inventory.
        """
    @property
    def size(self) -> int:
        """
        Returns the size of the inventory
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
    def time(self) -> int:
        """
        Gets and sets the relative in-game time on the server
        """
    @time.setter
    def time(self, arg1: int) -> None:
        ...
class Location(Position):
    """
    Represents a 3-dimensional location in a dimension within a level.
    """
    def __init__(self, dimension: Dimension, x: float, y: float, z: float, pitch: float = 0.0, yaw: float = 0.0) -> None:
        ...
    @property
    def pitch(self) -> float:
        """
        The pitch of this location, measured in degrees.
        """
    @pitch.setter
    def pitch(self, arg1: float) -> None:
        ...
    @property
    def yaw(self) -> float:
        """
        The yaw of this location, measured in degrees.
        """
    @yaw.setter
    def yaw(self, arg1: float) -> None:
        ...
class Logger:
    """
    Logger class which can format and output varies levels of logs.
    """
    class Level:
        """
        Specifies the log level.
        """
        CRITICAL: typing.ClassVar[Logger.Level]  # value = <Level.CRITICAL: 5>
        DEBUG: typing.ClassVar[Logger.Level]  # value = <Level.DEBUG: 1>
        ERROR: typing.ClassVar[Logger.Level]  # value = <Level.ERROR: 4>
        INFO: typing.ClassVar[Logger.Level]  # value = <Level.INFO: 2>
        TRACE: typing.ClassVar[Logger.Level]  # value = <Level.TRACE: 0>
        WARNING: typing.ClassVar[Logger.Level]  # value = <Level.WARNING: 3>
        __members__: typing.ClassVar[dict[str, Logger.Level]]  # value = {'TRACE': <Level.TRACE: 0>, 'DEBUG': <Level.DEBUG: 1>, 'INFO': <Level.INFO: 2>, 'WARNING': <Level.WARNING: 3>, 'ERROR': <Level.ERROR: 4>, 'CRITICAL': <Level.CRITICAL: 5>}
        def __eq__(self, other: typing.Any) -> bool:
            ...
        def __getstate__(self) -> int:
            ...
        def __hash__(self) -> int:
            ...
        def __index__(self) -> int:
            ...
        def __init__(self, value: int) -> None:
            ...
        def __int__(self) -> int:
            ...
        def __ne__(self, other: typing.Any) -> bool:
            ...
        def __repr__(self) -> str:
            ...
        def __setstate__(self, state: int) -> None:
            ...
        def __str__(self) -> str:
            ...
        @property
        def name(self) -> str:
            ...
        @property
        def value(self) -> int:
            ...
    CRITICAL: typing.ClassVar[Logger.Level]  # value = <Level.CRITICAL: 5>
    DEBUG: typing.ClassVar[Logger.Level]  # value = <Level.DEBUG: 1>
    ERROR: typing.ClassVar[Logger.Level]  # value = <Level.ERROR: 4>
    INFO: typing.ClassVar[Logger.Level]  # value = <Level.INFO: 2>
    TRACE: typing.ClassVar[Logger.Level]  # value = <Level.TRACE: 0>
    WARNING: typing.ClassVar[Logger.Level]  # value = <Level.WARNING: 3>
    def critical(self, message: str) -> None:
        """
        Log a message at the CRITICAL level.
        """
    def debug(self, message: str) -> None:
        """
        Log a message at the DEBUG level.
        """
    def error(self, message: str) -> None:
        """
        Log a message at the ERROR level.
        """
    def info(self, message: str) -> None:
        """
        Log a message at the INFO level.
        """
    def is_enabled_for(self, level: Logger.Level) -> bool:
        """
        Check if the Logger instance is enabled for the given log Level.
        """
    def set_level(self, level: Logger.Level) -> None:
        """
        Set the logging level for this Logger instance.
        """
    def trace(self, message: str) -> None:
        """
        Log a message at the TRACE level.
        """
    def warning(self, message: str) -> None:
        """
        Log a message at the WARNING level.
        """
    @property
    def name(self) -> str:
        """
        Get the name of this Logger instance.
        """
class Permissible:
    """
    Represents an object that may become a server operator and can be assigned permissions.
    """
    @typing.overload
    def add_attachment(self, plugin: Plugin, name: str, value: bool) -> PermissionAttachment:
        """
        Adds a new PermissionAttachment.
        """
    @typing.overload
    def add_attachment(self, plugin: Plugin) -> PermissionAttachment:
        """
        Adds a new PermissionAttachment.
        """
    @typing.overload
    def has_permission(self, name: str) -> bool:
        """
        Checks if a permissions is available by name.
        """
    @typing.overload
    def has_permission(self, perm: Permission) -> bool:
        """
        Checks if a permissions is available by permission.
        """
    @typing.overload
    def is_permission_set(self, name: str) -> bool:
        """
        Checks if a permissions is set by name.
        """
    @typing.overload
    def is_permission_set(self, perm: Permission) -> bool:
        """
        Checks if a permissions is set by permission.
        """
    def recalculate_permissions(self) -> None:
        """
        Recalculates the permissions.
        """
    def remove_attachment(self, attachment: PermissionAttachment) -> bool:
        """
        Removes a given PermissionAttachment.
        """
    @property
    def effective_permissions(self) -> set[PermissionAttachmentInfo]:
        """
        Gets effective permissions.
        """
    @property
    def op(self) -> bool:
        """
        The operator status of this object
        """
    @op.setter
    def op(self, arg1: bool) -> None:
        ...
class Permission:
    """
    Represents a unique permission that may be attached to a Permissible
    """
    def __init__(self, name: str, description: str | None = None, default: PermissionDefault | None = None, children: dict[str, bool] | None = None, *args, **kwargs) -> None:
        ...
    @typing.overload
    def add_parent(self, name: str, value: bool) -> Permission:
        """
        Adds this permission to the specified parent permission.
        """
    @typing.overload
    def add_parent(self, perm: Permission, value: bool) -> None:
        """
        Adds this permission to the specified parent permission.
        """
    def recalculate_permissibles(self) -> None:
        """
        Recalculates all Permissibles that contain this permission.
        """
    @property
    def children(self) -> dict[str, bool]:
        """
        Gets the children of this permission.
        """
    @property
    def default(self) -> PermissionDefault:
        """
        The default value of this permission.
        """
    @default.setter
    def default(self, arg1: PermissionDefault) -> None:
        ...
    @property
    def description(self) -> str:
        """
        The brief description of this permission
        """
    @description.setter
    def description(self, arg1: str) -> None:
        ...
    @property
    def name(self) -> str:
        """
        Gets the unique fully qualified name of this Permission.
        """
    @property
    def permissibles(self) -> set[Permissible]:
        """
        Gets a set containing every Permissible that has this permission.
        """
class PermissionAttachment:
    """
    Holds information about a permission attachment on a Permissible object
    """
    def __init__(self, plugin: Plugin, permissible: Permissible) -> None:
        ...
    def remove(self) -> bool:
        """
        Removes this attachment from its registered Permissible.
        """
    @typing.overload
    def set_permission(self, name: str, value: bool) -> None:
        """
        Sets a permission to the given value, by its fully qualified name.
        """
    @typing.overload
    def set_permission(self, perm: Permission, value: bool) -> None:
        """
        Sets a permission to the given value.
        """
    @typing.overload
    def unset_permission(self, name: str) -> None:
        """
        Removes the specified permission from this attachment by name.
        """
    @typing.overload
    def unset_permission(self, perm: Permission) -> None:
        """
        Removes the specified permission from this attachment.
        """
    @property
    def permissible(self) -> Permissible:
        """
        Gets the Permissible that this is attached to.
        """
    @property
    def permissions(self) -> dict[str, bool]:
        """
        Gets a copy of all set permissions and values contained within this attachment.
        """
    @property
    def plugin(self) -> Plugin:
        """
        Gets the plugin responsible for this attachment.
        """
    @property
    def removal_callback(self) -> typing.Callable[[PermissionAttachment], None]:
        """
        The callback to be called when this attachment is removed.
        """
    @removal_callback.setter
    def removal_callback(self, arg1: typing.Callable[[PermissionAttachment], None]) -> None:
        ...
class PermissionAttachmentInfo:
    """
    Holds information on a permission and which PermissionAttachment provides it
    """
    def __init__(self, permissible: Permissible, permission: str, attachment: PermissionAttachment, value: bool) -> None:
        ...
    @property
    def attachment(self) -> PermissionAttachment:
        """
        Gets the attachment providing this permission.
        """
    @property
    def permissible(self) -> Permissible:
        """
        Get the permissible this is attached to
        """
    @property
    def permission(self) -> str:
        """
        Gets the permission being set
        """
    @property
    def value(self) -> bool:
        """
        Gets the value of this permission
        """
class PermissionDefault:
    """
    Represents the possible default values for permissions
    """
    FALSE: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.FALSE: 1>
    NOT_OP: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.NOT_OP: 3>
    NOT_OPERATOR: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.NOT_OP: 3>
    OP: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.OP: 2>
    OPERATOR: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.OP: 2>
    TRUE: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.TRUE: 0>
    __members__: typing.ClassVar[dict[str, PermissionDefault]]  # value = {'TRUE': <PermissionDefault.TRUE: 0>, 'FALSE': <PermissionDefault.FALSE: 1>, 'OP': <PermissionDefault.OP: 2>, 'OPERATOR': <PermissionDefault.OP: 2>, 'NOT_OP': <PermissionDefault.NOT_OP: 3>, 'NOT_OPERATOR': <PermissionDefault.NOT_OP: 3>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Player(Actor):
    """
    Represents a player.
    """
    def kick(self, message: str) -> None:
        """
        Kicks player with custom kick message.
        """
    def perform_command(self, command: str) -> bool:
        """
        Makes the player perform the given command.
        """
    def send_popup(self, message: str) -> None:
        """
        Sends this player a popup message
        """
    def send_raw_message(self, message: str) -> None:
        """
        Sends this player a raw message
        """
    def send_tip(self, message: str) -> None:
        """
        Sends this player a tip message
        """
    def update_commands(self) -> None:
        """
        Send the list of commands to the client.
        """
    @property
    def address(self) -> SocketAddress:
        """
        Gets the socket address of this player
        """
    @property
    def game_mode(self) -> GameMode:
        """
        The player's current game mode.
        """
    @game_mode.setter
    def game_mode(self, arg1: GameMode) -> None:
        ...
    @property
    def inventory(self) -> PlayerInventory:
        """
        Get the player's inventory.
        """
    @property
    def is_flying(self) -> bool:
        """
        Checks to see if this player is currently flying or not.
        """
    @property
    def ping(self) -> datetime.timedelta:
        """
        Gets the player's average ping.
        """
    @property
    def unique_id(self) -> uuid.UUID:
        """
        Returns the UUID of this player
        """
class PlayerChatEvent(Event):
    """
    Called when a player sends a chat message.
    """
    @property
    def message(self) -> str:
        """
        The message that the player will send.
        """
    @message.setter
    def message(self, arg1: str) -> None:
        ...
    @property
    def player(self) -> Player:
        """
        Returns the Player who sends the message
        """
class PlayerCommandEvent(Event):
    """
    Called whenever a player runs a command.
    """
    @property
    def command(self) -> str:
        """
        The command that the player will send.
        """
    @command.setter
    def command(self, arg1: str) -> None:
        ...
    @property
    def player(self) -> Player:
        """
        Returns the Player who sends the command
        """
class PlayerInventory(Inventory):
    """
    Interface to the inventory of a Player, including the four armor slots and any extra slots.
    """
class PlayerJoinEvent(Event):
    """
    Called when a player joins a server
    """
    @property
    def player(self) -> Player:
        """
        Returns the Player who joins the server
        """
class PlayerLoginEvent(Event):
    """
    Called when a player attempts to login in.
    """
    @property
    def kick_message(self) -> str:
        """
        The kick message to display if event is cancelled
        """
    @kick_message.setter
    def kick_message(self, arg1: str) -> None:
        ...
    @property
    def player(self) -> Player:
        """
        Returns the Player who attempts to login in.
        """
class PlayerQuitEvent(Event):
    """
    Called when a player leaves a server.
    """
    @property
    def player(self) -> Player:
        """
        Returns the Player who leaves the server
        """
class Plugin(CommandExecutor):
    """
    Represents a Plugin
    """
    def __init__(self) -> None:
        ...
    def _get_description(self) -> PluginDescription:
        ...
    def get_command(self, name: str) -> PluginCommand:
        """
        Gets the command with the given name, specific to this plugin.
        """
    def on_disable(self) -> None:
        """
        Called when this plugin is disabled
        """
    def on_enable(self) -> None:
        """
        Called when this plugin is enabled
        """
    def on_load(self) -> None:
        """
        Called after a plugin is loaded but before it has been enabled.
        """
    @property
    def data_folder(self) -> os.PathLike:
        """
        Returns the folder that the plugin data's files are located in.
        """
    @property
    def enabled(self) -> bool:
        """
        Returns a value indicating whether this plugin is currently enabled
        """
    @property
    def logger(self) -> Logger:
        """
        Returns the plugin logger associated with this server's logger.
        """
    @property
    def name(self) -> str:
        """
        Returns the name of the plugin.
        """
    @property
    def plugin_loader(self) -> PluginLoader:
        """
        Gets the associated PluginLoader responsible for this plugin
        """
    @property
    def server(self) -> Server:
        """
        Returns the Server instance currently running this plugin
        """
class PluginCommand(Command):
    """
    Represents a Command belonging to a Plugin
    """
    @property
    def executor(self) -> CommandExecutor:
        """
        The CommandExecutor to run when parsing this command
        """
    @executor.setter
    def executor(self, arg1: CommandExecutor) -> None:
        ...
    @property
    def plugin(self) -> Plugin:
        """
        The owner of this PluginCommand
        """
class PluginDescription:
    """
    Represents the basic information about a plugin that the plugin loader needs to know.
    """
    def __init__(self, name: str, version: str, description: str | None = None, load: PluginLoadOrder | None = None, authors: list[str] | None = None, contributors: list[str] | None = None, website: str | None = None, prefix: str | None = None, provides: list[str] | None = None, depend: list[str] | None = None, soft_depend: list[str] | None = None, load_before: list[str] | None = None, default_permission: PermissionDefault | None = None, commands: list[Command] | None = None, permissions: list[Permission] | None = None, *args, **kwargs) -> None:
        ...
    @property
    def api_version(self) -> str:
        """
        Gives the API version which this plugin is designed to support.
        """
    @property
    def authors(self) -> list[str]:
        """
        Gives the list of authors for the plugin.
        """
    @property
    def commands(self) -> list[Command]:
        """
        Gives the list of commands the plugin will register at runtime.
        """
    @property
    def contributors(self) -> list[str]:
        """
        Gives the list of contributors for the plugin.
        """
    @property
    def default_permission(self) -> PermissionDefault:
        """
        Gives the default value of permissions registered for the plugin.
        """
    @property
    def depend(self) -> list[str]:
        """
        Gives a list of other plugins that the plugin requires.
        """
    @property
    def description(self) -> str:
        """
        Gives a human-friendly description of the functionality the plugin provides.
        """
    @property
    def full_name(self) -> str:
        """
        Returns the name of a plugin, including the version.
        """
    @property
    def load(self) -> PluginLoadOrder:
        """
        Gives the phase of server startup that the plugin should be loaded.
        """
    @property
    def load_before(self) -> list[str]:
        """
        Gets the list of plugins that should consider this plugin a soft-dependency.
        """
    @property
    def name(self) -> str:
        """
        Gives the name of the plugin. This name is a unique identifier for plugins.
        """
    @property
    def permissions(self) -> list[Permission]:
        """
        Gives the list of permissions the plugin will register at runtime, immediately proceeding enabling.
        """
    @property
    def prefix(self) -> str:
        """
        Gives the token to prefix plugin-specific logging messages with.
        """
    @property
    def provides(self) -> list[str]:
        """
        Gives the list of other plugin APIs which this plugin provides. These are usable for other plugins to depend on.
        """
    @property
    def soft_depend(self) -> list[str]:
        """
        Gives a list of other plugins that the plugin requires for full functionality.
        """
    @property
    def version(self) -> str:
        """
        Gives the version of the plugin.
        """
    @property
    def website(self) -> str:
        """
        Gives the plugin's or plugin's author's website.
        """
class PluginDisableEvent(Event):
    """
    Called when a plugin is disabled.
    """
    @property
    def plugin(self) -> Plugin:
        ...
class PluginEnableEvent(Event):
    """
    Called when a plugin is enabled.
    """
    @property
    def plugin(self) -> Plugin:
        ...
class PluginLoadOrder:
    """
    Represents the order in which a plugin should be initialized and enabled.
    """
    POSTWORLD: typing.ClassVar[PluginLoadOrder]  # value = <PluginLoadOrder.POSTWORLD: 1>
    STARTUP: typing.ClassVar[PluginLoadOrder]  # value = <PluginLoadOrder.STARTUP: 0>
    __members__: typing.ClassVar[dict[str, PluginLoadOrder]]  # value = {'STARTUP': <PluginLoadOrder.STARTUP: 0>, 'POSTWORLD': <PluginLoadOrder.POSTWORLD: 1>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class PluginLoader:
    """
    Represents a plugin loader, which handles direct access to specific types of plugins
    """
    def __init__(self, server: Server) -> None:
        ...
    def disable_plugin(self, plugin: Plugin) -> None:
        """
        Disables the specified plugin
        """
    def enable_plugin(self, plugin: Plugin) -> None:
        """
        Enables the specified plugin
        """
    def load_plugins(self, directory: str) -> list[Plugin]:
        """
        Loads the plugin contained within the specified directory
        """
    @property
    def server(self) -> Server:
        """
        Retrieves the Server object associated with the PluginLoader.
        """
class PluginManager:
    """
    Represents a plugin manager that handles all plugins from the Server
    """
    def call_event(self, event: Event) -> None:
        """
        Calls an event which will be passed to plugins.
        """
    def clear_plugins(self) -> None:
        """
        Disables and removes all plugins
        """
    def disable_plugin(self, plugin: Plugin) -> None:
        """
        Disables the specified plugin
        """
    def disable_plugins(self) -> None:
        """
        Disables all the loaded plugins
        """
    def enable_plugin(self, plugin: Plugin) -> None:
        """
        Enables the specified plugin
        """
    def enable_plugins(self) -> None:
        """
        Enable all the loaded plugins
        """
    def get_default_perm_subscriptions(self, op: bool) -> set[Permissible]:
        """
        Gets a set containing all subscribed Permissibles to the given default list, by op status.
        """
    def get_default_permissions(self, op: bool) -> set[Permission]:
        """
        Gets the default permissions for the given op status.
        """
    def get_permission(self, name: str) -> Permission:
        """
        Gets a Permission from its fully qualified name.
        """
    def get_permission_subscriptions(self, permission: str) -> set[Permissible]:
        """
        Gets a set containing all subscribed Permissibles to the given permission.
        """
    def get_plugin(self, name: str) -> Plugin:
        """
        Checks if the given plugin is loaded and returns it when applicable.
        """
    @typing.overload
    def is_plugin_enabled(self, plugin: str) -> bool:
        """
        Checks if the given plugin is enabled or not
        """
    @typing.overload
    def is_plugin_enabled(self, plugin: Plugin) -> bool:
        """
        Checks if the given plugin is enabled or not
        """
    def load_plugins(self, directory: str) -> list[Plugin]:
        """
        Loads the plugin contained within the specified directory
        """
    def recalculate_permission_defaults(self, perm: Permission) -> None:
        """
        Recalculates the defaults for the given Permission.
        """
    def register_event(self, name: str, executor: typing.Callable[[Event], None], priority: EventPriority, plugin: Plugin, ignore_cancelled: bool) -> None:
        """
        Registers the given event
        """
    @typing.overload
    def remove_permission(self, perm: Permission) -> None:
        """
        Removes a Permission registration from this plugin manager by permission object.
        """
    @typing.overload
    def remove_permission(self, name: str) -> None:
        """
        Removes a Permission registration from this plugin manager by name.
        """
    def subscribe_to_default_perms(self, op: bool, permissible: Permissible) -> None:
        """
        Subscribes to the given Default permissions by operator status.
        """
    def subscribe_to_permission(self, permission: str, permissible: Permissible) -> None:
        """
        Subscribes the given Permissible for information about the requested Permission.
        """
    def unsubscribe_from_default_perms(self, op: bool, permissible: Permissible) -> None:
        """
        Unsubscribes from the given Default permissions by operator status.
        """
    def unsubscribe_from_permission(self, permission: str, permissible: Permissible) -> None:
        """
        Unsubscribes the given Permissible for information about the requested Permission.
        """
    @property
    def permissions(self) -> set[Permission]:
        """
        Gets a set of all registered permissions.
        """
    @property
    def plugins(self) -> list[Plugin]:
        """
        Gets a list of all currently loaded plugins
        """
class Position(Vector):
    """
    Represents a 3-dimensional position in a dimension within a level.
    """
    def __init__(self, dimension: Dimension, x: float, y: float, z: float) -> None:
        ...
    @property
    def dimension(self) -> Dimension:
        """
        The Dimension that contains this position
        """
    @dimension.setter
    def dimension(self, arg1: Dimension) -> None:
        ...
class Scheduler:
    """
    Represents a scheduler that executes various tasks
    """
    def cancel_task(self, id: int) -> None:
        """
        Removes task from scheduler.
        """
    def cancel_tasks(self, plugin: Plugin) -> None:
        """
        Removes all tasks associated with a particular plugin from the scheduler.
        """
    def get_pending_tasks(self) -> list[Task]:
        """
        Returns a vector of all pending tasks.
        """
    def is_queued(self, id: int) -> bool:
        """
        Check if the task queued to be run later.
        """
    def is_running(self, id: int) -> bool:
        """
        Check if the task currently running.
        """
    def run_task(self, plugin: Plugin, task: typing.Callable[[], None]) -> Task:
        """
        Returns a task that will be executed synchronously on the next server tick.
        """
    def run_task_later(self, plugin: Plugin, task: typing.Callable[[], None], delay: int) -> Task:
        """
        Returns a task that will be executed synchronously after the specified number of server ticks.
        """
    def run_task_timer(self, plugin: Plugin, task: typing.Callable[[], None], delay: int, period: int) -> Task:
        """
        Returns a task that will be executed repeatedly until cancelled, starting after the specified number of server ticks.
        """
class Server:
    """
    Represents a server implementation.
    """
    def broadcast(self, message: str, permission: str) -> None:
        """
        Broadcasts the specified message to every user with the given permission name.
        """
    def broadcast_message(self, message: str) -> None:
        """
        Broadcasts the specified message to every user with permission endstone.broadcast.user
        """
    def dispatch_command(self, sender: CommandSender, command: str) -> bool:
        """
        Dispatches a command on this server, and executes it if found.
        """
    def get_level(self, name: str) -> Level:
        """
        Gets the level with the given name.
        """
    @typing.overload
    def get_player(self, name: str) -> Player:
        """
        Gets the player with the exact given name, case insensitive.
        """
    @typing.overload
    def get_player(self, unique_id: uuid.UUID) -> Player:
        """
        Gets the player with the given UUID.
        """
    def get_plugin_command(self, name: str) -> PluginCommand:
        """
        Gets a PluginCommand with the given name or alias.
        """
    @property
    def command_sender(self) -> CommandSender:
        """
        Gets a CommandSender for this server.
        """
    @property
    def levels(self) -> list[Level]:
        """
        Gets a list of all levels on this server.
        """
    @property
    def logger(self) -> Logger:
        """
        Returns the primary logger associated with this server instance.
        """
    @property
    def max_players(self) -> int:
        """
        The maximum amount of players which can login to this server.
        """
    @max_players.setter
    def max_players(self, arg1: int) -> None:
        ...
    @property
    def minecraft_version(self) -> str:
        """
        Gets the Minecraft version that this server is running.
        """
    @property
    def name(self) -> str:
        """
        Gets the name of this server implementation.
        """
    @property
    def online_players(self) -> list[Player]:
        """
        Gets a list of all currently online players.
        """
    @property
    def plugin_manager(self) -> PluginManager:
        """
        Gets the plugin manager for interfacing with plugins.
        """
    @property
    def scheduler(self) -> Scheduler:
        """
        Gets the scheduler for managing scheduled events.
        """
    @property
    def version(self) -> str:
        """
        Gets the version of this server implementation.
        """
class ServerCommandEvent(Event):
    """
    Called when the console runs a command, early in the process.
    """
    @property
    def command(self) -> str:
        """
        The command that the server will execute
        """
    @command.setter
    def command(self, arg1: str) -> None:
        ...
    @property
    def sender(self) -> CommandSender:
        """
        Get the command sender.
        """
class ServerListPingEvent(Event):
    """
    Called when a server ping is coming in.
    """
    @property
    def game_mode(self) -> GameMode:
        """
        The current game mode.
        """
    @game_mode.setter
    def game_mode(self, arg1: GameMode) -> None:
        ...
    @property
    def level_name(self) -> str:
        """
        The level name.
        """
    @level_name.setter
    def level_name(self, arg1: str) -> None:
        ...
    @property
    def local_port(self) -> int:
        """
        Get the local port of the server.
        """
    @property
    def local_port_v6(self) -> int:
        """
        Get the local port of the server for IPv6 support
        """
    @property
    def max_players(self) -> int:
        """
        The maximum number of players allowed.
        """
    @max_players.setter
    def max_players(self, arg1: int) -> None:
        ...
    @property
    def minecraft_version_network(self) -> str:
        """
        The network version of Minecraft that is supported by this server
        """
    @minecraft_version_network.setter
    def minecraft_version_network(self, arg1: str) -> None:
        ...
    @property
    def motd(self) -> str:
        """
        The message of the day message.
        """
    @motd.setter
    def motd(self, arg1: str) -> None:
        ...
    @property
    def network_protocol_version(self) -> int:
        """
        Get the network protocol version of this server
        """
    @property
    def num_players(self) -> int:
        """
        The number of players online.
        """
    @num_players.setter
    def num_players(self, arg1: int) -> None:
        ...
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
        Get the unique identifier of the server.
        """
class ServerLoadEvent(Event):
    """
    Called when either the server startup or reload has completed.
    """
    class LoadType:
        STARTUP: typing.ClassVar[ServerLoadEvent.LoadType]  # value = <LoadType.STARTUP: 0>
        __members__: typing.ClassVar[dict[str, ServerLoadEvent.LoadType]]  # value = {'STARTUP': <LoadType.STARTUP: 0>}
        def __eq__(self, other: typing.Any) -> bool:
            ...
        def __getstate__(self) -> int:
            ...
        def __hash__(self) -> int:
            ...
        def __index__(self) -> int:
            ...
        def __init__(self, value: int) -> None:
            ...
        def __int__(self) -> int:
            ...
        def __ne__(self, other: typing.Any) -> bool:
            ...
        def __repr__(self) -> str:
            ...
        def __setstate__(self, state: int) -> None:
            ...
        def __str__(self) -> str:
            ...
        @property
        def name(self) -> str:
            ...
        @property
        def value(self) -> int:
            ...
    STARTUP: typing.ClassVar[ServerLoadEvent.LoadType]  # value = <LoadType.STARTUP: 0>
    @property
    def type(self) -> ServerLoadEvent.LoadType:
        ...
class SocketAddress:
    """
    Represents an IP Socket Address (hostname + port number).
    """
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, hostname: str, port: int) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def __str__(self) -> str:
        ...
    @property
    def hostname(self) -> str:
        """
        Gets the hostname.
        """
    @property
    def port(self) -> int:
        """
        Gets the port number.
        """
class Task:
    """
    Represents a task being executed by the scheduler
    """
    def cancel(self) -> None:
        """
        Attempts to cancel this task.
        """
    @property
    def is_cancelled(self) -> bool:
        """
        Returns true if the task has been cancelled.
        """
    @property
    def is_sync(self) -> bool:
        """
        Returns true if the task is run by server thread.
        """
    @property
    def owner(self) -> Plugin:
        """
        Returns the Plugin that owns the task.
        """
    @property
    def task_id(self) -> int:
        """
        Returns the task id.
        """
class ThunderChangeEvent(Event):
    """
    Called when the thunder state in a world is changing.
    """
    @property
    def level(self) -> Level:
        """
        Returns the Level where this event is occurring
        """
    @property
    def to_thunder_state(self) -> bool:
        """
        Gets the state of thunder that the world is being set to
        """
class Vector:
    """
    Represents a 3-dimensional vector.
    """
    @typing.overload
    def __add__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __add__(self, arg0: float) -> Vector:
        ...
    def __iadd__(self, arg0: Vector) -> Vector:
        ...
    def __imul__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, x: float, y: float, z: float) -> None:
        ...
    def __isub__(self, arg0: Vector) -> Vector:
        ...
    def __itruediv__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __mul__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __mul__(self, arg0: float) -> Vector:
        ...
    def __radd__(self, arg0: float) -> Vector:
        ...
    def __repr__(self) -> str:
        ...
    def __rmul__(self, arg0: float) -> Vector:
        ...
    def __rsub__(self, arg0: float) -> Vector:
        ...
    def __rtruediv__(self, arg0: float) -> Vector:
        ...
    def __str__(self) -> str:
        ...
    @typing.overload
    def __sub__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __sub__(self, arg0: float) -> Vector:
        ...
    @typing.overload
    def __truediv__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __truediv__(self, arg0: float) -> Vector:
        ...
    def distance(self, other: Vector) -> float:
        """
        The distance between this Vector and another
        """
    def distance_squared(self, other: Vector) -> float:
        """
        The squared distance between this Vector and another
        """
    @property
    def length(self) -> float:
        """
        The magnitude of the Vector
        """
    @property
    def length_squared(self) -> float:
        """
        The squared magnitude of the Vector
        """
    @property
    def x(self) -> float:
        """
        The X component of the vector
        """
    @x.setter
    def x(self, arg1: float) -> None:
        ...
    @property
    def y(self) -> float:
        """
        The Y component of the vector
        """
    @y.setter
    def y(self, arg1: float) -> None:
        ...
    @property
    def z(self) -> float:
        """
        The Z component of the vector
        """
    @z.setter
    def z(self, arg1: float) -> None:
        ...
class WeatherChangeEvent(Event):
    """
    Called when the weather (rain) state in a world is changing.
    """
    @property
    def level(self) -> Level:
        """
        Returns the Level where this event is occurring
        """
    @property
    def to_weather_state(self) -> bool:
        """
        Gets the state of weather that the world is being set to
        """
