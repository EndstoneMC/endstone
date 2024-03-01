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

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "endstone/command/command_map.h"
#include "endstone/command/command_sender.h"

namespace endstone {
/**
 * Represents a registered Command, which executes various tasks upon user input
 */
class Command {
public:
    virtual ~Command() = default;

    /**
     * Executes the command, returning its success
     *
     * @param sender Source of the command
     * @param args Arguments passed to the command
     * @return true if the execution was successful, otherwise false
     */
    [[nodiscard]] virtual bool execute(CommandSender &sender, const std::map<std::string, std::string> &args) const = 0;

    /**
     * Returns the name of this command
     *
     * @return Name of this command
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * Gets a brief description of this command
     *
     * @return Description of this command
     */
    [[nodiscard]] virtual std::string getDescription() const
    {
        return "";
    }

    /**
     * Returns a list of aliases of this command
     *
     * @return List of aliases
     */
    [[nodiscard]] virtual std::vector<std::string> getAliases() const
    {
        return {};
    }

    /**
     * Registers this command to a CommandMap.
     * Once called it only allows changes the registered CommandMap
     *
     * @param command_map the CommandMap to register this command to
     * @return true if the registration was successful (the current registered
     *     CommandMap was the passed CommandMap or null) false otherwise
     */
    bool registerTo(CommandMap &command_map) noexcept
    {
        if (allowChangesFrom(command_map)) {
            command_map_ = &command_map;
            return true;
        }

        return false;
    }

    /**
     * Unregisters this command from the passed CommandMap applying any
     * outstanding changes
     *
     * @param command_map the CommandMap to unregister
     * @return true if the unregistration was successful (the current
     *     registered CommandMap was the passed CommandMap or null) false
     *     otherwise
     */
    bool unregisterFrom(CommandMap &command_map) noexcept
    {
        if (allowChangesFrom(command_map)) {
            command_map_ = nullptr;
            return true;
        }

        return false;
    }

    /**
     * Returns the current registered state of this command
     *
     * @return true if this command is currently registered false otherwise
     */
    [[nodiscard]] bool isRegistered() const noexcept
    {
        return command_map_ != nullptr;
    }

private:
    bool allowChangesFrom(CommandMap &command_map) noexcept
    {
        return (!isRegistered() || command_map_ == &command_map);
    }

    CommandMap *command_map_ = nullptr;
};
}  // namespace endstone
