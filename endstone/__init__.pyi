from __future__ import annotations

import collections
import datetime
import enum
import typing
import uuid

import numpy

from . import (
    actor,
    ban,
    block,
    boss,
    command,
    damage,
    enchantments,
    event,
    form,
    inventory,
    lang,
    level,
    map,
    permissions,
    plugin,
    scheduler,
    scoreboard,
    util,
)

__all__: list[str] = [
    "__minecraft_version__",
    "ColorFormat",
    "EnchantmentRegistry",
    "GameMode",
    "ItemRegistry",
    "Logger",
    "OfflinePlayer",
    "Player",
    "Server",
    "Skin",
    "actor",
    "ban",
    "block",
    "boss",
    "command",
    "damage",
    "enchantments",
    "event",
    "form",
    "inventory",
    "lang",
    "level",
    "map",
    "permissions",
    "plugin",
    "scheduler",
    "scoreboard",
    "util",
]

__minecraft_version__: str

class ColorFormat:
    """
    All supported color and format codes.
    """

    AQUA: typing.ClassVar[str] = "§b"
    BLACK: typing.ClassVar[str] = "§0"
    BLUE: typing.ClassVar[str] = "§9"
    BOLD: typing.ClassVar[str] = "§l"
    DARK_AQUA: typing.ClassVar[str] = "§3"
    DARK_BLUE: typing.ClassVar[str] = "§1"
    DARK_GRAY: typing.ClassVar[str] = "§8"
    DARK_GREEN: typing.ClassVar[str] = "§2"
    DARK_PURPLE: typing.ClassVar[str] = "§5"
    DARK_RED: typing.ClassVar[str] = "§4"
    GOLD: typing.ClassVar[str] = "§6"
    GRAY: typing.ClassVar[str] = "§7"
    GREEN: typing.ClassVar[str] = "§a"
    ITALIC: typing.ClassVar[str] = "§o"
    LIGHT_PURPLE: typing.ClassVar[str] = "§d"
    MATERIAL_AMETHYST: typing.ClassVar[str] = "§u"
    MATERIAL_COPPER: typing.ClassVar[str] = "§n"
    MATERIAL_DIAMOND: typing.ClassVar[str] = "§s"
    MATERIAL_EMERALD: typing.ClassVar[str] = "§q"
    MATERIAL_GOLD: typing.ClassVar[str] = "§p"
    MATERIAL_IRON: typing.ClassVar[str] = "§i"
    MATERIAL_LAPIS: typing.ClassVar[str] = "§t"
    MATERIAL_NETHERITE: typing.ClassVar[str] = "§j"
    MATERIAL_QUARTZ: typing.ClassVar[str] = "§h"
    MATERIAL_REDSTONE: typing.ClassVar[str] = "§m"
    MATERIAL_RESIN: typing.ClassVar[str] = "§v"
    MINECOIN_GOLD: typing.ClassVar[str] = "§g"
    OBFUSCATED: typing.ClassVar[str] = "§k"
    RED: typing.ClassVar[str] = "§c"
    RESET: typing.ClassVar[str] = "§r"
    WHITE: typing.ClassVar[str] = "§f"
    YELLOW: typing.ClassVar[str] = "§e"

class EnchantmentRegistry:
    def __contains__(self, key: str) -> bool: ...
    def __getitem__(self, key: str) -> enchantments.Enchantment: ...
    def __iter__(self) -> list: ...
    def get(self, key: str) -> enchantments.Enchantment: ...
    def get_or_throw(self, key: str) -> enchantments.Enchantment: ...

class GameMode(enum.Enum):
    """
    Represents the various type of game modes that Players may have.
    """

    ADVENTURE: typing.ClassVar[GameMode]  # value = <GameMode.ADVENTURE: 2>
    CREATIVE: typing.ClassVar[GameMode]  # value = <GameMode.CREATIVE: 1>
    SPECTATOR: typing.ClassVar[GameMode]  # value = <GameMode.SPECTATOR: 3>
    SURVIVAL: typing.ClassVar[GameMode]  # value = <GameMode.SURVIVAL: 0>

class ItemRegistry:
    def __contains__(self, key: str) -> bool: ...
    def __getitem__(self, key: str) -> inventory.ItemType: ...
    def __iter__(self) -> list: ...
    def get(self, key: str) -> inventory.ItemType: ...
    def get_or_throw(self, key: str) -> inventory.ItemType: ...

class Logger:
    """
    Logger class which can format and output varies levels of logs.
    """

    class Level(enum.IntEnum):
        """
        Specifies the log level.
        """

        CRITICAL: typing.ClassVar[Logger.Level]  # value = <Level.CRITICAL: 5>
        DEBUG: typing.ClassVar[Logger.Level]  # value = <Level.DEBUG: 1>
        ERROR: typing.ClassVar[Logger.Level]  # value = <Level.ERROR: 4>
        INFO: typing.ClassVar[Logger.Level]  # value = <Level.INFO: 2>
        TRACE: typing.ClassVar[Logger.Level]  # value = <Level.TRACE: 0>
        WARNING: typing.ClassVar[Logger.Level]  # value = <Level.WARNING: 3>

        @classmethod
        def __new__(cls, value): ...
        def __format__(self, format_spec):
            """
            Convert to a string according to format_spec.
            """

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

class OfflinePlayer:
    """
    Represents a reference to a player identity and the data belonging to a player that is stored on the disk and can, thus, be retrieved without the player needing to be online.
    """

    @property
    def name(self) -> str:
        """
        Returns the name of this player
        """

    @property
    def unique_id(self) -> uuid.UUID:
        """
        Returns the UUID of this player
        """

class Player(actor.Mob, OfflinePlayer):
    """
    Represents a player.
    """

    def close_form(self) -> None:
        """
        Closes the forms that are currently open for the player.
        """

    def give_exp(self, amount: typing.SupportsInt) -> None:
        """
        Gives the player the amount of experience specified.
        """

    def give_exp_levels(self, amount: typing.SupportsInt) -> None:
        """
        Gives the player the amount of experience levels specified.
        """

    def kick(self, message: str) -> None:
        """
        Kicks player with custom kick message.
        """

    def perform_command(self, command: str) -> bool:
        """
        Makes the player perform the given command.
        """

    def play_sound(
        self,
        location: level.Location,
        sound: str,
        volume: typing.SupportsFloat = 1.0,
        pitch: typing.SupportsFloat = 1.0,
    ) -> None:
        """
        Play a sound for a player at the location.
        """

    def reset_title(self) -> None:
        """
        Resets the title displayed to the player. This will clear the displayed title / subtitle and reset timings to their default values.
        """

    def send_form(
        self,
        form: form.MessageForm | form.ActionForm | form.ModalForm,
    ) -> None:
        """
        Sends a form to the player.
        """

    def send_packet(self, packet_id: typing.SupportsInt, payload: bytes) -> None:
        """
        Sends a packet to the player.
        """

    def send_popup(self, message: str) -> None:
        """
        Sends this player a popup message
        """

    def send_tip(self, message: str) -> None:
        """
        Sends this player a tip message
        """

    def send_title(
        self,
        title: str,
        subtitle: str,
        fade_in: typing.SupportsInt = 10,
        stay: typing.SupportsInt = 70,
        fade_out: typing.SupportsInt = 20,
    ) -> None:
        """
        Sends a title and a subtitle message to the player. If they are empty strings, the display will be updated as such.
        """

    def send_toast(self, title: str, content: str) -> None:
        """
        Sends this player a toast notification.
        """

    @typing.overload
    def spawn_particle(self, name: str, location: level.Location, molang_variables_json: str | None = None) -> None:
        """
        Spawns the particle at the target location.
        """

    @typing.overload
    def spawn_particle(
        self,
        name: str,
        x: typing.SupportsFloat,
        y: typing.SupportsFloat,
        z: typing.SupportsFloat,
        molang_variables_json: str | None = None,
    ) -> None:
        """
        Spawns the particle at the target location.
        """

    def stop_all_sounds(self) -> None:
        """
        Stop all sounds from playing.
        """

    def stop_sound(self, sound: str) -> None:
        """
        Stop the specified sound from playing.
        """

    def transfer(self, host: str, port: typing.SupportsInt = 19132) -> None:
        """
        Transfers the player to another server.
        """

    def update_commands(self) -> None:
        """
        Send the list of commands to the client.
        """

    @property
    def address(self) -> util.SocketAddress:
        """
        Gets the socket address of this player
        """

    @property
    def allow_flight(self) -> bool:
        """
        If the Player is allowed to fly via jump key double-tap.
        """

    @allow_flight.setter
    def allow_flight(self, arg1: bool) -> None: ...
    @property
    def device_id(self) -> str:
        """
        Get the player's current device id.
        """

    @property
    def device_os(self) -> str:
        """
        Get the player's current device's operation system (OS).
        """

    @property
    def ender_chest(self) -> inventory.Inventory:
        """
        Get the player's EnderChest inventory.
        """

    @property
    def exp_level(self) -> int:
        """
        Gets or sets the players current experience level.
        """

    @exp_level.setter
    def exp_level(self, arg1: typing.SupportsInt) -> None: ...
    @property
    def exp_progress(self) -> float:
        """
        Gets or sets the players current experience progress towards the next level.
        """

    @exp_progress.setter
    def exp_progress(self, arg1: typing.SupportsFloat) -> None: ...
    @property
    def fly_speed(self) -> float:
        """
        Gets or sets the current allowed speed that a client can fly.
        """

    @fly_speed.setter
    def fly_speed(self, arg1: typing.SupportsFloat) -> None: ...
    @property
    def game_mode(self) -> GameMode:
        """
        The player's current game mode.
        """

    @game_mode.setter
    def game_mode(self, arg1: GameMode) -> None: ...
    @property
    def game_version(self) -> str:
        """
        Get the player's current game version.
        """

    @property
    def inventory(self) -> inventory.PlayerInventory:
        """
        Get the player's inventory.
        """

    @property
    def is_flying(self) -> bool:
        """
        If the player is currently flying or not.
        """

    @is_flying.setter
    def is_flying(self, arg1: bool) -> None: ...
    @property
    def is_op(self) -> bool:
        """
        The operator status of this playerall
        """

    @is_op.setter
    def is_op(self, arg1: bool) -> None: ...
    @property
    def is_sneaking(self) -> bool:
        """
        Gets or sets the sneak mode of the player
        """

    @is_sneaking.setter
    def is_sneaking(self, arg1: bool) -> None: ...
    @property
    def is_sprinting(self) -> bool:
        """
        Gets or sets whether the player is sprinting or not.
        """

    @is_sprinting.setter
    def is_sprinting(self, arg1: bool) -> None: ...
    @property
    def locale(self) -> str:
        """
        Get the player's current locale.
        """

    @property
    def name(self) -> str:
        """
        Returns the name of this player
        """

    @property
    def ping(self) -> int:
        """
        Gets the player's average ping in milliseconds.
        """

    @property
    def scoreboard(self) -> scoreboard.Scoreboard:
        """
        Gets or sets the player's visible Scoreboard.
        """

    @scoreboard.setter
    def scoreboard(self, arg1: scoreboard.Scoreboard) -> None: ...
    @property
    def skin(self) -> Skin:
        """
        Get the player's skin.
        """

    @property
    def total_exp(self) -> int:
        """
        Gets the players total experience points.
        """

    @property
    def unique_id(self) -> uuid.UUID:
        """
        Returns the UUID of this player
        """

    @property
    def walk_speed(self) -> float:
        """
        Gets or sets the current allowed speed that a client can walk.
        """

    @walk_speed.setter
    def walk_speed(self, arg1: typing.SupportsFloat) -> None: ...
    @property
    def xuid(self) -> str:
        """
        Returns the Xbox User ID (XUID) of this player
        """

class Server:
    """
    Represents a server implementation.
    """

    def broadcast(self, message: str | lang.Translatable, permission: str) -> None:
        """
        Broadcasts the specified message to every user with the given permission name.
        """

    def broadcast_message(self, message: str | lang.Translatable) -> None:
        """
        Broadcasts the specified message to every user with permission endstone.broadcast.user
        """

    def create_block_data(
        self, type: str, block_states: collections.abc.Mapping[str, bool | str | typing.SupportsInt] | None = None
    ) -> block.BlockData:
        """
        Creates a new BlockData instance for the specified block type, with all properties initialized to defaults, except for those provided.
        """

    def create_boss_bar(
        self,
        title: str,
        color: boss.BarColor,
        style: boss.BarStyle,
        flags: collections.abc.Sequence[boss.BarFlag] | None = None,
    ) -> boss.BossBar:
        """
        Creates a boss bar instance to display to players. The progress defaults to 1.0.
        """

    def create_scoreboard(self) -> scoreboard.Scoreboard:
        """
        Creates a new Scoreboard to be tracked by the server.
        """

    def dispatch_command(self, sender: command.CommandSender, command_line: str) -> bool:
        """
        Dispatches a command on this server, and executes it if found.
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

    def get_plugin_command(self, name: str) -> plugin.PluginCommand:
        """
        Gets a PluginCommand with the given name or alias.
        """

    def reload(self) -> None:
        """
        Reloads the server configuration, functions, scripts and plugins.
        """

    def reload_data(self) -> None:
        """
        Reload only the Minecraft data for the server.
        """

    def shutdown(self) -> None:
        """
        Shutdowns the server, stopping everything.
        """

    @property
    def average_mspt(self) -> float:
        """
        Gets the average milliseconds per tick (MSPT).
        """

    @property
    def average_tick_usage(self) -> float:
        """
        Gets the average tick usage of the server.
        """

    @property
    def average_tps(self) -> float:
        """
        Gets the average ticks per second (TPS).
        """

    @property
    def ban_list(self) -> ban.PlayerBanList:
        """
        Gets the player ban list.
        """

    @property
    def command_sender(self) -> command.ConsoleCommandSender:
        """
        Gets a CommandSender for this server.
        """

    @property
    def current_mspt(self) -> float:
        """
        Gets the current milliseconds per tick (MSPT).
        """

    @property
    def current_tick_usage(self) -> float:
        """
        Gets the current tick usage of the server.
        """

    @property
    def current_tps(self) -> float:
        """
        Gets the current ticks per second (TPS).
        """

    @property
    def enchantment_registry(self) -> EnchantmentRegistry:
        """
        Returns the registry for all the enchantments.
        """

    @property
    def ip_ban_list(self) -> ban.IpBanList:
        """
        Gets the IP ban list.
        """

    @property
    def item_factory(self) -> inventory.ItemFactory:
        """
        Gets the instance of the item factory (for ItemMeta).
        """

    @property
    def item_registry(self) -> ItemRegistry:
        """
        Returns the registry for all the item types.
        """

    @property
    def language(self) -> lang.Language:
        """
        Gets the current language interface used by the server.
        """

    @property
    def level(self) -> level.Level:
        """
        Gets the server level.
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
    def max_players(self, arg1: typing.SupportsInt) -> None: ...
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
    def online_mode(self) -> bool:
        """
        Gets whether the Server is in online mode or not.
        """

    @property
    def online_players(self) -> list[Player]:
        """
        Gets a list of all currently online players.
        """

    @property
    def plugin_manager(self) -> plugin.PluginManager:
        """
        Gets the plugin manager for interfacing with plugins.
        """

    @property
    def port(self) -> int:
        """
        Get the game port that the server runs on.
        """

    @property
    def port_v6(self) -> int:
        """
        Get the game port (IPv6) that the server runs on.
        """

    @property
    def protocol_version(self) -> int:
        """
        Gets the network protocol version that this server supports.
        """

    @property
    def scheduler(self) -> scheduler.Scheduler:
        """
        Gets the scheduler for managing scheduled events.
        """

    @property
    def scoreboard(self) -> scoreboard.Scoreboard:
        """
        Gets the primary Scoreboard controlled by the server.
        """

    @property
    def service_manager(self) -> plugin.ServiceManager:
        """
        Gets the service manager.
        """

    @property
    def start_time(self) -> datetime.datetime:
        """
        Gets the start time of the server.
        """

    @property
    def version(self) -> str:
        """
        Gets the version of this server implementation.
        """

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
    def cape_id(self) -> str | None:
        """
        Get the Cape ID.
        """

    @property
    def cape_image(self) -> numpy.ndarray[numpy.uint8]:
        """
        Get the Cape image.
        """

    @property
    def id(self) -> str:
        """
        Get the Skin ID.
        """

    @property
    def image(self) -> numpy.ndarray[numpy.uint8]:
        """
        Get the Skin image.
        """
