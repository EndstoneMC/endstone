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

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "endstone/level.h"
#include "endstone/logger.h"
#include "endstone/player.h"
#include "endstone/util/uuid.h"

namespace endstone {

class CommandSender;
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
     * Returns the primary logger associated with this server instance.
     *
     * @return Logger associated with this server
     */
    [[nodiscard]] virtual Logger &getLogger() const = 0;

    /**
     * Gets the plugin manager for interfacing with plugins.
     *
     * @return a plugin manager for this Server instance
     */
    [[nodiscard]] virtual PluginManager &getPluginManager() const = 0;

    /**
     * Gets a PluginCommand with the given name or alias.
     *
     * @param name the name of the command to retrieve
     * @return a plugin command if found, null otherwise
     */
    [[nodiscard]] virtual PluginCommand *getPluginCommand(std::string name) const = 0;

    /**
     * Gets a CommandSender for this server.
     *
     * @return a console command sender
     */
    [[nodiscard]] virtual CommandSender &getCommandSender() const = 0;

    /**
     * Gets a list of all levels on this server.
     *
     * @return a list of levels
     */
    [[nodiscard]] virtual std::vector<Level *> getLevels() const = 0;

    /**
     * Gets the level with the given name.
     *
     * @param name the name of the world to retrieve
     * @return a level with the given name, or nullptr if none exists
     */
    [[nodiscard]] virtual Level *getLevel(std::string name) const = 0;

    /**
     * Gets the player with the given UUID.
     *
     * @param id UUID of the player to retrieve
     * @return a player object if one was found, null otherwise
     */
    [[nodiscard]] virtual Player *getPlayer(endstone::UUID id) const = 0;

    /**
     * Gets the name of this server implementation.
     *
     * @return name of this server implementation
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * Gets the version string of this server implementation.
     *
     * @return version of this server implementation
     */
    [[nodiscard]] virtual std::string getVersion() const = 0;

    /**
     * Gets the Minecraft version that this server is running.
     *
     * @return version of Minecraft
     */
    [[nodiscard]] virtual std::string getMinecraftVersion() const = 0;

    /**
     * Checks the current thread against the expected primary server thread
     *
     * @return true if the current thread matches the expected primary thread, false otherwise
     */
    [[nodiscard]] virtual bool isPrimaryThread() const = 0;
};

}  // namespace endstone
