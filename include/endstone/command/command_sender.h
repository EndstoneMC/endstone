// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <string>
#include <utility>

#include <fmt/format.h>

#include "endstone/message.h"
#include "endstone/permissions/permissible.h"

namespace endstone {

class Actor;
class BlockCommandSender;
class ConsoleCommandSender;
class Mob;
class Server;
class Player;

/**
 * @brief Represents a command sender.
 */
class CommandSender : public Permissible {
public:
    // Permissible
    [[nodiscard]] CommandSender *asCommandSender() const override { return const_cast<CommandSender *>(this); }

    /**
     * @brief Gets a CommandSender as ConsoleCommandSender
     *
     * @return CommandSender, nullptr if not a ConsoleCommandSender
     */
    [[nodiscard]] virtual ConsoleCommandSender *asConsole() const = 0;

    /**
     * @brief Gets a CommandSender as BlockCommandSender
     *
     * @return CommandSender, nullptr if not a BlockCommandSender
     */
    [[nodiscard]] virtual BlockCommandSender *asBlock() const = 0;

    /**
     * @brief Gets a CommandSender as Actor
     *
     * @return Actor, nullptr if not an Actor
     */
    [[nodiscard]] virtual Actor *asActor() const = 0;

    /**
     * @brief Gets a CommandSender as Player
     *
     * @return Player, nullptr if not a Player
     */
    [[nodiscard]] virtual Player *asPlayer() const = 0;

    /**
     * @brief Sends this sender a message
     *
     * @param message Message to be displayed
     */
    virtual void sendMessage(const Message &message) const = 0;

    template <typename... Args>
    void sendMessage(const fmt::format_string<Args...> format, Args &&...args) const
    {
        try {
            sendMessage(fmt::format(format, std::forward<Args>(args)...));
        }
        catch (std::exception &e) {
            sendErrorMessage(e.what());
        }
    }

    /**
     * @brief Sends this sender a error message
     *
     * @param message Error message to be displayed
     */
    virtual void sendErrorMessage(const Message &message) const = 0;

    template <typename... Args>
    void sendErrorMessage(const fmt::format_string<Args...> format, Args &&...args) const
    {
        sendErrorMessage(fmt::format(format, std::forward<Args>(args)...));
    }

    /**
     * @brief Returns the server instance that this command is running on
     *
     * @return Server instance
     */
    [[nodiscard]] virtual Server &getServer() const = 0;

    /**
     * @brief Gets the name of this command sender
     *
     * @return Name of the sender
     */
    [[nodiscard]] virtual std::string getName() const = 0;
};
}  // namespace endstone
