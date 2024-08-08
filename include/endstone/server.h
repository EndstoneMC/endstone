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
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "endstone/level/level.h"
#include "endstone/logger.h"
#include "endstone/player.h"
#include "endstone/scoreboard/scoreboard.h"
#include "endstone/util/uuid.h"

namespace endstone {

class ConsoleCommandSender;
class Scheduler;
class PluginCommand;
class PluginManager;

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
     * @brief Returns the primary logger associated with this server instance.
     *
     * @return Logger associated with this server
     */
    [[nodiscard]] virtual Logger &getLogger() const = 0;

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
     * @param command the command + arguments.
     * @return true if execution is successful, false otherwise
     */
    [[nodiscard]] virtual bool dispatchCommand(CommandSender &sender, std::string command) const = 0;

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
     * @brief Gets the player with the exact given name, case insensitive.
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
    virtual void broadcast(const std::string &message, const std::string &permission) const = 0;

    /**
     * @brief Broadcasts the specified message to every user with permission endstone.broadcast.user
     *
     * @param message the message
     */
    virtual void broadcastMessage(const std::string &message) const = 0;

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
     * @brief Gets the primary Scoreboard controlled by the server.
     * <p>
     * This Scoreboard is saved by the server, is affected by the /scoreboard command, and is the scoreboard shown by
     * default to players. This will only exist after the level has been loaded.
     *
     * @return the default server scoreboard
     */
    [[nodiscard]] virtual Scoreboard *getScoreboard() const = 0;

    /**
     * Gets a new Scoreboard to be tracked by the server.
     * <p>
     * This will not be saved by the server and is not affected by the /scoreboard command.
     *
     * @return the newly created Scoreboard
     */
    [[nodiscard]] virtual std::shared_ptr<Scoreboard> getNewScoreboard() = 0;

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
     * @return The start time of the server。
     */
    [[nodiscard]] virtual std::chrono::system_clock::time_point getStartTime() = 0;

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
