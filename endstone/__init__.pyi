import collections
import datetime
import enum
import typing
import uuid

import numpy

from . import (
    actor,
    attribute,
    ban,
    block,
    boss,
    command,
    damage,
    effect,
    enchantments,
    event,
    form,
    inventory,
    lang,
    level,
    map,
    nbt,
    permissions,
    plugin,
    potion,
    scheduler,
    scoreboard,
    util,
)
from ._version import __version__
from .actor import Mob
from .ban import IpBanList, PlayerBanList
from .block import BlockData
from .boss import BarColor, BarFlag, BarStyle, BossBar
from .command import CommandSender, ConsoleCommandSender
from .form import ActionForm, MessageForm, ModalForm
from .inventory import Inventory, ItemFactory, PlayerInventory
from .lang import Language, Translatable
from .level import Dimension, Level, Location
from .map import MapView
from .plugin import PluginCommand, PluginManager, ServiceManager
from .scheduler import Scheduler
from .scoreboard import Scoreboard
from .util import SocketAddress

__all__ = [
    "__version__",
    "ColorFormat",
    "GameMode",
    "Logger",
    "OfflinePlayer",
    "Player",
    "Registry",
    "Server",
    "Skin",
    "__minecraft_version__",
    "actor",
    "attribute",
    "ban",
    "block",
    "boss",
    "command",
    "damage",
    "effect",
    "enchantments",
    "event",
    "form",
    "inventory",
    "lang",
    "level",
    "map",
    "nbt",
    "permissions",
    "plugin",
    "potion",
    "scheduler",
    "scoreboard",
    "util",
]

_T = typing.TypeVar("_T")

class Server:
    """
    Represents a server implementation.
    """
    @property
    def name(self) -> str:
        """
        Gets the name of this server implementation.
        """
        ...
    @property
    def version(self) -> str:
        """
        Gets the version of this server implementation.
        """
        ...
    @property
    def minecraft_version(self) -> str:
        """
        Gets the Minecraft version that this server is running.
        """
        ...
    @property
    def protocol_version(self) -> int:
        """
        Gets the network protocol version that this server supports.
        """
        ...
    @property
    def logger(self) -> Logger:
        """
        Returns the primary logger associated with this server instance.
        """
        ...
    @property
    def language(self) -> Language:
        """
        Gets the current language interface used by the server.
        """
        ...
    @property
    def plugin_manager(self) -> PluginManager:
        """
        Gets the plugin manager for interfacing with plugins.
        """
        ...
    def get_plugin_command(self, name: str) -> PluginCommand:
        """
        Gets a PluginCommand with the given name or alias.
        """
        ...
    @property
    def command_sender(self) -> ConsoleCommandSender:
        """
        Gets a CommandSender for this server.
        """
        ...
    def dispatch_command(self, sender: CommandSender, command_line: str) -> bool:
        """
        Dispatches a command on this server, and executes it if found.
        """
        ...
    @property
    def scheduler(self) -> Scheduler:
        """
        Gets the scheduler for managing scheduled events.
        """
        ...
    @property
    def service_manager(self) -> ServiceManager:
        """
        Gets the service manager.
        """
        ...
    def get_registry(self, type: typing.Type[_T]) -> Registry[_T]:
        """
        Returns the registry for the given type.
        """
        ...
    @property
    def level(self) -> Level:
        """
        Gets the server level.
        """
        ...
    @property
    def online_players(self) -> list[Player]:
        """
        Gets a list of all currently online players.
        """
        ...
    @property
    def max_players(self) -> int:
        """
        The maximum amount of players which can login to this server.
        """
        ...
    @max_players.setter
    def max_players(self, arg1: int) -> None: ...
    @typing.overload
    def get_player(self, name: str) -> Player:
        """
        Gets the player with the exact given name, case insensitive.
        """
        ...
    @typing.overload
    def get_player(self, unique_id: uuid.UUID) -> Player:
        """
        Gets the player with the given UUID.
        """
        ...
    @property
    def online_mode(self) -> bool:
        """
        Gets whether the Server is in online mode or not.
        """
        ...
    @property
    def port(self) -> int:
        """
        Get the game port that the server runs on.
        """
        ...
    @property
    def port_v6(self) -> int:
        """
        Get the game port (IPv6) that the server runs on.
        """
        ...
    def shutdown(self) -> None:
        """
        Shutdowns the server, stopping everything.
        """
        ...
    def reload(self) -> None:
        """
        Reloads the server configuration, functions, scripts and plugins.
        """
        ...
    def reload_data(self) -> None:
        """
        Reload only the Minecraft data for the server.
        """
        ...
    def broadcast(self, message: str | Translatable, permission: str) -> None:
        """
        Broadcasts the specified message to every user with the given permission name.
        """
        ...
    def broadcast_message(self, message: str | Translatable) -> None:
        """
        Broadcasts the specified message to every user with permission endstone.broadcast.user
        """
        ...
    @property
    def item_factory(self) -> ItemFactory:
        """
        Gets the instance of the item factory (for ItemMeta).
        """
        ...
    @property
    def scoreboard(self) -> Scoreboard:
        """
        Gets the primary Scoreboard controlled by the server.
        """
        ...
    def create_scoreboard(self) -> Scoreboard:
        """
        Creates a new Scoreboard to be tracked by the server.
        """
        ...
    @property
    def current_mspt(self) -> float:
        """
        Gets the current milliseconds per tick (MSPT).
        """
        ...
    @property
    def average_mspt(self) -> float:
        """
        Gets the average milliseconds per tick (MSPT).
        """
        ...
    @property
    def current_tps(self) -> float:
        """
        Gets the current ticks per second (TPS).
        """
        ...
    @property
    def average_tps(self) -> float:
        """
        Gets the average ticks per second (TPS).
        """
        ...
    @property
    def current_tick_usage(self) -> float:
        """
        Gets the current tick usage of the server.
        """
        ...
    @property
    def average_tick_usage(self) -> float:
        """
        Gets the average tick usage of the server.
        """
        ...
    @property
    def start_time(self) -> datetime.datetime:
        """
        Gets the start time of the server.
        """
        ...
    def create_boss_bar(
        self, title: str, color: BarColor, style: BarStyle, flags: list[BarFlag] | None = None
    ) -> BossBar:
        """
        Creates a boss bar instance to display to players. The progress defaults to 1.0.
        """
        ...
    def create_block_data(
        self, type: str, block_states: collections.abc.Mapping[str, bool | str | int] | None = None
    ) -> BlockData:
        """
        Creates a new BlockData instance for the specified block type, with all properties initialized to defaults, except for those provided.
        """
        ...
    @property
    def ban_list(self) -> PlayerBanList:
        """
        Gets the player ban list.
        """
        ...
    @property
    def ip_ban_list(self) -> IpBanList:
        """
        Gets the IP ban list.
        """
        ...
    def get_map(self, id: int) -> MapView:
        """
        Gets the map from the given map ID.
        """
        ...
    def create_map(self, dimension: Dimension) -> MapView:
        """
        Create a new map with an automatically assigned ID.
        """
        ...

class OfflinePlayer:
    """
    Represents a reference to a player identity and the data belonging to a player that is stored on the disk and can, thus, be retrieved without the player needing to be online.
    """
    @property
    def name(self) -> str:
        """
        Returns the name of this player
        """
        ...
    @property
    def unique_id(self) -> uuid.UUID:
        """
        Returns the UUID of this player
        """
        ...

class Player(Mob, OfflinePlayer):
    """
    Represents a player.
    """
    @property
    def name(self) -> str:
        """
        Returns the name of this player
        """
        ...
    @property
    def unique_id(self) -> uuid.UUID:
        """
        Returns the UUID of this player
        """
        ...
    @property
    def is_op(self) -> bool:
        """
        The operator status of this playerall
        """
        ...
    @is_op.setter
    def is_op(self, arg1: bool) -> None: ...
    @property
    def xuid(self) -> str:
        """
        Returns the Xbox User ID (XUID) of this player
        """
        ...
    @property
    def address(self) -> SocketAddress:
        """
        Gets the socket address of this player
        """
        ...
    def transfer(self, host: str, port: int = 19132) -> None:
        """
        Transfers the player to another server.
        """
        ...
    def kick(self, message: str) -> None:
        """
        Kicks player with custom kick message.
        """
        ...
    def perform_command(self, command: str) -> bool:
        """
        Makes the player perform the given command.
        """
        ...
    @property
    def is_sneaking(self) -> bool:
        """
        Gets or sets the sneak mode of the player
        """
        ...
    @is_sneaking.setter
    def is_sneaking(self, arg1: bool) -> None: ...
    @property
    def is_sprinting(self) -> bool:
        """
        Gets or sets whether the player is sprinting or not.
        """
        ...
    @is_sprinting.setter
    def is_sprinting(self, arg1: bool) -> None: ...
    def play_sound(self, location: Location, sound: str, volume: float = 1.0, pitch: float = 1.0) -> None:
        """
        Play a sound for a player at the location.
        """
        ...
    def stop_sound(self, sound: str) -> None:
        """
        Stop the specified sound from playing.
        """
        ...
    def stop_all_sounds(self) -> None:
        """
        Stop all sounds from playing.
        """
        ...
    def send_popup(self, message: str) -> None:
        """
        Sends this player a popup message
        """
        ...
    def send_tip(self, message: str) -> None:
        """
        Sends this player a tip message
        """
        ...
    def send_toast(self, title: str, content: str) -> None:
        """
        Sends this player a toast notification.
        """
        ...
    def give_exp(self, amount: int) -> None:
        """
        Gives the player the amount of experience specified.
        """
        ...
    def give_exp_levels(self, amount: int) -> None:
        """
        Gives the player the amount of experience levels specified.
        """
        ...
    @property
    def exp_progress(self) -> float:
        """
        Gets or sets the players current experience progress towards the next level.
        """
        ...
    @exp_progress.setter
    def exp_progress(self, arg1: float) -> None: ...
    @property
    def exp_level(self) -> int:
        """
        Gets or sets the players current experience level.
        """
        ...
    @exp_level.setter
    def exp_level(self, arg1: int) -> None: ...
    @property
    def total_exp(self) -> int:
        """
        Gets the players total experience points.
        """
        ...
    @property
    def is_flying(self) -> bool:
        """
        If the player is currently flying or not.
        """
        ...
    @is_flying.setter
    def is_flying(self, arg1: bool) -> None: ...
    @property
    def allow_flight(self) -> bool:
        """
        If the Player is allowed to fly via jump key double-tap.
        """
        ...
    @allow_flight.setter
    def allow_flight(self, arg1: bool) -> None: ...
    @property
    def fly_speed(self) -> float:
        """
        Gets or sets the current allowed speed that a client can fly.
        """
        ...
    @fly_speed.setter
    def fly_speed(self, arg1: float) -> None: ...
    @property
    def walk_speed(self) -> float:
        """
        Gets or sets the current allowed speed that a client can walk.
        """
        ...
    @walk_speed.setter
    def walk_speed(self, arg1: float) -> None: ...
    @property
    def scoreboard(self) -> Scoreboard:
        """
        Gets or sets the player's visible Scoreboard.
        """
        ...
    @scoreboard.setter
    def scoreboard(self, arg1: Scoreboard) -> None: ...
    def send_title(self, title: str, subtitle: str, fade_in: int = 10, stay: int = 70, fade_out: int = 20) -> None:
        """
        Sends a title and a subtitle message to the player. If they are empty strings, the display will be updated as such.
        """
        ...
    def reset_title(self) -> None:
        """
        Resets the title displayed to the player. This will clear the displayed title / subtitle and reset timings to their default values.
        """
        ...
    @typing.overload
    def spawn_particle(self, name: str, location: Location, molang_variables_json: str | None = None) -> None:
        """
        Spawns the particle at the target location.
        """
        ...
    @typing.overload
    def spawn_particle(self, name: str, x: float, y: float, z: float, molang_variables_json: str | None = None) -> None:
        """
        Spawns the particle at the target location.
        """
        ...
    @property
    def ping(self) -> int:
        """
        Gets the player's average ping in milliseconds.
        """
        ...
    def update_commands(self) -> None:
        """
        Send the list of commands to the client.
        """
        ...
    @property
    def game_mode(self) -> GameMode:
        """
        The player's current game mode.
        """
        ...
    @game_mode.setter
    def game_mode(self, arg1: GameMode) -> None: ...
    @property
    def inventory(self) -> PlayerInventory:
        """
        Get the player's inventory.
        """
        ...
    @property
    def ender_chest(self) -> Inventory:
        """
        Get the player's EnderChest inventory.
        """
        ...
    @property
    def locale(self) -> str:
        """
        Get the player's current locale.
        """
        ...
    @property
    def device_os(self) -> str:
        """
        Get the player's current device's operation system (OS).
        """
        ...
    @property
    def device_id(self) -> str:
        """
        Get the player's current device id.
        """
        ...
    @property
    def game_version(self) -> str:
        """
        Get the player's current game version.
        """
        ...
    @property
    def skin(self) -> Skin:
        """
        Get the player's skin.
        """
        ...
    def send_form(self, form: MessageForm | ActionForm | ModalForm) -> None:
        """
        Sends a form to the player.
        """
        ...
    def close_form(self) -> None:
        """
        Closes the forms that are currently open for the player.
        """
        ...
    def send_packet(self, packet_id: int, payload: bytes) -> None:
        """
        Sends a packet to the player.
        """
        ...

class ColorFormat:
    """
    All supported color and format codes.
    """

    BLACK = "§0"
    DARK_BLUE = "§1"
    DARK_GREEN = "§2"
    DARK_AQUA = "§3"
    DARK_RED = "§4"
    DARK_PURPLE = "§5"
    GOLD = "§6"
    GRAY = "§7"
    DARK_GRAY = "§8"
    BLUE = "§9"
    GREEN = "§a"
    AQUA = "§b"
    RED = "§c"
    LIGHT_PURPLE = "§d"
    YELLOW = "§e"
    WHITE = "§f"
    MINECOIN_GOLD = "§g"
    MATERIAL_QUARTZ = "§h"
    MATERIAL_IRON = "§i"
    MATERIAL_NETHERITE = "§j"
    MATERIAL_REDSTONE = "§m"
    MATERIAL_COPPER = "§n"
    MATERIAL_GOLD = "§p"
    MATERIAL_EMERALD = "§q"
    MATERIAL_DIAMOND = "§s"
    MATERIAL_LAPIS = "§t"
    MATERIAL_AMETHYST = "§u"
    MATERIAL_RESIN = "§v"
    OBFUSCATED = "§k"
    BOLD = "§l"
    ITALIC = "§o"
    RESET = "§r"

class GameMode(enum.Enum):
    """
    Represents the various type of game modes that Players may have.
    """

    SURVIVAL = 0
    CREATIVE = 1
    ADVENTURE = 2
    SPECTATOR = 3

class Logger:
    """
    Logger class which can format and output varies levels of logs.
    """
    class Level(enum.IntEnum):
        """
        Specifies the log level.
        """

        TRACE = 0
        DEBUG = 1
        INFO = 2
        WARNING = 3
        ERROR = 4
        CRITICAL = 5

    TRACE = Level.TRACE
    DEBUG = Level.DEBUG
    INFO = Level.INFO
    WARNING = Level.WARNING
    ERROR = Level.ERROR
    CRITICAL = Level.CRITICAL
    def set_level(self, level: Level) -> None:
        """
        Set the logging level for this Logger instance.
        """
        ...
    def is_enabled_for(self, level: Level) -> bool:
        """
        Check if the Logger instance is enabled for the given log Level.
        """
        ...
    def trace(self, message: str) -> None:
        """
        Log a message at the TRACE level.
        """
        ...
    def debug(self, message: str) -> None:
        """
        Log a message at the DEBUG level.
        """
        ...
    def info(self, message: str) -> None:
        """
        Log a message at the INFO level.
        """
        ...
    def warning(self, message: str) -> None:
        """
        Log a message at the WARNING level.
        """
        ...
    def error(self, message: str) -> None:
        """
        Log a message at the ERROR level.
        """
        ...
    def critical(self, message: str) -> None:
        """
        Log a message at the CRITICAL level.
        """
        ...
    @property
    def name(self) -> str:
        """
        Get the name of this Logger instance.
        """
        ...

class Skin:
    """
    Represents a player skin.
    """
    def __init__(
        self,
        id: str,
        image: numpy.ndarray[numpy.uint8],
        cape_id: str | None = None,
        cape_image: numpy.ndarray[numpy.uint8] | None = None,
    ) -> None: ...
    @property
    def id(self) -> str:
        """
        Get the Skin ID.
        """
        ...
    @property
    def image(self) -> numpy.ndarray[numpy.uint8]:
        """
        Get the Skin image.
        """
        ...
    @property
    def cape_id(self) -> str | None:
        """
        Get the Cape ID.
        """
        ...
    @property
    def cape_image(self) -> numpy.ndarray[numpy.uint8]:
        """
        Get the Cape image.
        """
        ...

__minecraft_version__ = "26.1"

class Registry(typing.Generic[_T]):
    """
    Presents a registry
    """
    def get(self, key: str) -> _T | None:
        """
        Get the object by its key.
        """
        ...
    def get_or_throw(self, key: str) -> _T:
        """
        Get the object by its key or throw if missing.
        """
        ...
    def __getitem__(self, key: str) -> _T: ...
    def __iter__(self) -> list: ...
    def __contains__(self, key: str) -> bool: ...
