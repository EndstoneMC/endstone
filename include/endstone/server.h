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

#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "endstone/ban/ip_ban_list.h"
#include "endstone/ban/player_ban_list.h"
#include "endstone/block/block_data.h"
#include "endstone/boss/boss_bar.h"
#include "endstone/lang/language.h"
#include "endstone/level/level.h"
#include "endstone/logger.h"
#include "endstone/map/map_view.h"
#include "endstone/player.h"
#include "endstone/plugin/service_manager.h"
#include "endstone/scoreboard/scoreboard.h"
#include "endstone/util/result.h"
#include "endstone/util/uuid.h"

namespace endstone {

class ConsoleCommandSender;
class Enchantment;
class ItemFactory;
class ItemType;
class IRegistry;
class Scheduler;
class PluginCommand;
class PluginManager;

template <typename T>
class Registry;

/**
 * @brief Represents a server implementation.
 */
class Server {
public:
    Server() = default;
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

    virtual ~Server() = default;

    /**
     * @brief Gets the name of this server implementation.
     *
     * @return name of this server implementation
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * @brief Gets the version string of this server implementation.
     *
     * @return version of this server implementation
     */
    [[nodiscard]] virtual std::string getVersion() const = 0;

    /**
     * @brief Gets the Minecraft version that this server is running.
     *
     * @return version of Minecraft
     */
    [[nodiscard]] virtual std::string getMinecraftVersion() const = 0;

    /**
     * @brief Gets the network protocol version that this server supports.
     *
     * @return version of network protocol
     */
    [[nodiscard]] virtual int getProtocolVersion() const = 0;

    /**
     * @brief Returns the primary logger associated with this server instance.
     *
     * @return Logger associated with this server
     */
    [[nodiscard]] virtual Logger &getLogger() const = 0;

    /**
     * @brief Gets the current language interface used by the server.
     *
     * @return Language interface used by the server for translations.
     */
    [[nodiscard]] virtual Language &getLanguage() const = 0;

    /**
     * @brief Gets the plugin manager for interfacing with plugins.
     *
     * @return a plugin manager for this Server instance
     */
    [[nodiscard]] virtual PluginManager &getPluginManager() const = 0;

    /**
     * @brief Gets a PluginCommand with the given name or alias.
     *
     * @param name the name of the command to retrieve
     * @return a plugin command if found, null otherwise
     */
    [[nodiscard]] virtual PluginCommand *getPluginCommand(std::string name) const = 0;

    /**
     * @brief Gets a CommandSender for this server.
     *
     * @return a console command sender
     */
    [[nodiscard]] virtual ConsoleCommandSender &getCommandSender() const = 0;

    /**
     * @brief Dispatches a command on this server, and executes it if found.
     *
     * @param sender the apparent sender of the command
     * @param command_line the command + arguments.
     * @return true if execution is successful, false otherwise
     */
    [[nodiscard]] virtual bool dispatchCommand(CommandSender &sender, std::string command_line) const = 0;

    /**
     * @brief Gets the scheduler for managing scheduled events.
     *
     * @return a scheduling service for this server
     */
    [[nodiscard]] virtual Scheduler &getScheduler() const = 0;

    /**
     * @brief Gets the server level.
     *
     * @return the server level
     */
    [[nodiscard]] virtual Level *getLevel() const = 0;

    /**
     * @brief Gets a list of all currently online players.
     *
     * @return a list of currently online players.
     */
    [[nodiscard]] virtual std::vector<Player *> getOnlinePlayers() const = 0;

    /**
     * @brief Get the maximum amount of players which can login to this server.
     *
     * @return the amount of players this server allows
     */
    [[nodiscard]] virtual int getMaxPlayers() const = 0;

    /**
     * @brief  Set the maximum amount of players allowed to be logged in at once.
     *
     * @param max_players The maximum amount of concurrent players
     */
    virtual void setMaxPlayers(int max_players) = 0;

    /**
     * @brief Gets the player with the given UUID.
     *
     * @param id UUID of the player to retrieve
     * @return a player object if one was found, null otherwise
     */
    [[nodiscard]] virtual Player *getPlayer(endstone::UUID id) const = 0;

    /**
     * @brief Get the game port that the server runs on.
     *
     * @return the port number of this server
     */
    [[nodiscard]] virtual int getPort() const = 0;

    /**
     * @brief Get the game port (IPv6) that the server runs on.
     *
     * @return the port number of this server
     */
    [[nodiscard]] virtual int getPortV6() const = 0;

    /**
     * @brief Gets whether the Server is in online mode or not.
     *
     * @return true if the server authenticates clients, false otherwise
     */
    [[nodiscard]] virtual bool getOnlineMode() const = 0;

    /**
     * @brief Gets the player with the exact given name, case-insensitive.
     *
     * @param name Exact name of the player to retrieve
     * @return a player object if one was found, null otherwise
     */
    [[nodiscard]] virtual Player *getPlayer(std::string name) const = 0;

    /**
     * @brief Shutdowns the server, stopping everything.
     */
    virtual void shutdown() = 0;

    /**
     * @brief Reloads the server configuration, functions, scripts and plugins.
     */
    virtual void reload() = 0;

    /**
     * @brief Reload only the Minecraft data for the server.
     *
     * This includes functions and script files from all behaviour packs.
     */
    virtual void reloadData() = 0;

    /**
     * @brief Broadcasts the specified message to every user with the given permission name.
     *
     * @param message message to broadcast
     * @param permission the required permission Permissibles must have to receive the broadcast
     */
    virtual void broadcast(const Message &message, const std::string &permission) const = 0;

    /**
     * @brief Broadcasts the specified message to every user with permission endstone.broadcast.user
     *
     * @param message the message
     */
    virtual void broadcastMessage(const Message &message) const = 0;

    template <typename... Args>
    void broadcastMessage(const fmt::format_string<Args...> format, Args &&...args) const
    {
        try {
            broadcastMessage(fmt::format(format, std::forward<Args>(args)...));
        }
        catch (std::exception &e) {
            getLogger().log(Logger::Error, e.what());
        }
    }

    /**
     * @brief Checks the current thread against the expected primary server thread
     *
     * @return true if the current thread matches the expected primary thread, false otherwise
     */
    [[nodiscard]] virtual bool isPrimaryThread() const = 0;

    /**
     * Gets the instance of the item factory (for ItemMeta).
     *
     * @return the item factory
     * @see ItemFactory
     */
    [[nodiscard]] virtual ItemFactory &getItemFactory() const = 0;

    /**
     * @brief Gets the primary Scoreboard controlled by the server.
     * <p>
     * This Scoreboard is saved by the server, is affected by the /scoreboard command, and is the scoreboard shown by
     * default to players. This will only exist after the level has been loaded.
     *
     * @return the default server scoreboard
     */
    [[nodiscard]] virtual Scoreboard *getScoreboard() const = 0;

    /**
     * @brief Creates a new Scoreboard to be tracked by the server.
     * <p>
     * This will not be saved by the server and is not affected by the /scoreboard command.
     *
     * @return the newly created Scoreboard
     */
    [[nodiscard]] virtual std::shared_ptr<Scoreboard> createScoreboard() = 0;

    /**
     * @brief Gets the current milliseconds per tick (MSPT).
     *
     * @return The average current of milliseconds per tick.
     */
    virtual float getCurrentMillisecondsPerTick() = 0;

    /**
     * @brief Gets the average milliseconds per tick (MSPT).
     *
     * @return The average number of milliseconds per tick.
     */
    virtual float getAverageMillisecondsPerTick() = 0;

    /**
     * @brief Gets the current ticks per second (TPS).
     *
     * @return The current ticks per second
     */
    virtual float getCurrentTicksPerSecond() = 0;
    /**
     * @brief Gets the average ticks per second (TPS).
     *
     * @return The average ticks per second
     */
    virtual float getAverageTicksPerSecond() = 0;

    /**
     * @brief Gets the current tick usage of the server.
     *
     * @return The current tick usage in percentage.
     */
    virtual float getCurrentTickUsage() = 0;

    /**
     * @brief Gets the average tick usage of the server.
     *
     * @return The average tick usage in percentage.
     */
    virtual float getAverageTickUsage() = 0;

    /**
     * @brief Gets the start time of the server.
     *
     * @return The start time of the server.
     */
    [[nodiscard]] virtual std::chrono::system_clock::time_point getStartTime() = 0;

    /**
     * @brief Creates a boss bar instance to display to players. The progress defaults to 1.0.
     *
     * @param title the title of the boss bar
     * @param color the color of the boss bar
     * @param style the style of the boss bar
     * @return the created boss bar
     */
    [[nodiscard]] virtual std::unique_ptr<BossBar> createBossBar(std::string title, BarColor color,
                                                                 BarStyle style) const = 0;

    /**
     * @brief Creates a boss bar instance to display to players. The progress defaults to 1.0.
     *
     * @param title the title of the boss bar
     * @param color the color of the boss bar
     * @param style the style of the boss bar
     * @param flags a list of flags to set on the boss bar
     * @return the created boss bar
     */
    [[nodiscard]] virtual std::unique_ptr<BossBar> createBossBar(std::string title, BarColor color, BarStyle style,
                                                                 std::vector<BarFlag> flags) const = 0;

    /**
     * @brief Creates a new BlockData instance for the specified block type, with all properties initialized to
     * defaults.
     *
     * @param type the block type
     * @return new data instance
     */
    [[nodiscard]] virtual std::unique_ptr<BlockData> createBlockData(std::string type) const = 0;

    /**
     * @brief Creates a new BlockData instance for the specified block type, with all properties initialized to
     * defaults, except for those provided in data.
     *
     * @param type the block type
     * @param block_states block states, for example `{"old_leaf_type":"birch", "persistent_bit":true}`
     * @return new data instance
     */
    [[nodiscard]] virtual std::unique_ptr<BlockData> createBlockData(std::string type,
                                                                     BlockStates block_states) const = 0;

    /**
     * Gets the player ban list.
     *
     * @return The player ban list
     */
    [[nodiscard]] virtual PlayerBanList &getBanList() const = 0;

    /**
     * Gets the IP ban list.
     *
     * @return The IP ban list
     */
    [[nodiscard]] virtual IpBanList &getIpBanList() const = 0;

    /**
     * Gets the service manager.
     *
     * @return The service manager
     */
    [[nodiscard]] virtual ServiceManager &getServiceManager() const = 0;

    /**
     * @brief Returns the registry for the given type.
     *
     * If no registry is present for the given type null will be returned.
     *
     * @param type of the registry to get
     *
     * @return the corresponding registry or null if not present
     */
    [[nodiscard]] virtual IRegistry *_getRegistry(const std::string &type) const = 0;

    template <typename T>
    [[nodiscard]] const Registry<T> &getRegistry() const
    {
        return *static_cast<Registry<T> *>(_getRegistry(T::RegistryType));
    }

    /**
     * @brief Gets the map from the given item ID.
     *
     * @param id the id of the map to get
     *
     * @return a map view if it exists, or null otherwise
     */
    [[nodiscard]] virtual MapView *getMap(std::int64_t id) const = 0;

    /**
     * @brief Create a new map with an automatically assigned ID.
     *
     * @param dimension the Dimension the map will belong to
     *
     * @return a newly created map view
     */
    [[nodiscard]] virtual MapView &createMap(const Dimension &dimension) const = 0;

    /**
     * @brief Used for all administrative messages, such as an operator using a command.
     */
    inline static const std::string BroadcastChannelAdmin = "endstone.broadcast.admin";

    /**
     * @brief Used for all announcement messages, such as informing users that a player has joined.
     */
    inline static const std::string BroadcastChannelUser = "endstone.broadcast.user";
};

}  // namespace endstone
