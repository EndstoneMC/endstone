from __future__ import annotations
import typing
import uuid
__all__ = ['Actor', 'ActorRemoveEvent', 'ActorSpawnEvent', 'ColorFormat', 'Command', 'CommandExecutor', 'CommandSender', 'Event', 'EventPriority', 'GameMode', 'HumanActor', 'Level', 'Logger', 'Permissible', 'Permission', 'PermissionAttachment', 'PermissionAttachmentInfo', 'PermissionDefault', 'Player', 'PlayerChatEvent', 'PlayerJoinEvent', 'PlayerQuitEvent', 'Plugin', 'PluginCommand', 'PluginDescription', 'PluginDisableEvent', 'PluginEnableEvent', 'PluginLoadOrder', 'PluginLoader', 'PluginManager', 'Server', 'ServerCommandEvent', 'ServerListPingEvent', 'ServerLoadEvent', 'ThunderChangeEvent', 'WeatherChangeEvent']
class Actor(CommandSender):
    pass
class ActorRemoveEvent(Event):
    @property
    def actor(self) -> Actor:
        """
        Returns the Actor being removed
        """
class ActorSpawnEvent(Event):
    @property
    def actor(self) -> Actor:
        """
        Returns the Actor being spawned
        """
class ColorFormat:
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
    def __init__(self) -> None:
        ...
    def on_command(self, sender: CommandSender, command: Command, args: list[str]) -> bool:
        """
        Executes the given command, returning its success.
        """
class CommandSender(Permissible):
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
class Event:
    class Result:
        """
        Members:
        
          DENY
        
          DEFAULT
        
          ALLOW
        """
        ALLOW: typing.ClassVar[Event.Result]  # value = <Result.ALLOW: 2>
        DEFAULT: typing.ClassVar[Event.Result]  # value = <Result.DEFAULT: 1>
        DENY: typing.ClassVar[Event.Result]  # value = <Result.DENY: 0>
        __members__: typing.ClassVar[dict[str, Event.Result]]  # value = {'DENY': <Result.DENY: 0>, 'DEFAULT': <Result.DEFAULT: 1>, 'ALLOW': <Result.ALLOW: 2>}
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
    ALLOW: typing.ClassVar[Event.Result]  # value = <Result.ALLOW: 2>
    DEFAULT: typing.ClassVar[Event.Result]  # value = <Result.DEFAULT: 1>
    DENY: typing.ClassVar[Event.Result]  # value = <Result.DENY: 0>
    def asynchronous(self) -> bool:
        ...
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
    
    Members:
    
      LOWEST : Event call is of very low importance and should be run first, to allow other plugins to further customise the outcome
    
      LOW : Event call is of low importance
    
      NORMAL :  Event call is neither important nor unimportant, and may be run normally
    
      HIGH : Event call is of high importance
    
      HIGHEST : Event call is critical and must have the final say in what happens to the event
    
      MONITOR : Event is listened to purely for monitoring the outcome of an event. No modifications to the event should be made under this priority.
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
    Members:
    
      SURVIVAL
    
      CREATIVE
    
      ADVENTURE
    
      SPECTATOR
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
class HumanActor(Actor):
    pass
class Level:
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
class Logger:
    class Level:
        """
        Members:
        
          TRACE
        
          DEBUG
        
          INFO
        
          WARNING
        
          ERROR
        
          CRITICAL
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
    Members:
    
      TRUE
    
      FALSE
    
      OP
    
      OPERATOR
    
      NOT_OP
    
      NOT_OPERATOR
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
class Player(HumanActor):
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
    @property
    def unique_id(self) -> uuid.UUID:
        """
        Returns the UUID of this player
        """
class PlayerChatEvent(Event):
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
        Returns the Player who joins the server
        """
class PlayerJoinEvent(Event):
    @property
    def player(self) -> Player:
        """
        Returns the Player who joins the server
        """
class PlayerQuitEvent(Event):
    @property
    def player(self) -> Player:
        """
        Returns the Player who leaves the server
        """
class Plugin(CommandExecutor):
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
    @property
    def plugin(self) -> Plugin:
        ...
class PluginEnableEvent(Event):
    @property
    def plugin(self) -> Plugin:
        ...
class PluginLoadOrder:
    """
    Members:
    
      STARTUP
    
      POSTWORLD
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
class Server:
    def get_level(self, name: str) -> Level:
        """
        Gets the level with the given name.
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
    def plugin_manager(self) -> PluginManager:
        """
        Gets the plugin manager for interfacing with plugins.
        """
    @property
    def version(self) -> str:
        """
        Gets the version of this server implementation.
        """
class ServerCommandEvent(Event):
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
    class LoadType:
        """
        Members:
        
          STARTUP
        """
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
class ThunderChangeEvent(Event):
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
class WeatherChangeEvent(Event):
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
