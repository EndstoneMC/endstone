import collections.abc
import datetime
import enum
import typing
import uuid

import numpy
import numpy.typing

from endstone.actor import Mob
from endstone.ban import IpBanList, PlayerBanList
from endstone.block import BlockData, BlockType
from endstone.boss import BarColor, BarFlag, BarStyle, BossBar
from endstone.command import CommandMap, CommandSender, ConsoleCommandSender
from endstone.form import ActionForm, MessageForm, ModalForm
from endstone.inventory import Inventory, ItemFactory, PlayerInventory
from endstone.lang import Language, Translatable
from endstone.level import Dimension, Level, Location
from endstone.map import MapView
from endstone.plugin import PluginCommand, PluginManager, ServiceManager
from endstone.scheduler import Scheduler
from endstone.scoreboard import Scoreboard
from endstone.util import SocketAddress

from . import (
    actor,
    attribute,
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
    nbt,
    permissions,
    plugin,
    potion,
    scheduler,
    scoreboard,
    util,
)
from ._version import __version__

__all__ = [
    "ColorFormat",
    "GameMode",
    "GameRule",
    "Identifier",
    "Input",
    "Logger",
    "OfflinePlayer",
    "Player",
    "Registry",
    "Rotation",
    "Server",
    "Skin",
    "__minecraft_version__",
    "__version__",
    "actor",
    "attribute",
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
    "nbt",
    "permissions",
    "plugin",
    "potion",
    "scheduler",
    "scoreboard",
    "util",
]

_T = typing.TypeVar("_T")

class Identifier(typing.Generic[_T]):
    """
    Represents a namespaced identifier consisting of a namespace and a key.
    """
    def __init__(self, id: str) -> None: ...
    @property
    def namespace(self) -> str:
        """
        The namespace component of this identifier.
        """
    @property
    def key(self) -> str:
        """
        The key component of this identifier.
        """
    def __hash__(self) -> int: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...

class Server:
    """
    Represents a server implementation.
    """
    @property
    def name(self) -> str:
        """
        The name of this server implementation.
        """

    @property
    def version(self) -> str:
        """
        The version string of this server implementation.
        """

    @property
    def minecraft_version(self) -> str:
        """
        The Minecraft version that this server is running.
        """

    @property
    def protocol_version(self) -> int:
        """
        The network protocol version that this server supports.
        """

    @property
    def logger(self) -> Logger:
        """
        The primary logger associated with this server instance.
        """

    @property
    def language(self) -> Language:
        """
        The current language interface used by the server for translations.
        """

    @property
    def plugin_manager(self) -> PluginManager:
        """
        The plugin manager for interfacing with plugins.
        """

    @property
    def command_map(self) -> CommandMap:
        """
        The command map for registering and looking up commands.
        """

    def get_plugin_command(self, name: str) -> PluginCommand | None:
        """
        Gets a `PluginCommand` with the given name or alias.

        Args:
            name: The name of the command to retrieve.

        Returns:
            A `PluginCommand` if found, `None` otherwise.
        """

    @property
    def command_sender(self) -> ConsoleCommandSender:
        """
        A console `CommandSender` for this server.
        """

    def dispatch_command(self, sender: CommandSender, command_line: str) -> bool:
        """
        Dispatches a command on this server, and executes it if found.

        Args:
            sender: The apparent sender of the command.
            command_line: The command + arguments.

        Returns:
            `True` if execution is successful, `False` otherwise.
        """

    @property
    def scheduler(self) -> Scheduler:
        """
        The scheduler for managing scheduled events.
        """

    @property
    def service_manager(self) -> ServiceManager:
        """
        The service manager.
        """

    def get_registry(self, type: type[_T]) -> Registry[_T]:
        """
        Returns the registry for the given type.

        If no registry is present for the given type, `None` will be returned.

        Args:
            type: Type of the registry to get.

        Returns:
            The corresponding registry, or `None` if not present.
        """

    @property
    def level(self) -> Level:
        """
        The server level.
        """

    @property
    def online_players(self) -> list[Player]:
        """
        A list of all currently online players.
        """

    @property
    def max_players(self) -> int:
        """
        The maximum amount of players which can login to this server.
        """

    @max_players.setter
    def max_players(self, arg1: int) -> None: ...
    @typing.overload
    def get_player(self, name: str) -> Player | None:
        """
        Gets the player with the exact given name, case-insensitive.

        Args:
            name: Exact name of the player to retrieve.

        Returns:
            A player object if one was found, `None` otherwise.
        """

    @typing.overload
    def get_player(self, unique_id: uuid.UUID) -> Player | None:
        """
        Gets the player with the given UUID.

        Args:
            unique_id: UUID of the player to retrieve.

        Returns:
            A player object if one was found, `None` otherwise.
        """

    @property
    def online_mode(self) -> bool:
        """
        Whether the Server is in online mode or not.
        """

    @property
    def port(self) -> int:
        """
        The game port that the server runs on.
        """

    @property
    def port_v6(self) -> int:
        """
        The game port (IPv6) that the server runs on.
        """

    def shutdown(self) -> None:
        """
        Shutdowns the server, stopping everything.
        """

    def reload(self) -> None:
        """
        Reloads the server configuration, functions, scripts and plugins.
        """

    def reload_data(self) -> None:
        """
        Reload only the Minecraft data for the server.

        This includes functions and script files from all behaviour packs.
        """

    def broadcast(self, message: str | Translatable, permission: str) -> None:
        """
        Broadcasts the specified message to every user with the given permission name.

        Args:
            message: Message to broadcast.
            permission: The required permission Permissibles must have to receive the broadcast.
        """

    def broadcast_message(self, message: str | Translatable) -> None:
        """
        Broadcasts the specified message to every user with permission `endstone.broadcast.user`.

        Args:
            message: The message.
        """

    @property
    def item_factory(self) -> ItemFactory:
        """
        The instance of the item factory (for `ItemMeta`).
        """

    @property
    def scoreboard(self) -> Scoreboard | None:
        """
        The primary `Scoreboard` controlled by the server.

        This `Scoreboard` is saved by the server, is affected by the `/scoreboard` command, and is the
        scoreboard shown by default to players. This will only exist after the level has been loaded.
        """

    def create_scoreboard(self) -> Scoreboard:
        """
        Creates a new `Scoreboard` to be tracked by the server.

        This will not be saved by the server and is not affected by the `/scoreboard` command.

        Returns:
            The newly created `Scoreboard`.
        """

    @property
    def current_mspt(self) -> float:
        """
        The current milliseconds per tick (MSPT).
        """

    @property
    def average_mspt(self) -> float:
        """
        The average milliseconds per tick (MSPT).
        """

    @property
    def current_tps(self) -> float:
        """
        The current ticks per second (TPS).
        """

    @property
    def average_tps(self) -> float:
        """
        The average ticks per second (TPS).
        """

    @property
    def current_tick_usage(self) -> float:
        """
        The current tick usage of the server.
        """

    @property
    def average_tick_usage(self) -> float:
        """
        The average tick usage of the server.
        """

    @property
    def start_time(self) -> datetime.datetime:
        """
        The start time of the server.
        """

    def create_boss_bar(
        self, title: str, color: BarColor, style: BarStyle, flags: list[BarFlag] | None = None
    ) -> BossBar:
        """
        Creates a boss bar instance to display to players. The progress defaults to 1.0.

        Args:
            title: The title of the boss bar.
            color: The color of the boss bar.
            style: The style of the boss bar.
            flags: A list of flags to set on the boss bar.

        Returns:
            The created boss bar.
        """

    def create_block_data(
        self,
        type: Identifier[BlockType] | str,
        block_states: collections.abc.Mapping[str, bool | str | int] | None = None,
    ) -> BlockData:
        """
        Creates a new `BlockData` instance for the specified block type, with all properties initialized to defaults,
        except for those provided in `block_states`.

        Args:
            type: The block type.
            block_states: Block states, for example `{"old_leaf_type":"birch", "persistent_bit":True}`.

        Returns:
            New data instance.
        """

    @property
    def ban_list(self) -> PlayerBanList:
        """
        The player ban list.
        """

    @property
    def ip_ban_list(self) -> IpBanList:
        """
        The IP ban list.
        """

    def get_map(self, id: int) -> MapView:
        """
        Gets the map from the given item ID.

        Args:
            id: The id of the map to get.

        Returns:
            A map view if it exists, or `None` otherwise.
        """

    def create_map(self, dimension: Dimension) -> MapView:
        """
        Create a new map with an automatically assigned ID.

        Args:
            dimension: The `Dimension` the map will belong to.

        Returns:
            A newly created map view.
        """

class OfflinePlayer:
    """
    Represents a reference to a player identity and the data belonging to a player that is stored on the disk
    and can, thus, be retrieved without the player needing to be online.
    """
    @property
    def name(self) -> str:
        """
        The name of this player.

        Names are no longer unique past a single game session. For persistent storage it is recommended that
        you use `unique_id` instead. May be an empty string if we have not seen a name for this player yet.
        """

    @property
    def unique_id(self) -> uuid.UUID:
        """
        The UUID of this player.
        """

class Player(Mob):
    """
    Represents a player.
    """
    @property
    def name(self) -> str:
        """
        The name of this player.

        May be an empty string if we have not seen a name for this player yet.
        """

    @property
    def unique_id(self) -> uuid.UUID:
        """
        The UUID of this player.
        """

    @property
    def is_op(self) -> bool:
        """
        Whether this player is a server operator.
        """

    @is_op.setter
    def is_op(self, arg1: bool) -> None: ...
    @property
    def xuid(self) -> str:
        """
        The Xbox User ID (XUID) of this player.
        """

    @property
    def address(self) -> SocketAddress:
        """
        The socket address of this player.
        """

    def transfer(self, host: str, port: int = 19132) -> None:
        """
        Transfers the player to another server.

        Args:
            host: Server address to transfer the player to.
            port: Server port to transfer the player to.
        """

    def kick(self, message: str) -> None:
        """
        Kicks player with custom kick message.

        Args:
            message: Kick message.
        """

    def perform_command(self, command: str) -> bool:
        """
        Makes the player perform the given command.

        Args:
            command: Command to perform.

        Returns:
            `True` if the command was successful, `False` otherwise.
        """

    @property
    def is_sneaking(self) -> bool:
        """
        Whether the player is in sneak mode.
        """

    @is_sneaking.setter
    def is_sneaking(self, arg1: bool) -> None: ...
    @property
    def is_sprinting(self) -> bool:
        """
        Whether the player is sprinting or not.
        """

    @is_sprinting.setter
    def is_sprinting(self, arg1: bool) -> None: ...
    def play_sound(self, location: Location, sound: str, volume: float = 1.0, pitch: float = 1.0) -> None:
        """
        Play a sound for a player at the location.

        Args:
            location: The location to play the sound.
            sound: The internal sound name to play.
            volume: The volume of the sound.
            pitch: The pitch of the sound.
        """

    def stop_sound(self, sound: str) -> None:
        """
        Stop the specified sound from playing.

        Args:
            sound: The sound to stop.
        """

    def stop_all_sounds(self) -> None:
        """
        Stop all sounds from playing.
        """

    def send_action_bar(self, message: str) -> None:
        """
        Sends this player an action bar message.

        Args:
            message: Message to be displayed.
        """

    def send_popup(self, message: str) -> None:
        """
        Sends this player a popup message.

        Args:
            message: Message to be displayed.
        """

    def send_tip(self, message: str) -> None:
        """
        Sends this player a tip message.

        Args:
            message: Message to be displayed.
        """

    def send_toast(self, title: str, content: str) -> None:
        """
        Sends this player a toast notification.

        Args:
            title: The title of the toast notification.
            content: The content of the toast notification.
        """

    def give_exp(self, amount: int) -> None:
        """
        Gives the player the amount of experience specified.

        Args:
            amount: Exp amount to give.
        """

    def give_exp_levels(self, amount: int) -> None:
        """
        Gives the player the amount of experience levels specified.

        Args:
            amount: Amount of experience levels to give or take.
        """

    @property
    def exp_progress(self) -> float:
        """
        The player's current experience progress towards the next level.

        This is a percentage value. 0.0 is "no progress" and 1.0 is "next level".
        """

    @exp_progress.setter
    def exp_progress(self, arg1: float) -> None: ...
    @property
    def exp_level(self) -> int:
        """
        The player's current experience level.
        """

    @exp_level.setter
    def exp_level(self, arg1: int) -> None: ...
    @property
    def total_exp(self) -> int:
        """
        The player's total experience points.

        This refers to the total amount of experience the player has collected over time and is not currently
        displayed to the client.
        """

    @property
    def is_flying(self) -> bool:
        """
        Whether the player is currently flying or not.
        """

    @is_flying.setter
    def is_flying(self, arg1: bool) -> None: ...
    @property
    def is_crawling(self) -> bool:
        """
        Whether the player is currently crawling or not.
        """

    @property
    def allow_flight(self) -> bool:
        """
        Whether the Player is allowed to fly via jump key double-tap.
        """

    @allow_flight.setter
    def allow_flight(self, arg1: bool) -> None: ...
    @property
    def fly_speed(self) -> float:
        """
        The current allowed speed that a client can fly. Default is 0.05.
        """

    @fly_speed.setter
    def fly_speed(self, arg1: float) -> None: ...
    @property
    def walk_speed(self) -> float:
        """
        The current allowed speed that a client can walk. Default is 0.10.
        """

    @walk_speed.setter
    def walk_speed(self, arg1: float) -> None: ...
    @property
    def scoreboard(self) -> Scoreboard:
        """
        The player's visible `Scoreboard`.
        """

    @scoreboard.setter
    def scoreboard(self, arg1: Scoreboard) -> None: ...
    def send_title(self, title: str, subtitle: str, fade_in: int = 10, stay: int = 70, fade_out: int = 20) -> None:
        """
        Sends a title and a subtitle message to the player.

        If they are empty strings, the display will be updated as such.

        Args:
            title: Title text.
            subtitle: Subtitle text.
            fade_in: Time in ticks for titles to fade in. Defaults to 10.
            stay: Time in ticks for titles to stay. Defaults to 70.
            fade_out: Time in ticks for titles to fade out. Defaults to 20.
        """

    def reset_title(self) -> None:
        """
        Resets the title displayed to the player.

        This will clear the displayed title / subtitle and reset timings to their default values.
        """

    @typing.overload
    def spawn_particle(self, name: str, location: Location, molang_variables_json: str | None = None) -> None:
        """
        Spawns the particle at the target location.

        Args:
            name: The name of the particle effect to spawn.
            location: The location to spawn at.
            molang_variables_json: The customizable molang variables that can be adjusted for this particle, in json.
        """

    @typing.overload
    def spawn_particle(self, name: str, x: float, y: float, z: float, molang_variables_json: str | None = None) -> None:
        """
        Spawns the particle at the target location.

        Args:
            name: The name of the particle effect to spawn.
            x: The position on the x axis to spawn at.
            y: The position on the y axis to spawn at.
            z: The position on the z axis to spawn at.
            molang_variables_json: The customizable molang variables that can be adjusted for this particle, in json.
        """

    @property
    def ping(self) -> int:
        """
        The player's average ping in milliseconds.
        """

    def update_commands(self) -> None:
        """
        Send the list of commands to the client.

        Generally useful to ensure the client has a complete list of commands after permission changes are done.
        """

    @property
    def game_mode(self) -> GameMode:
        """
        The player's current `GameMode`.
        """

    @game_mode.setter
    def game_mode(self, arg1: GameMode) -> None: ...
    @property
    def inventory(self) -> PlayerInventory:
        """
        The player's inventory. This also contains the armor slots.
        """

    @property
    def ender_chest(self) -> Inventory:
        """
        The player's `EnderChest` inventory.
        """

    @property
    def locale(self) -> str:
        """
        The player's current locale.
        """

    @property
    def device_os(self) -> str:
        """
        The player's current device's operation system (OS).
        """

    @property
    def device_id(self) -> str:
        """
        The player's current device id.
        """

    @property
    def game_version(self) -> str:
        """
        The player's current game version.
        """

    @property
    def skin(self) -> Skin:
        """
        The player's skin.
        """

    def send_form(self, form: MessageForm | ActionForm | ModalForm) -> None:
        """
        Sends a form to the player.

        Args:
            form: The form to send.
        """

    def close_form(self) -> None:
        """
        Closes the forms that are currently open for the player.
        """

    def send_map(self, map: MapView) -> None:
        """
        Render a map and send it to the player in its entirety.

        This may be used when streaming the map in the normal manner is not desirable.

        Args:
            map: The map to send.
        """

    def send_packet(self, packet_id: int, payload: bytes) -> None:
        """
        Sends a packet to the player.

        Args:
            packet_id: The packet ID to be sent.
            payload: The payload of the packet to be transmitted.
        """

class ColorFormat:
    """
    All supported color and format codes.
    """

    BLACK: str = "§0"
    DARK_BLUE: str = "§1"
    DARK_GREEN: str = "§2"
    DARK_AQUA: str = "§3"
    DARK_RED: str = "§4"
    DARK_PURPLE: str = "§5"
    GOLD: str = "§6"
    GRAY: str = "§7"
    DARK_GRAY: str = "§8"
    BLUE: str = "§9"
    GREEN: str = "§a"
    AQUA: str = "§b"
    RED: str = "§c"
    LIGHT_PURPLE: str = "§d"
    YELLOW: str = "§e"
    WHITE: str = "§f"
    MINECOIN_GOLD: str = "§g"
    MATERIAL_QUARTZ: str = "§h"
    MATERIAL_IRON: str = "§i"
    MATERIAL_NETHERITE: str = "§j"
    MATERIAL_REDSTONE: str = "§m"
    MATERIAL_COPPER: str = "§n"
    MATERIAL_GOLD: str = "§p"
    MATERIAL_EMERALD: str = "§q"
    MATERIAL_DIAMOND: str = "§s"
    MATERIAL_LAPIS: str = "§t"
    MATERIAL_AMETHYST: str = "§u"
    MATERIAL_RESIN: str = "§v"
    OBFUSCATED: str = "§k"
    BOLD: str = "§l"
    ITALIC: str = "§o"
    RESET: str = "§r"

class GameMode(enum.Enum):
    """
    Represents the various type of game modes that Players may have.
    """

    SURVIVAL = 0
    CREATIVE = 1
    ADVENTURE = 2
    SPECTATOR = 3

class GameRule(typing.Generic[_T]):
    """
    Represents a game rule.
    """
    def __hash__(self) -> int: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def id(self) -> Identifier[GameRule[_T]]:
        """
        The identifier of this game rule.
        """

    @property
    def translation_key(self) -> str:
        """
        Raises, as Bedrock does not localize game rule names.
        """

    @typing.overload
    @staticmethod
    def get(name: Identifier[GameRule[_T]]) -> GameRule[_T] | None: ...
    @typing.overload
    @staticmethod
    def get(name: str) -> GameRule[typing.Any] | None:
        """
        Attempts to get the `GameRule` with the given name.

        Args:
            name: The identifier of the game rule (e.g. `minecraft:dofiretick`).

        Returns:
            The `GameRule`, or `None` if no game rule with that name exists.
        """

    COMMAND_BLOCK_OUTPUT: Identifier[GameRule[bool]] = "minecraft:commandblockoutput"
    COMMAND_BLOCKS_ENABLED: Identifier[GameRule[bool]] = "minecraft:commandblocksenabled"
    DO_DAY_LIGHT_CYCLE: Identifier[GameRule[bool]] = "minecraft:dodaylightcycle"
    DO_ENTITY_DROPS: Identifier[GameRule[bool]] = "minecraft:doentitydrops"
    DO_FIRE_TICK: Identifier[GameRule[bool]] = "minecraft:dofiretick"
    DO_IMMEDIATE_RESPAWN: Identifier[GameRule[bool]] = "minecraft:doimmediaterespawn"
    DO_INSOMNIA: Identifier[GameRule[bool]] = "minecraft:doinsomnia"
    DO_LIMITED_CRAFTING: Identifier[GameRule[bool]] = "minecraft:dolimitedcrafting"
    DO_MOB_LOOT: Identifier[GameRule[bool]] = "minecraft:domobloot"
    DO_MOB_SPAWNING: Identifier[GameRule[bool]] = "minecraft:domobspawning"
    DO_TILE_DROPS: Identifier[GameRule[bool]] = "minecraft:dotiledrops"
    DO_WEATHER_CYCLE: Identifier[GameRule[bool]] = "minecraft:doweathercycle"
    DROWNING_DAMAGE: Identifier[GameRule[bool]] = "minecraft:drowningdamage"
    FALL_DAMAGE: Identifier[GameRule[bool]] = "minecraft:falldamage"
    FIRE_DAMAGE: Identifier[GameRule[bool]] = "minecraft:firedamage"
    FREEZE_DAMAGE: Identifier[GameRule[bool]] = "minecraft:freezedamage"
    FUNCTION_COMMAND_LIMIT: Identifier[GameRule[int]] = "minecraft:functioncommandlimit"
    KEEP_INVENTORY: Identifier[GameRule[bool]] = "minecraft:keepinventory"
    LOCATOR_BAR: Identifier[GameRule[bool]] = "minecraft:locatorbar"
    MAX_COMMAND_CHAIN_LENGTH: Identifier[GameRule[int]] = "minecraft:maxcommandchainlength"
    MOB_GRIEFING: Identifier[GameRule[bool]] = "minecraft:mobgriefing"
    NATURAL_REGENERATION: Identifier[GameRule[bool]] = "minecraft:naturalregeneration"
    PLAYERS_SLEEPING_PERCENTAGE: Identifier[GameRule[int]] = "minecraft:playerssleepingpercentage"
    PLAYER_WAYPOINTS: Identifier[GameRule[int]] = "minecraft:playerwaypoints"
    PROJECTILES_CAN_BREAK_BLOCKS: Identifier[GameRule[bool]] = "minecraft:projectilescanbreakblocks"
    PVP: Identifier[GameRule[bool]] = "minecraft:pvp"
    RANDOM_TICK_SPEED: Identifier[GameRule[int]] = "minecraft:randomtickspeed"
    RECIPES_UNLOCK: Identifier[GameRule[bool]] = "minecraft:recipesunlock"
    RESPAWN_BLOCKS_EXPLODE: Identifier[GameRule[bool]] = "minecraft:respawnblocksexplode"
    SEND_COMMAND_FEEDBACK: Identifier[GameRule[bool]] = "minecraft:sendcommandfeedback"
    SHOW_BORDER_EFFECT: Identifier[GameRule[bool]] = "minecraft:showbordereffect"
    SHOW_COORDINATES: Identifier[GameRule[bool]] = "minecraft:showcoordinates"
    SHOW_DAYS_PLAYED: Identifier[GameRule[bool]] = "minecraft:showdaysplayed"
    SHOW_DEATH_MESSAGES: Identifier[GameRule[bool]] = "minecraft:showdeathmessages"
    SHOW_RECIPE_MESSAGES: Identifier[GameRule[bool]] = "minecraft:showrecipemessages"
    SHOW_TAGS: Identifier[GameRule[bool]] = "minecraft:showtags"
    SPAWN_RADIUS: Identifier[GameRule[int]] = "minecraft:spawnradius"
    TNT_EXPLODES: Identifier[GameRule[bool]] = "minecraft:tntexplodes"
    TNT_EXPLOSION_DROP_DECAY: Identifier[GameRule[bool]] = "minecraft:tntexplosiondropdecay"

class Rotation(enum.Enum):
    """
    Represents a rotation based orientation, like that on a clock. It represents how something is viewed, as opposed to cardinal directions.
    """

    NONE = 0
    CLOCKWISE_45 = 1
    CLOCKWISE = 2
    CLOCKWISE_135 = 3
    FLIPPED = 4
    FLIPPED_45 = 5
    COUNTER_CLOCKWISE = 6
    COUNTER_CLOCKWISE_45 = 7

class Input:
    """
    Represents a movement input applied to a player.
    """
    @property
    def is_forward(self) -> bool:
        """
        Whether a forward input is applied.
        """

    @property
    def is_backward(self) -> bool:
        """
        Whether a backward input is applied.
        """

    @property
    def is_left(self) -> bool:
        """
        Whether a left input is applied.
        """

    @property
    def is_right(self) -> bool:
        """
        Whether a right input is applied.
        """

    @property
    def is_jump(self) -> bool:
        """
        Whether a jump input is applied.
        """

    @property
    def is_sneak(self) -> bool:
        """
        Whether a sneak input is applied.
        """

    @property
    def is_sprint(self) -> bool:
        """
        Whether a sprint input is applied.
        """

    def __eq__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...

class Logger:
    """
    `Logger` class which can format and output varies levels of logs.
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
        Set the logging level for this `Logger` instance.

        Args:
            level: The desired log level.
        """

    def is_enabled_for(self, level: Level) -> bool:
        """
        Check if the `Logger` instance is enabled for the given log `Level`.

        Args:
            level: The log level to check.

        Returns:
            `True` if the logger is enabled for the `Level`, `False` otherwise.
        """

    def trace(self, message: str) -> None:
        """
        Log a message at the `TRACE` level.
        """

    def debug(self, message: str) -> None:
        """
        Log a message at the `DEBUG` level.
        """

    def info(self, message: str) -> None:
        """
        Log a message at the `INFO` level.
        """

    def warning(self, message: str) -> None:
        """
        Log a message at the `WARNING` level.
        """

    def error(self, message: str) -> None:
        """
        Log a message at the `ERROR` level.
        """

    def critical(self, message: str) -> None:
        """
        Log a message at the `CRITICAL` level.
        """

    @property
    def name(self) -> str:
        """
        The name of this `Logger` instance.
        """

class Skin:
    """
    Represents a player skin.
    """
    def __init__(
        self,
        id: str,
        image: numpy.typing.NDArray[numpy.uint8],
        cape_id: str | None = None,
        cape_image: numpy.typing.NDArray[numpy.uint8] | None = None,
    ) -> None: ...
    @property
    def id(self) -> str:
        """
        The skin id.
        """

    @property
    def image(self) -> numpy.typing.NDArray[numpy.uint8]:
        """
        The skin image.
        """

    @property
    def cape_id(self) -> str | None:
        """
        The cape id.
        """

    @property
    def cape_image(self) -> numpy.typing.NDArray[numpy.uint8]:
        """
        The cape image.
        """

class Registry(typing.Generic[_T]):
    """
    Presents a registry
    """
    def get(self, id: Identifier[_T] | str) -> _T | None:
        """
        Get the object by its key.
        """
    def get_or_throw(self, id: Identifier[_T] | str) -> _T:
        """
        Get the object by its key or throw if missing.
        """
    def __getitem__(self, id: Identifier[_T] | str) -> _T: ...
    def __iter__(self) -> typing.Iterator[_T]: ...
    def __contains__(self, id: Identifier[_T] | str) -> bool: ...
    def __len__(self) -> int: ...

__minecraft_version__: str = "26.44"
