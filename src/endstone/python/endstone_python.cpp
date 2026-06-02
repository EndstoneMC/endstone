// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "endstone_python.h"

#include <utility>

#include "endstone/detail.h"
#include "endstone/endstone.hpp"
#include "event.h"
#include "registry.h"
#include "type_caster.h"

namespace endstone::python {
void init_actor(py::module_ &, py_class<Actor> &actor, py_class<Mob> &mob);
void init_attribute(py::module_ &);
void init_ban(py::module_ &);
void init_block(py::module_ &, py::classh<Block> &block);
void init_boss(py::module_ &);
void init_color_format(py::module_ &);
void init_command(py::module &, py_class<CommandSender> &command_sender);
void init_damage(py::module_ &);
void init_effect(py::module_ &);
void init_enchantments(py::module_ &);
void init_event(py::module_ &, py::class_<Event, PyEvent> &event);
void init_form(py::module_ &);
void init_game_mode(py::module_ &);
void init_inventory(py::module_ &, py::class_<ItemStack> &item_stack);
void init_lang(py::module_ &);
void init_level(py::module_ &, py::classh<Level> &level, py::classh<Dimension> &dimension,
                py::class_<Location> &location);
void init_logger(py::module_ &);
void init_map(py::module_ &);
void init_nbt(py::module_ &);
void init_permissions(py::module_ &, py_class<Permissible> &permissible, py::classh<Permission> &permission);
void init_player(py::module_ &, py_class<Player> &player);
void init_plugin(py::module_ &);
void init_potion(py::module_ &);
void init_registry(py::module_ &);
void init_scheduler(py::module_ &);
void init_scoreboard(py::module_ &);
void init_server(py::classh<Server> &server);
void init_util(py::module_ &, py::class_<Vector> &);

PYBIND11_MODULE(_python, m)  // NOLINT(*-use-anonymous-namespace)
{
    py::options options;
    options.disable_enum_members_docstring();

    // Identifier (registered early, before classes that use it via type caster)
    py::class_<PyIdentifier>(m, "Identifier",
                             "Represents an identifier consisting of two components: a `namespace` and a `key`.")
        .def(py::init<std::string>(), py::arg("id"))
        .def(py::init<std::string, std::string>(), py::arg("namespace_"), py::arg("key"))
        .def_property_readonly(
            "namespace", [](const PyIdentifier &self) { return self.namespace_; },
            "The namespace component of this identifier.")
        .def_property_readonly(
            "key", [](const PyIdentifier &self) { return self.key_; }, "The key component of this identifier.")
        .def("__str__", &PyIdentifier::str)
        .def("__repr__", &PyIdentifier::str)
        .def("__hash__",
             [](const PyIdentifier &self) { return py::hash(py::str(self.str())); })
        .def("__eq__",
             [](const PyIdentifier &self, const py::object &other) {
                 if (py::isinstance<PyIdentifier>(other)) {
                     return self == other.cast<PyIdentifier>();
                 }
                 if (py::isinstance<py::str>(other)) {
                     return self.str() == other.cast<std::string>();
                 }
                 return false;
             })
        .def("__ne__",
             [](const PyIdentifier &self, const py::object &other) {
                 if (py::isinstance<PyIdentifier>(other)) {
                     return !(self == other.cast<PyIdentifier>());
                 }
                 if (py::isinstance<py::str>(other)) {
                     return self.str() != other.cast<std::string>();
                 }
                 return true;
             })
        .def_static(
            "__class_getitem__", [](const py::object &) { return py::type::of<PyIdentifier>(); }, py::arg("item"));
    py::implicitly_convertible<std::string, PyIdentifier>();

    // Submodules
    auto m_actor =
        m.def_submodule("actor", "Classes relating to actors (entities) that can exist in a world, including all "
                                 "players, monsters, projectiles, etc.");
    auto m_attribute = m.def_submodule("attribute", "Classes relevant to attributes.");
    auto m_ban = m.def_submodule("ban", "Classes relevant to bans.");
    auto m_block = m.def_submodule("block", "Classes relating to the blocks in a world, including special states.");
    auto m_boss =
        m.def_submodule("boss", "Classes relating to the boss bars that appear at the top of the player's screen.");
    auto m_command = m.def_submodule("command", "Classes relating to handling specialized non-chat player input.");
    auto m_damage =
        m.def_submodule("damage", "Classes relating to damage types and sources applicable to mobs (living entities).");
    auto m_effect = m.def_submodule("effect", "Classes relating to the effects that can be applied to entities.");
    auto m_enchantments =
        m.def_submodule("enchantments", "Classes relating to the specialized enhancements to ItemStacks.");
    auto m_event = m.def_submodule("event", "Classes relating to handling triggered code executions.");
    auto m_form = m.def_submodule("form");
    auto m_inventory = m.def_submodule("inventory", "Classes relating to player inventories and item interactions.");
    auto m_lang = m.def_submodule("lang");
    auto m_level = m.def_submodule("level");
    auto m_map = m.def_submodule("map", "Classes relating to plugin handling of map displays.");
    auto m_nbt = m.def_submodule("nbt", "Classes relating to the NBT data format.");
    auto m_permissions = m.def_submodule("permissions", "Classes relating to permissions of players.");
    auto m_plugin = m.def_submodule("plugin", "Classes relating to loading and managing plugins.");
    auto m_potion = m.def_submodule("potion", "Classes relating to potion effects.");
    auto m_scheduler =
        m.def_submodule("scheduler", "Classes relating to letting plugins run code at specific time intervals.");
    auto m_scoreboard =
        m.def_submodule("scoreboard", "Classes relating to manage the client side score display system.");
    auto m_util = m.def_submodule("util", "Multi and single purpose classes.");

    py::native_enum<EventPriority>(
        m_event, "EventPriority", "enum.IntEnum",
        "Listeners are called in following order: `LOWEST` -> `LOW` -> `NORMAL` -> `HIGH` -> `HIGHEST` -> `MONITOR`")
        .value("LOWEST", EventPriority::Lowest,
               "Event call is of very low importance and should be run first, to allow other plugins to further "
               "customise the outcome")
        .value("LOW", EventPriority::Low, "Event call is of low importance")
        .value("NORMAL", EventPriority::Normal,
               " Event call is neither important nor unimportant, and may be run normally")
        .value("HIGH", EventPriority::High, "Event call is of high importance")
        .value("HIGHEST", EventPriority::Highest,
               "Event call is critical and must have the final say in what happens to the event")
        .value("MONITOR", EventPriority::Monitor,
               "Event is listened to purely for monitoring the outcome of an event. No modifications to the event "
               "should be made under this priority.")
        .finalize();
    py::native_enum<PermissionDefault>(m_permissions, "PermissionDefault", "enum.Enum",
                                       "Represents the possible default values for permissions")
        .value("TRUE", PermissionDefault::True)
        .value("FALSE", PermissionDefault::False)
        .value("OP", PermissionDefault::Operator)
        .value("OPERATOR", PermissionDefault::Operator)
        .value("NOT_OP", PermissionDefault::NotOperator)
        .value("NOT_OPERATOR", PermissionDefault::NotOperator)
        .value("CONSOLE", PermissionDefault::Console)
        .finalize();
    py::native_enum<PermissionLevel>(m_permissions, "PermissionLevel", "enum.IntEnum")
        .value("DEFAULT", PermissionLevel::Default)
        .value("OP", PermissionLevel::Operator)
        .value("OPERATOR", PermissionLevel::Operator)
        .value("CONSOLE", PermissionLevel::Console)
        .finalize();

    // Forward declaration, see:
    // https://pybind11.readthedocs.io/en/stable/advanced/misc.html#avoiding-c-types-in-docstrings
    auto event = py::class_<Event, PyEvent>(m_event, "Event", "Represents an event.");
    auto permissible = py_class<Permissible>(
        m_permissions, "Permissible",
        "Represents an object that may become a server operator and can be assigned permissions.");
    auto permission = py::classh<Permission>(m_permissions, "Permission",
                                             "Represents a unique permission that may be attached to a `Permissible`");
    auto server = py::classh<Server>(m, "Server", "Represents a server implementation.");
    auto block = py::classh<Block>(m_block, "Block", "Represents a block.");
    auto command_sender = py_class<CommandSender>(m_command, "CommandSender", "Represents a command sender.");
    auto actor = py_class<Actor>(m_actor, "Actor", "Represents a base actor in the level.");
    auto mob =
        py_class<Mob>(m_actor, "Mob", "Represents a mobile entity (i.e. living entity), such as a monster or player.");
    py::classh<OfflinePlayer>(m, "OfflinePlayer", R"doc(
    Represents a reference to a player identity and the data belonging to a player that is stored on the disk
    and can, thus, be retrieved without the player needing to be online.
)doc")
        .def_property_readonly("name", &OfflinePlayer::getName, R"doc(
    The name of this player.

    Names are no longer unique past a single game session. For persistent storage it is recommended that
    you use `unique_id` instead. May be an empty string if we have not seen a name for this player yet.
)doc")
        .def_property_readonly("unique_id", &OfflinePlayer::getUniqueId, "The UUID of this player.");
    auto player = py_class<Player>(m, "Player", "Represents a player.");
    auto item_stack = py::class_<ItemStack>(m_inventory, "ItemStack", "Represents a stack of items.");
    auto level =
        py::classh<Level>(m_level, "Level", "Represents a level, which may contain actors, chunks and blocks.");
    auto dimension = py::classh<Dimension>(m_level, "Dimension", "Represents a dimension within a `Level`.");
    auto vector = py::class_<Vector>(m_util, "Vector", "Represents a 3-dimensional vector.");
    auto location =
        py::class_<Location>(m_level, "Location", "Represents a 3-dimensional location in a dimension within a level.");

    init_attribute(m_attribute);
    init_color_format(m);
    init_damage(m_damage);
    init_game_mode(m);
    init_logger(m);
    init_lang(m_lang);
    init_form(m_form);
    init_enchantments(m_enchantments);
    init_map(m_map);
    init_nbt(m_nbt);
    init_effect(m_effect);
    init_potion(m_potion);
    init_inventory(m_inventory, item_stack);
    init_util(m_util, vector);
    init_ban(m_ban);
    init_scoreboard(m_scoreboard);
    init_block(m_block, block);
    init_actor(m_actor, actor, mob);
    init_level(m_level, level, dimension, location);
    init_player(m, player);
    init_boss(m_boss);
    init_command(m_command, command_sender);
    init_plugin(m_plugin);
    init_scheduler(m_scheduler);
    init_permissions(m_permissions, permissible, permission);
    init_registry(m);
    init_server(server);
    init_event(m_event, event);

    m.attr("__minecraft_version__") = MINECRAFT_VERSION;
}

void init_color_format(py::module_ &m)
{
    py::class_<ColorFormat>(m, "ColorFormat", "All supported color and format codes.")
        .def_property_readonly_static(
            "BLACK", [](const py::object &) { return ColorFormat::Black; }, "Black (§0).")
        .def_property_readonly_static(
            "DARK_BLUE", [](const py::object &) { return ColorFormat::DarkBlue; }, "Dark blue (§1).")
        .def_property_readonly_static(
            "DARK_GREEN", [](const py::object &) { return ColorFormat::DarkGreen; }, "Dark green (§2).")
        .def_property_readonly_static(
            "DARK_AQUA", [](const py::object &) { return ColorFormat::DarkAqua; }, "Dark aqua (§3).")
        .def_property_readonly_static(
            "DARK_RED", [](const py::object &) { return ColorFormat::DarkRed; }, "Dark red (§4).")
        .def_property_readonly_static(
            "DARK_PURPLE", [](const py::object &) { return ColorFormat::DarkPurple; }, "Dark purple (§5).")
        .def_property_readonly_static(
            "GOLD", [](const py::object &) { return ColorFormat::Gold; }, "Gold (§6).")
        .def_property_readonly_static(
            "GRAY", [](const py::object &) { return ColorFormat::Gray; }, "Gray (§7).")
        .def_property_readonly_static(
            "DARK_GRAY", [](const py::object &) { return ColorFormat::DarkGray; }, "Dark gray (§8).")
        .def_property_readonly_static(
            "BLUE", [](const py::object &) { return ColorFormat::Blue; }, "Blue (§9).")
        .def_property_readonly_static(
            "GREEN", [](const py::object &) { return ColorFormat::Green; }, "Green (§a).")
        .def_property_readonly_static(
            "AQUA", [](const py::object &) { return ColorFormat::Aqua; }, "Aqua (§b).")
        .def_property_readonly_static(
            "RED", [](const py::object &) { return ColorFormat::Red; }, "Red (§c).")
        .def_property_readonly_static(
            "LIGHT_PURPLE", [](const py::object &) { return ColorFormat::LightPurple; }, "Light purple (§d).")
        .def_property_readonly_static(
            "YELLOW", [](const py::object &) { return ColorFormat::Yellow; }, "Yellow (§e).")
        .def_property_readonly_static(
            "WHITE", [](const py::object &) { return ColorFormat::White; }, "White (§f).")
        .def_property_readonly_static(
            "MINECOIN_GOLD", [](const py::object &) { return ColorFormat::MinecoinGold; }, "Minecoin gold (§g).")
        .def_property_readonly_static(
            "MATERIAL_QUARTZ", [](const py::object &) { return ColorFormat::MaterialQuartz; },
            "Quartz material color (§h).")
        .def_property_readonly_static(
            "MATERIAL_IRON", [](const py::object &) { return ColorFormat::MaterialIron; },
            "Iron material color (§i).")
        .def_property_readonly_static(
            "MATERIAL_NETHERITE", [](const py::object &) { return ColorFormat::MaterialNetherite; },
            "Netherite material color (§j).")
        .def_property_readonly_static(
            "MATERIAL_REDSTONE", [](const py::object &) { return ColorFormat::MaterialRedstone; },
            "Redstone material color (§m).")
        .def_property_readonly_static(
            "MATERIAL_COPPER", [](const py::object &) { return ColorFormat::MaterialCopper; },
            "Copper material color (§n).")
        .def_property_readonly_static(
            "MATERIAL_GOLD", [](const py::object &) { return ColorFormat::MaterialGold; },
            "Gold material color (§p).")
        .def_property_readonly_static(
            "MATERIAL_EMERALD", [](const py::object &) { return ColorFormat::MaterialEmerald; },
            "Emerald material color (§q).")
        .def_property_readonly_static(
            "MATERIAL_DIAMOND", [](const py::object &) { return ColorFormat::MaterialDiamond; },
            "Diamond material color (§s).")
        .def_property_readonly_static(
            "MATERIAL_LAPIS", [](const py::object &) { return ColorFormat::MaterialLapis; },
            "Lapis material color (§t).")
        .def_property_readonly_static(
            "MATERIAL_AMETHYST", [](const py::object &) { return ColorFormat::MaterialAmethyst; },
            "Amethyst material color (§u).")
        .def_property_readonly_static(
            "MATERIAL_RESIN", [](const py::object &) { return ColorFormat::MaterialResin; },
            "Resin material color (§v).")
        .def_property_readonly_static(
            "OBFUSCATED", [](const py::object &) { return ColorFormat::Obfuscated; },
            "Makes the text obfuscated, randomly cycling through characters (§k).")
        .def_property_readonly_static(
            "BOLD", [](const py::object &) { return ColorFormat::Bold; }, "Makes the text bold (§l).")
        .def_property_readonly_static(
            "ITALIC", [](const py::object &) { return ColorFormat::Italic; }, "Makes the text italic (§o).")
        .def_property_readonly_static(
            "RESET", [](const py::object &) { return ColorFormat::Reset; },
            "Resets all colors and formatting codes (§r).");
}

void init_game_mode(py::module_ &m)
{
    py::native_enum<GameMode>(m, "GameMode", "enum.Enum",
                              "Represents the various type of game modes that Players may have.")
        .value("SURVIVAL", GameMode::Survival)
        .value("CREATIVE", GameMode::Creative)
        .value("ADVENTURE", GameMode::Adventure)
        .value("SPECTATOR", GameMode::Spectator)
        .finalize();
}

void init_logger(py::module &m)
{
    auto logger = py::classh<Logger>(m, "Logger", "`Logger` class which can format and output varies levels of logs.");

    py::native_enum<Logger::Level>(logger, "Level", "enum.IntEnum", "Specifies the log level.")
        .value("TRACE", Logger::Level::Trace)
        .value("DEBUG", Logger::Level::Debug)
        .value("INFO", Logger::Level::Info)
        .value("WARNING", Logger::Level::Warning)
        .value("ERROR", Logger::Level::Error)
        .value("CRITICAL", Logger::Level::Critical)
        .export_values()
        .finalize();

    logger.def("set_level", &Logger::setLevel, py::arg("level"), R"doc(
    Set the logging level for this `Logger` instance.

    Args:
        level: The desired log level.
)doc")
        .def("is_enabled_for", &Logger::isEnabledFor, py::arg("level"), R"doc(
    Check if the `Logger` instance is enabled for the given log `Level`.

    Args:
        level: The log level to check.

    Returns:
        `True` if the logger is enabled for the `Level`, `False` otherwise.
)doc")
        .def(
            "trace", [](const Logger &self, const std::string_view message) { self.log(Logger::Trace, message); },
            py::arg("message"), "Log a message at the `TRACE` level.")
        .def(
            "debug", [](const Logger &self, const std::string_view message) { self.log(Logger::Debug, message); },
            py::arg("message"), "Log a message at the `DEBUG` level.")
        .def(
            "info", [](const Logger &self, const std::string_view message) { self.log(Logger::Info, message); },
            py::arg("message"), "Log a message at the `INFO` level.")
        .def(
            "warning", [](const Logger &self, const std::string_view message) { self.log(Logger::Warning, message); },
            py::arg("message"), "Log a message at the `WARNING` level.")
        .def(
            "error", [](const Logger &self, const std::string_view message) { self.log(Logger::Error, message); },
            py::arg("message"), "Log a message at the `ERROR` level.")
        .def(
            "critical", [](const Logger &self, const std::string_view message) { self.log(Logger::Critical, message); },
            py::arg("message"), "Log a message at the `CRITICAL` level.")
        .def_property_readonly("name", &Logger::getName, "The name of this `Logger` instance.");
}

void init_registry(py::module_ &m)
{
    py::class_<PyRegistry>(m, "Registry", R"doc(
    Abstract registry interface for objects with identifiers.

    Provides lookup by identifier (nullable or exception-throwing) and iteration.
)doc")
        .def("get", &PyRegistry::get, py::arg("id"), R"doc(
    Get the object by its identifier.

    Args:
        id: Identifier to look up.

    Returns:
        The object, or `None` if it does not exist.
)doc")
        .def("get_or_throw", &PyRegistry::getOrThrow, py::arg("id"), R"doc(
    Get the object by its identifier or raise if missing.

    Args:
        id: Identifier of the object to retrieve.

    Returns:
        The object with the given identifier.

    Raises:
        ValueError: If no object with the given identifier exists.
)doc")
        .def("__getitem__", &PyRegistry::getOrThrow, py::arg("id"))
        .def("__iter__", &PyRegistry::iter)
        .def("__contains__", &PyRegistry::contains, py::arg("id"))
        .def("__len__", &PyRegistry::size);
}

void init_server(py::classh<Server> &server)
{
    server.def_property_readonly("name", &Server::getName, "The name of this server implementation.")
        .def_property_readonly("version", &Server::getVersion, "The version string of this server implementation.")
        .def_property_readonly("minecraft_version", &Server::getMinecraftVersion,
                               "The Minecraft version that this server is running.")
        .def_property_readonly("protocol_version", &Server::getProtocolVersion,
                               "The network protocol version that this server supports.")
        .def_property_readonly("logger", &Server::getLogger, py::return_value_policy::reference,
                               "The primary logger associated with this server instance.")
        .def_property_readonly("language", &Server::getLanguage, py::return_value_policy::reference,
                               "The current language interface used by the server for translations.")
        .def_property_readonly("plugin_manager", &Server::getPluginManager, py::return_value_policy::reference,
                               "The plugin manager for interfacing with plugins.")
        .def("get_plugin_command", &Server::getPluginCommand, py::arg("name"), R"doc(
    Gets a `PluginCommand` with the given name or alias.

    Args:
        name: The name of the command to retrieve.

    Returns:
        A `PluginCommand` if found, `None` otherwise.
)doc")
        .def_property_readonly("command_sender", &Server::getCommandSender, py::return_value_policy::reference,
                               "A console `CommandSender` for this server.")
        .def("dispatch_command", &Server::dispatchCommand, py::arg("sender"), py::arg("command_line"), R"doc(
    Dispatches a command on this server, and executes it if found.

    Args:
        sender: The apparent sender of the command.
        command_line: The command + arguments.

    Returns:
        `True` if execution is successful, `False` otherwise.
)doc")
        .def_property_readonly("scheduler", &Server::getScheduler, py::return_value_policy::reference,
                               "The scheduler for managing scheduled events.")
        .def_property_readonly("service_manager", &Server::getServiceManager, py::return_value_policy::reference,
                               "The service manager.")
        .def(
            "get_registry",
            [](const Server &self, const py::type &t) -> py::object {
                auto *type_info = py::detail::get_type_info(reinterpret_cast<PyTypeObject *>(t.ptr()));
                if (!type_info) {
                    return py::none();
                }
                auto *registry = self._getRegistry(*type_info->cpptype);
                if (!registry) {
                    return py::none();
                }
                return py::cast(PyRegistry(*registry));
            },
            py::arg("type"), R"doc(
    Returns the registry for the given type.

    If no registry is present for the given type, `None` will be returned.

    Args:
        type: Type of the registry to get.

    Returns:
        The corresponding registry, or `None` if not present.
)doc")
        .def_property_readonly("level", &Server::getLevel, py::return_value_policy::reference_internal,
                               "The server level.")
        .def_property_readonly("online_players", &Server::getOnlinePlayers, py::return_value_policy::reference_internal,
                               "A list of all currently online players.")
        .def_property("max_players", &Server::getMaxPlayers, &Server::setMaxPlayers,
                      "The maximum amount of players which can login to this server.")
        .def("get_player", py::overload_cast<std::string>(&Server::getPlayer, py::const_), py::arg("name").noconvert(),
             R"doc(
    Gets the player with the exact given name, case-insensitive.

    Args:
        name: Exact name of the player to retrieve.

    Returns:
        A player object if one was found, `None` otherwise.
)doc")
        .def("get_player", py::overload_cast<UUID>(&Server::getPlayer, py::const_), py::arg("unique_id").noconvert(),
             R"doc(
    Gets the player with the given UUID.

    Args:
        unique_id: UUID of the player to retrieve.

    Returns:
        A player object if one was found, `None` otherwise.
)doc")
        .def_property_readonly("online_mode", &Server::getOnlineMode,
                               "Whether the Server is in online mode or not.")
        .def_property_readonly("port", &Server::getPort, "The game port that the server runs on.")
        .def_property_readonly("port_v6", &Server::getPortV6, "The game port (IPv6) that the server runs on.")
        .def("shutdown", &Server::shutdown, "Shutdowns the server, stopping everything.")
        .def("reload", &Server::reload, "Reloads the server configuration, functions, scripts and plugins.")
        .def("reload_data", &Server::reloadData, R"doc(
    Reload only the Minecraft data for the server.

    This includes functions and script files from all behaviour packs.
)doc")
        .def("broadcast", &Server::broadcast, py::arg("message"), py::arg("permission"), R"doc(
    Broadcasts the specified message to every user with the given permission name.

    Args:
        message: Message to broadcast.
        permission: The required permission Permissibles must have to receive the broadcast.
)doc")
        .def(
            "broadcast_message", [](const Server &self, const Message &message) { self.broadcastMessage(message); },
            py::arg("message"), R"doc(
    Broadcasts the specified message to every user with permission `endstone.broadcast.user`.

    Args:
        message: The message.
)doc")
        .def_property_readonly("item_factory", &Server::getItemFactory,
                               "The instance of the item factory (for `ItemMeta`).",
                               py::return_value_policy::reference)
        .def_property_readonly("scoreboard", &Server::getScoreboard, R"doc(
    The primary `Scoreboard` controlled by the server.

    This `Scoreboard` is saved by the server, is affected by the `/scoreboard` command, and is the
    scoreboard shown by default to players. This will only exist after the level has been loaded.
)doc")
        .def("create_scoreboard", &Server::createScoreboard, R"doc(
    Creates a new `Scoreboard` to be tracked by the server.

    This will not be saved by the server and is not affected by the `/scoreboard` command.

    Returns:
        The newly created `Scoreboard`.
)doc",
             py::return_value_policy::reference)
        .def_property_readonly("current_mspt", &Server::getCurrentMillisecondsPerTick,
                               "The current milliseconds per tick (MSPT).")
        .def_property_readonly("average_mspt", &Server::getAverageMillisecondsPerTick,
                               "The average milliseconds per tick (MSPT).")
        .def_property_readonly("current_tps", &Server::getCurrentTicksPerSecond,
                               "The current ticks per second (TPS).")
        .def_property_readonly("average_tps", &Server::getAverageTicksPerSecond,
                               "The average ticks per second (TPS).")
        .def_property_readonly("current_tick_usage", &Server::getCurrentTickUsage,
                               "The current tick usage of the server.")
        .def_property_readonly("average_tick_usage", &Server::getAverageTickUsage,
                               "The average tick usage of the server.")
        .def_property_readonly("start_time", &Server::getStartTime, "The start time of the server.")
        .def(
            "create_boss_bar",
            [](const Server &self, std::string title, BarColor color, BarStyle style,
               const std::optional<std::vector<BarFlag>> &flags) {
                return self.createBossBar(std::move(title), color, style, flags.value_or(std::vector<BarFlag>()));
            },
            py::arg("title"), py::arg("color"), py::arg("style"), py::arg("flags") = std::nullopt, R"doc(
    Creates a boss bar instance to display to players. The progress defaults to 1.0.

    Args:
        title: The title of the boss bar.
        color: The color of the boss bar.
        style: The style of the boss bar.
        flags: A list of flags to set on the boss bar.

    Returns:
        The created boss bar.
)doc")
        .def(
            "create_block_data",
            [](const Server &self, BlockTypeId type, const std::optional<BlockStates> &block_states) {
                return self.createBlockData(type, block_states.value_or(BlockStates{}));
            },
            py::arg("type"), py::arg("block_states") = std::nullopt, R"doc(
    Creates a new `BlockData` instance for the specified block type, with all properties initialized to defaults,
    except for those provided in `block_states`.

    Args:
        type: The block type.
        block_states: Block states, for example `{"old_leaf_type":"birch", "persistent_bit":True}`.

    Returns:
        New data instance.
)doc")
        .def_property_readonly("ban_list", &Server::getBanList, "The player ban list.",
                               py::return_value_policy::reference)
        .def_property_readonly("ip_ban_list", &Server::getIpBanList, "The IP ban list.",
                               py::return_value_policy::reference)
        .def("get_map", &Server::getMap, py::arg("id"), py::return_value_policy::reference, R"doc(
    Gets the map from the given item ID.

    Args:
        id: The id of the map to get.

    Returns:
        A map view if it exists, or `None` otherwise.
)doc")
        .def("create_map", &Server::createMap, py::arg("dimension"), py::return_value_policy::reference, R"doc(
    Create a new map with an automatically assigned ID.

    Args:
        dimension: The `Dimension` the map will belong to.

    Returns:
        A newly created map view.
)doc");
}

void init_player(py::module_ &m, py_class<Player> &player)
{
    py::class_<Skin>(m, "Skin", "Represents a player skin.")
        .def(py::init<std::string, Image, std::optional<std::string>, std::optional<Image>>(), py::arg("id"),
             py::arg("image"), py::arg("cape_id") = py::none(), py::arg("cape_image") = py::none())
        .def_property_readonly("id", &Skin::getId, "The skin id.")
        .def_property_readonly("image", &Skin::getImage, "The skin image.", py::return_value_policy::reference)
        .def_property_readonly("cape_id", &Skin::getCapeId, "The cape id.")
        .def_property_readonly("cape_image", &Skin::getCapeImage, "The cape image.",
                               py::return_value_policy::reference);

    player  //
        .def_property_readonly("name", &Player::getName, R"doc(
    The name of this player.

    May be an empty string if we have not seen a name for this player yet.
)doc")
        .def_property_readonly("unique_id", &Player::getUniqueId, "The UUID of this player.")
        .def_property("is_op", &Player::isOp, &Player::setOp, "Whether this player is a server operator.")
        .def_property_readonly("xuid", &Player::getXuid, "The Xbox User ID (XUID) of this player.")
        .def_property_readonly("address", &Player::getAddress, "The socket address of this player.")
        .def("transfer", &Player::transfer, py::arg("host"), py::arg("port") = 19132, R"doc(
    Transfers the player to another server.

    Args:
        host: Server address to transfer the player to.
        port: Server port to transfer the player to.
)doc")
        .def("kick", &Player::kick, py::arg("message"), R"doc(
    Kicks player with custom kick message.

    Args:
        message: Kick message.
)doc")
        .def("perform_command", &Player::performCommand, py::arg("command"), R"doc(
    Makes the player perform the given command.

    Args:
        command: Command to perform.

    Returns:
        `True` if the command was successful, `False` otherwise.
)doc")
        .def_property("is_sneaking", &Player::isSneaking, &Player::setSneaking,
                      "Whether the player is in sneak mode.")
        .def_property("is_sprinting", &Player::isSprinting, &Player::setSprinting,
                      "Whether the player is sprinting or not.")
        .def("play_sound", &Player::playSound, py::arg("location"), py::arg("sound"), py::arg("volume") = 1.0,
             py::arg("pitch") = 1.0, R"doc(
    Play a sound for a player at the location.

    Args:
        location: The location to play the sound.
        sound: The internal sound name to play.
        volume: The volume of the sound.
        pitch: The pitch of the sound.
)doc")
        .def("stop_sound", &Player::stopSound, py::arg("sound"), R"doc(
    Stop the specified sound from playing.

    Args:
        sound: The sound to stop.
)doc")
        .def("stop_all_sounds", &Player::stopAllSounds, "Stop all sounds from playing.")
        .def("send_popup", &Player::sendPopup, py::arg("message"), R"doc(
    Sends this player a popup message.

    Args:
        message: Message to be displayed.
)doc")
        .def("send_tip", &Player::sendTip, py::arg("message"), R"doc(
    Sends this player a tip message.

    Args:
        message: Message to be displayed.
)doc")
        .def("send_toast", &Player::sendToast, py::arg("title"), py::arg("content"), R"doc(
    Sends this player a toast notification.

    Args:
        title: The title of the toast notification.
        content: The content of the toast notification.
)doc")
        .def("give_exp", &Player::giveExp, py::arg("amount"), R"doc(
    Gives the player the amount of experience specified.

    Args:
        amount: Exp amount to give.
)doc")
        .def("give_exp_levels", &Player::giveExpLevels, py::arg("amount"), R"doc(
    Gives the player the amount of experience levels specified.

    Args:
        amount: Amount of experience levels to give or take.
)doc")
        .def_property("exp_progress", &Player::getExpProgress, &Player::setExpProgress, R"doc(
    The player's current experience progress towards the next level.

    This is a percentage value. 0.0 is "no progress" and 1.0 is "next level".
)doc")
        .def_property("exp_level", &Player::getExpLevel, &Player::setExpLevel,
                      "The player's current experience level.")
        .def_property_readonly("total_exp", &Player::getTotalExp, R"doc(
    The player's total experience points.

    This refers to the total amount of experience the player has collected over time and is not currently
    displayed to the client.
)doc")
        .def_property("is_flying", &Player::isFlying, &Player::setFlying,
                      "Whether the player is currently flying or not.")
        .def_property("allow_flight", &Player::getAllowFlight, &Player::setAllowFlight,
                      "Whether the Player is allowed to fly via jump key double-tap.")
        .def_property("fly_speed", &Player::getFlySpeed, &Player::setFlySpeed,
                      "The current allowed speed that a client can fly. Default is 0.05.")
        .def_property("walk_speed", &Player::getWalkSpeed, &Player::setWalkSpeed,
                      "The current allowed speed that a client can walk. Default is 0.10.")
        .def_property("scoreboard", &Player::getScoreboard, &Player::setScoreboard,
                      "The player's visible `Scoreboard`.", py::return_value_policy::reference)
        .def("send_title", py::overload_cast<std::string, std::string, int, int, int>(&Player::sendTitle, py::const_),
             py::arg("title"), py::arg("subtitle"), py::arg("fade_in") = 10, py::arg("stay") = 70,
             py::arg("fade_out") = 20, R"doc(
    Sends a title and a subtitle message to the player.

    If they are empty strings, the display will be updated as such.

    Args:
        title: Title text.
        subtitle: Subtitle text.
        fade_in: Time in ticks for titles to fade in. Defaults to 10.
        stay: Time in ticks for titles to stay. Defaults to 70.
        fade_out: Time in ticks for titles to fade out. Defaults to 20.
)doc")
        .def("reset_title", &Player::resetTitle, R"doc(
    Resets the title displayed to the player.

    This will clear the displayed title / subtitle and reset timings to their default values.
)doc")
        .def("spawn_particle",
             py::overload_cast<std::string, Location, std::optional<std::string>>(&Player::spawnParticle, py::const_),
             py::arg("name"), py::arg("location").noconvert(),
             py::arg("molang_variables_json") = std::nullopt, R"doc(
    Spawns the particle at the target location.

    Args:
        name: The name of the particle effect to spawn.
        location: The location to spawn at.
        molang_variables_json: The customizable molang variables that can be adjusted for this particle, in json.
)doc")
        .def("spawn_particle",
             py::overload_cast<std::string, float, float, float, std::optional<std::string>>(&Player::spawnParticle,
                                                                                             py::const_),
             py::arg("name"), py::arg("x"), py::arg("y"), py::arg("z"),
             py::arg("molang_variables_json") = std::nullopt, R"doc(
    Spawns the particle at the target location.

    Args:
        name: The name of the particle effect to spawn.
        x: The position on the x axis to spawn at.
        y: The position on the y axis to spawn at.
        z: The position on the z axis to spawn at.
        molang_variables_json: The customizable molang variables that can be adjusted for this particle, in json.
)doc")
        .def_property_readonly(
            "ping", [](const Player &self) { return self.getPing().count(); },
            "The player's average ping in milliseconds.")
        .def("update_commands", &Player::updateCommands, R"doc(
    Send the list of commands to the client.

    Generally useful to ensure the client has a complete list of commands after permission changes are done.
)doc")

        .def_property("game_mode", &Player::getGameMode, &Player::setGameMode, "The player's current `GameMode`.")
        .def_property_readonly("inventory", &Player::getInventory, py::return_value_policy::reference,
                               "The player's inventory. This also contains the armor slots.")
        .def_property_readonly("ender_chest", &Player::getEnderChest, py::return_value_policy::reference,
                               "The player's `EnderChest` inventory.")
        .def_property_readonly("locale", &Player::getLocale, "The player's current locale.")
        .def_property_readonly("device_os", &Player::getDeviceOS,
                               "The player's current device's operation system (OS).")
        .def_property_readonly("device_id", &Player::getDeviceId, "The player's current device id.")
        .def_property_readonly("game_version", &Player::getGameVersion, "The player's current game version.")
        .def_property_readonly("skin", &Player::getSkin, "The player's skin.")
        .def("send_form", &Player::sendForm, py::arg("form"), R"doc(
    Sends a form to the player.

    Args:
        form: The form to send.
)doc")
        .def("close_form", &Player::closeForm, "Closes the forms that are currently open for the player.")
        .def("send_map", &Player::sendMap, py::arg("map"), R"doc(
    Render a map and send it to the player in its entirety.

    This may be used when streaming the map in the normal manner is not desirable.

    Args:
        map: The map to send.
)doc")
        .def(
            "send_packet",
            [](const Player &self, const int packet_id, const py::bytes &payload) {
                return self.sendPacket(packet_id, payload);
            },
            py::arg("packet_id"), py::arg("payload"), R"doc(
    Sends a packet to the player.

    Args:
        packet_id: The packet ID to be sent.
        payload: The payload of the packet to be transmitted.
)doc");
}

}  // namespace endstone::python
