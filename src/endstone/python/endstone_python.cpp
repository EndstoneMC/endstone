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
#include "registry.h"
#include "type_caster.h"

namespace py = pybind11;

namespace endstone::python {
void init_actor(py::module_ &, py::class_<Actor, CommandSender> &actor, py::class_<Mob, Actor> &mob);
void init_attribute(py::module_ &);
void init_ban(py::module_ &);
void init_block(py::module_ &, py::class_<Block> &block);
void init_boss(py::module_ &);
void init_color_format(py::module_ &);
void init_command(py::module &, py::class_<CommandSender, Permissible> &command_sender);
void init_damage(py::module_ &);
void init_effect(py::module_ &);
void init_enchantments(py::module_ &);
void init_event(py::module_ &, py::class_<Event> &event);
void init_form(py::module_ &);
void init_game_mode(py::module_ &);
void init_inventory(py::module_ &, py::class_<ItemStack> &item_stack);
void init_lang(py::module_ &);
void init_level(py::module_ &, py::class_<Level> &level, py::class_<Dimension> &dimension,
                py::class_<Location> &location);
void init_logger(py::module_ &);
void init_map(py::module_ &);
void init_nbt(py::module_ &);
void init_permissions(py::module_ &, py::class_<Permissible> &permissible, py::class_<Permission> &permission);
void init_player(py::module_ &, py::class_<OfflinePlayer> &offline_player,
                 py::class_<Player, Mob, OfflinePlayer> &player);
void init_plugin(py::module_ &);
void init_potion(py::module_ &);
void init_registry(py::module_ &);
void init_scheduler(py::module_ &);
void init_scoreboard(py::module_ &);
void init_server(py::class_<Server> &server);
void init_util(py::module_ &, py::class_<Vector> &);

PYBIND11_MODULE(_python, m)  // NOLINT(*-use-anonymous-namespace)
{
    py::options options;
    options.disable_enum_members_docstring();

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
        "Listeners are called in following order: LOWEST -> LOW -> NORMAL -> HIGH -> HIGHEST -> MONITOR")
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
    auto event = py::class_<Event>(m_event, "Event", "Represents an event.");
    auto permissible = py::class_<Permissible>(
        m_permissions, "Permissible",
        "Represents an object that may become a server operator and can be assigned permissions.");
    auto permission = py::class_<Permission>(m_permissions, "Permission",
                                             "Represents a unique permission that may be attached to a Permissible");
    auto server = py::class_<Server>(m, "Server", "Represents a server implementation.");
    auto block = py::class_<Block>(m_block, "Block", "Represents a block.");
    auto command_sender =
        py::class_<CommandSender, Permissible>(m_command, "CommandSender", "Represents a command sender.");
    auto actor = py::class_<Actor, CommandSender>(m_actor, "Actor", "Represents a base actor in the level.");
    auto mob = py::class_<Mob, Actor>(m_actor, "Mob",
                                      "Represents a mobile entity (i.e. living entity), such as a monster or player.");
    auto offline_player = py::class_<OfflinePlayer>(
        m, "OfflinePlayer",
        "Represents a reference to a player identity and the data belonging to a player that is stored on the disk and "
        "can, thus, be retrieved without the player needing to be online.");
    auto player = py::class_<Player, Mob, OfflinePlayer>(m, "Player", "Represents a player.");
    auto item_stack = py::class_<ItemStack>(m_inventory, "ItemStack", "Represents a stack of items.");
    auto level = py::class_<Level>(m_level, "Level");
    auto dimension = py::class_<Dimension>(m_level, "Dimension", "Represents a dimension within a Level.");
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
    init_player(m, offline_player, player);
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
        .def_property_readonly_static("BLACK", [](const py::object &) { return ColorFormat::Black; })
        .def_property_readonly_static("DARK_BLUE", [](const py::object &) { return ColorFormat::DarkBlue; })
        .def_property_readonly_static("DARK_GREEN", [](const py::object &) { return ColorFormat::DarkGreen; })
        .def_property_readonly_static("DARK_AQUA", [](const py::object &) { return ColorFormat::DarkAqua; })
        .def_property_readonly_static("DARK_RED", [](const py::object &) { return ColorFormat::DarkRed; })
        .def_property_readonly_static("DARK_PURPLE", [](const py::object &) { return ColorFormat::DarkPurple; })
        .def_property_readonly_static("GOLD", [](const py::object &) { return ColorFormat::Gold; })
        .def_property_readonly_static("GRAY", [](const py::object &) { return ColorFormat::Gray; })
        .def_property_readonly_static("DARK_GRAY", [](const py::object &) { return ColorFormat::DarkGray; })
        .def_property_readonly_static("BLUE", [](const py::object &) { return ColorFormat::Blue; })
        .def_property_readonly_static("GREEN", [](const py::object &) { return ColorFormat::Green; })
        .def_property_readonly_static("AQUA", [](const py::object &) { return ColorFormat::Aqua; })
        .def_property_readonly_static("RED", [](const py::object &) { return ColorFormat::Red; })
        .def_property_readonly_static("LIGHT_PURPLE", [](const py::object &) { return ColorFormat::LightPurple; })
        .def_property_readonly_static("YELLOW", [](const py::object &) { return ColorFormat::Yellow; })
        .def_property_readonly_static("WHITE", [](const py::object &) { return ColorFormat::White; })
        .def_property_readonly_static("MINECOIN_GOLD", [](const py::object &) { return ColorFormat::MinecoinGold; })
        .def_property_readonly_static("MATERIAL_QUARTZ", [](const py::object &) { return ColorFormat::MaterialQuartz; })
        .def_property_readonly_static("MATERIAL_IRON", [](const py::object &) { return ColorFormat::MaterialIron; })
        .def_property_readonly_static("MATERIAL_NETHERITE",
                                      [](const py::object &) { return ColorFormat::MaterialNetherite; })
        .def_property_readonly_static("MATERIAL_REDSTONE",
                                      [](const py::object &) { return ColorFormat::MaterialRedstone; })
        .def_property_readonly_static("MATERIAL_COPPER", [](const py::object &) { return ColorFormat::MaterialCopper; })
        .def_property_readonly_static("MATERIAL_GOLD", [](const py::object &) { return ColorFormat::MaterialGold; })
        .def_property_readonly_static("MATERIAL_EMERALD",
                                      [](const py::object &) { return ColorFormat::MaterialEmerald; })
        .def_property_readonly_static("MATERIAL_DIAMOND",
                                      [](const py::object &) { return ColorFormat::MaterialDiamond; })
        .def_property_readonly_static("MATERIAL_LAPIS", [](const py::object &) { return ColorFormat::MaterialLapis; })
        .def_property_readonly_static("MATERIAL_AMETHYST",
                                      [](const py::object &) { return ColorFormat::MaterialAmethyst; })
        .def_property_readonly_static("MATERIAL_RESIN", [](const py::object &) { return ColorFormat::MaterialResin; })
        .def_property_readonly_static("OBFUSCATED", [](const py::object &) { return ColorFormat::Obfuscated; })
        .def_property_readonly_static("BOLD", [](const py::object &) { return ColorFormat::Bold; })
        .def_property_readonly_static("ITALIC", [](const py::object &) { return ColorFormat::Italic; })
        .def_property_readonly_static("RESET", [](const py::object &) { return ColorFormat::Reset; });
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
    auto logger = py::class_<Logger>(m, "Logger", "Logger class which can format and output varies levels of logs.");

    py::native_enum<Logger::Level>(logger, "Level", "enum.IntEnum", "Specifies the log level.")
        .value("TRACE", Logger::Level::Trace)
        .value("DEBUG", Logger::Level::Debug)
        .value("INFO", Logger::Level::Info)
        .value("WARNING", Logger::Level::Warning)
        .value("ERROR", Logger::Level::Error)
        .value("CRITICAL", Logger::Level::Critical)
        .export_values()
        .finalize();

    logger.def("set_level", &Logger::setLevel, py::arg("level"), "Set the logging level for this Logger instance.")
        .def("is_enabled_for", &Logger::isEnabledFor, py::arg("level"),
             "Check if the Logger instance is enabled for the given log Level.")
        .def(
            "trace", [](const Logger &self, const std::string_view message) { self.log(Logger::Trace, message); },
            py::arg("message"), "Log a message at the TRACE level.")
        .def(
            "debug", [](const Logger &self, const std::string_view message) { self.log(Logger::Debug, message); },
            py::arg("message"), "Log a message at the DEBUG level.")
        .def(
            "info", [](const Logger &self, const std::string_view message) { self.log(Logger::Info, message); },
            py::arg("message"), "Log a message at the INFO level.")
        .def(
            "warning", [](const Logger &self, const std::string_view message) { self.log(Logger::Warning, message); },
            py::arg("message"), "Log a message at the WARNING level.")
        .def(
            "error", [](const Logger &self, const std::string_view message) { self.log(Logger::Error, message); },
            py::arg("message"), "Log a message at the ERROR level.")
        .def(
            "critical", [](const Logger &self, const std::string_view message) { self.log(Logger::Critical, message); },
            py::arg("message"), "Log a message at the CRITICAL level.")
        .def_property_readonly("name", &Logger::getName, "Get the name of this Logger instance.");
}

void init_registry(py::module_ &m)
{
    bind_registries(m, RegistryTypes{});
}

void init_server(py::class_<Server> &server)
{
    server.def_property_readonly("name", &Server::getName, "Gets the name of this server implementation.")
        .def_property_readonly("version", &Server::getVersion, "Gets the version of this server implementation.")
        .def_property_readonly("minecraft_version", &Server::getMinecraftVersion,
                               "Gets the Minecraft version that this server is running.")
        .def_property_readonly("protocol_version", &Server::getProtocolVersion,
                               "Gets the network protocol version that this server supports.")
        .def_property_readonly("logger", &Server::getLogger, py::return_value_policy::reference,
                               "Returns the primary logger associated with this server instance.")
        .def_property_readonly("language", &Server::getLanguage, py::return_value_policy::reference,
                               "Gets the current language interface used by the server.")
        .def_property_readonly("plugin_manager", &Server::getPluginManager, py::return_value_policy::reference,
                               "Gets the plugin manager for interfacing with plugins.")
        .def("get_plugin_command", &Server::getPluginCommand, py::arg("name"), py::return_value_policy::reference,
             "Gets a PluginCommand with the given name or alias.")
        .def_property_readonly("command_sender", &Server::getCommandSender, py::return_value_policy::reference,
                               "Gets a CommandSender for this server.")
        .def("dispatch_command", &Server::dispatchCommand, py::arg("sender"), py::arg("command_line"),
             "Dispatches a command on this server, and executes it if found.")
        .def_property_readonly("scheduler", &Server::getScheduler, py::return_value_policy::reference,
                               "Gets the scheduler for managing scheduled events.")
        .def_property_readonly("service_manager", &Server::getServiceManager, py::return_value_policy::reference,
                               "Gets the service manager.")
        .def(
            "get_registry",
            [](const Server &self, const py::type &t) -> py::object { return get_registry(self, t, RegistryTypes{}); },
            py::arg("type"), py::return_value_policy::reference, "Returns the registry for all the enchantments.")
        .def_property_readonly("level", &Server::getLevel, py::return_value_policy::reference_internal,
                               "Gets the server level.")
        .def_property_readonly("online_players", &Server::getOnlinePlayers, py::return_value_policy::reference_internal,
                               "Gets a list of all currently online players.")
        .def_property("max_players", &Server::getMaxPlayers, &Server::setMaxPlayers,
                      "The maximum amount of players which can login to this server.")
        .def("get_player", py::overload_cast<std::string>(&Server::getPlayer, py::const_), py::arg("name").noconvert(),
             py::return_value_policy::reference, "Gets the player with the exact given name, case insensitive.")
        .def("get_player", py::overload_cast<UUID>(&Server::getPlayer, py::const_), py::arg("unique_id").noconvert(),
             py::return_value_policy::reference, "Gets the player with the given UUID.")
        .def_property_readonly("online_mode", &Server::getOnlineMode,
                               "Gets whether the Server is in online mode or not.")
        .def_property_readonly("port", &Server::getPort, "Get the game port that the server runs on.")
        .def_property_readonly("port_v6", &Server::getPortV6, "Get the game port (IPv6) that the server runs on.")
        .def("shutdown", &Server::shutdown, "Shutdowns the server, stopping everything.")
        .def("reload", &Server::reload, "Reloads the server configuration, functions, scripts and plugins.")
        .def("reload_data", &Server::reloadData, "Reload only the Minecraft data for the server.")
        .def("broadcast", &Server::broadcast, py::arg("message"), py::arg("permission"),
             "Broadcasts the specified message to every user with the given permission name.")
        .def(
            "broadcast_message", [](const Server &self, const Message &message) { self.broadcastMessage(message); },
            py::arg("message"),
            "Broadcasts the specified message to every user with permission endstone.broadcast.user")
        .def_property_readonly("item_factory", &Server::getItemFactory,
                               "Gets the instance of the item factory (for ItemMeta).",
                               py::return_value_policy::reference)
        .def_property_readonly("scoreboard", &Server::getScoreboard,
                               "Gets the primary Scoreboard controlled by the server.",
                               py::return_value_policy::reference)
        .def("create_scoreboard", &Server::createScoreboard, "Creates a new Scoreboard to be tracked by the server.",
             py::return_value_policy::reference)
        .def_property_readonly("current_mspt", &Server::getCurrentMillisecondsPerTick,
                               "Gets the current milliseconds per tick (MSPT).")
        .def_property_readonly("average_mspt", &Server::getAverageMillisecondsPerTick,
                               "Gets the average milliseconds per tick (MSPT).")
        .def_property_readonly("current_tps", &Server::getCurrentTicksPerSecond,
                               "Gets the current ticks per second (TPS).")
        .def_property_readonly("average_tps", &Server::getAverageTicksPerSecond,
                               "Gets the average ticks per second (TPS).")
        .def_property_readonly("current_tick_usage", &Server::getCurrentTickUsage,
                               "Gets the current tick usage of the server.")
        .def_property_readonly("average_tick_usage", &Server::getAverageTickUsage,
                               "Gets the average tick usage of the server.")
        .def_property_readonly("start_time", &Server::getStartTime, "Gets the start time of the server.")
        .def(
            "create_boss_bar",
            [](const Server &self, std::string title, BarColor color, BarStyle style,
               const std::optional<std::vector<BarFlag>> &flags) {
                return self.createBossBar(std::move(title), color, style, flags.value_or(std::vector<BarFlag>()));
            },
            py::arg("title"), py::arg("color"), py::arg("style"), py::arg("flags") = std::nullopt,
            "Creates a boss bar instance to display to players. The progress defaults to 1.0.")
        .def(
            "create_block_data",
            [](const Server &self, std::string type, const std::optional<BlockStates> &block_states) {
                return self.createBlockData(std::move(type), block_states.value_or(BlockStates{}));
            },
            py::arg("type"), py::arg("block_states") = std::nullopt,
            "Creates a new BlockData instance for the specified block type, with all properties initialized to "
            "defaults, except for those provided.")
        .def_property_readonly("ban_list", &Server::getBanList, "Gets the player ban list.",
                               py::return_value_policy::reference)
        .def_property_readonly("ip_ban_list", &Server::getIpBanList, "Gets the IP ban list.",
                               py::return_value_policy::reference)
        .def("get_map", &Server::getMap, "Gets the map from the given map ID.", py::arg("id"),
             py::return_value_policy::reference)
        .def("create_map", &Server::createMap, "Create a new map with an automatically assigned ID.",
             py::arg("dimension"), py::return_value_policy::reference);
}

void init_player(py::module_ &m, py::class_<OfflinePlayer> &offline_player,
                 py::class_<Player, Mob, OfflinePlayer> &player)
{
    py::class_<Skin>(m, "Skin", "Represents a player skin.")
        .def(py::init<std::string, Image, std::optional<std::string>, std::optional<Image>>(), py::arg("id"),
             py::arg("image"), py::arg("cape_id") = py::none(), py::arg("cape_image") = py::none())
        .def_property_readonly("id", &Skin::getId, "Get the Skin ID.")
        .def_property_readonly("image", &Skin::getImage, "Get the Skin image.", py::return_value_policy::reference)
        .def_property_readonly("cape_id", &Skin::getCapeId, "Get the Cape ID.")
        .def_property_readonly("cape_image", &Skin::getCapeImage, "Get the Cape image.",
                               py::return_value_policy::reference);

    offline_player  //
        .def_property_readonly("name", &OfflinePlayer::getName, "Returns the name of this player")
        .def_property_readonly("unique_id", &OfflinePlayer::getUniqueId, "Returns the UUID of this player");

    player  //
        .def_property_readonly("name", &OfflinePlayer::getName, "Returns the name of this player")
        .def_property_readonly("unique_id", &Player::getUniqueId, "Returns the UUID of this player")
        .def_property("is_op", &Player::isOp, &Player::setOp, "The operator status of this playerall")
        .def_property_readonly("xuid", &Player::getXuid, "Returns the Xbox User ID (XUID) of this player")
        .def_property_readonly("address", &Player::getAddress, "Gets the socket address of this player")
        .def("transfer", &Player::transfer, "Transfers the player to another server.", py::arg("host"),
             py::arg("port") = 19132)
        .def("kick", &Player::kick, py::arg("message"), "Kicks player with custom kick message.")
        .def("perform_command", &Player::performCommand, py::arg("command"),
             "Makes the player perform the given command.")
        .def_property("is_sneaking", &Player::isSneaking, &Player::setSneaking,
                      "Gets or sets the sneak mode of the player")
        .def_property("is_sprinting", &Player::isSprinting, &Player::setSprinting,
                      "Gets or sets whether the player is sprinting or not.")
        .def("play_sound", &Player::playSound, py::arg("location"), py::arg("sound"), py::arg("volume") = 1.0,
             py::arg("pitch") = 1.0, "Play a sound for a player at the location.")
        .def("stop_sound", &Player::stopSound, py::arg("sound"), "Stop the specified sound from playing.")
        .def("stop_all_sounds", &Player::stopAllSounds, "Stop all sounds from playing.")
        .def("send_popup", &Player::sendPopup, py::arg("message"), "Sends this player a popup message")
        .def("send_tip", &Player::sendTip, py::arg("message"), "Sends this player a tip message")
        .def("send_toast", &Player::sendToast, py::arg("title"), py::arg("content"),
             "Sends this player a toast notification.")
        .def("give_exp", &Player::giveExp, py::arg("amount"), "Gives the player the amount of experience specified.")
        .def("give_exp_levels", &Player::giveExpLevels, py::arg("amount"),
             "Gives the player the amount of experience levels specified.")
        .def_property("exp_progress", &Player::getExpProgress, &Player::setExpProgress,
                      "Gets or sets the players current experience progress towards the next level.")
        .def_property("exp_level", &Player::getExpLevel, &Player::setExpLevel,
                      "Gets or sets the players current experience level.")
        .def_property_readonly("total_exp", &Player::getTotalExp, "Gets the players total experience points.")
        .def_property("is_flying", &Player::isFlying, &Player::setFlying, "If the player is currently flying or not.")
        .def_property("allow_flight", &Player::getAllowFlight, &Player::setAllowFlight,
                      "If the Player is allowed to fly via jump key double-tap.")
        .def_property("fly_speed", &Player::getFlySpeed, &Player::setFlySpeed,
                      "Gets or sets the current allowed speed that a client can fly.")
        .def_property("walk_speed", &Player::getWalkSpeed, &Player::setWalkSpeed,
                      "Gets or sets the current allowed speed that a client can walk.")
        .def_property("scoreboard", &Player::getScoreboard, &Player::setScoreboard,
                      "Gets or sets the player's visible Scoreboard.", py::return_value_policy::reference)
        .def("send_title", py::overload_cast<std::string, std::string, int, int, int>(&Player::sendTitle, py::const_),
             "Sends a title and a subtitle message to the player. If they are empty strings, the display will be "
             "updated as such.",
             py::arg("title"), py::arg("subtitle"), py::arg("fade_in") = 10, py::arg("stay") = 70,
             py::arg("fade_out") = 20)
        .def("reset_title", &Player::resetTitle,
             "Resets the title displayed to the player. This will clear the displayed title / subtitle and reset "
             "timings to their default values.")
        .def("spawn_particle",
             py::overload_cast<std::string, Location, std::optional<std::string>>(&Player::spawnParticle, py::const_),
             "Spawns the particle at the target location.", py::arg("name"), py::arg("location").noconvert(),
             py::arg("molang_variables_json") = std::nullopt)
        .def("spawn_particle",
             py::overload_cast<std::string, float, float, float, std::optional<std::string>>(&Player::spawnParticle,
                                                                                             py::const_),
             "Spawns the particle at the target location.", py::arg("name"), py::arg("x"), py::arg("y"), py::arg("z"),
             py::arg("molang_variables_json") = std::nullopt)
        .def_property_readonly(
            "ping", [](const Player &self) { return self.getPing().count(); },
            "Gets the player's average ping in milliseconds.")
        .def("update_commands", &Player::updateCommands, "Send the list of commands to the client.")

        .def_property("game_mode", &Player::getGameMode, &Player::setGameMode, "The player's current game mode.")
        .def_property_readonly("inventory", &Player::getInventory, py::return_value_policy::reference,
                               "Get the player's inventory.")
        .def_property_readonly("ender_chest", &Player::getEnderChest, py::return_value_policy::reference,
                               "Get the player's EnderChest inventory.")
        .def_property_readonly("locale", &Player::getLocale, "Get the player's current locale.")
        .def_property_readonly("device_os", &Player::getDeviceOS,
                               "Get the player's current device's operation system (OS).")
        .def_property_readonly("device_id", &Player::getDeviceId, "Get the player's current device id.")
        .def_property_readonly("game_version", &Player::getGameVersion, "Get the player's current game version.")
        .def_property_readonly("skin", &Player::getSkin, "Get the player's skin.")
        .def("send_form", &Player::sendForm, "Sends a form to the player.", py::arg("form"))
        .def("close_form", &Player::closeForm, "Closes the forms that are currently open for the player.")
        .def(
            "send_packet",
            [](const Player &self, const int packet_id, const py::bytes &payload) {
                return self.sendPacket(packet_id, payload);
            },
            py::arg("packet_id"), py::arg("payload"), "Sends a packet to the player.");
}

}  // namespace endstone::python
