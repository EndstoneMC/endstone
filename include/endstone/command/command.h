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
#include <utility>
#include <vector>

#include "endstone/command/command_map.h"
#include "endstone/command/command_sender.h"

namespace endstone {
/**
 * Represents a Command, which executes various tasks upon user input
 */
class Command {
public:
    explicit Command(std::string name, std::string description = "", std::vector<std::string> usages = {"/command"},
                     std::vector<std::string> aliases = {}) noexcept
        : name_(std::move(name)), description_(std::move(description)), usages_(std::move(usages)),
          aliases_(std::move(aliases))
    {
    }
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
    [[nodiscard]] std::string getName() const
    {
        return name_;
    }

    /**
     * Gets a brief description of this command
     *
     * @return Description of this command
     */
    [[nodiscard]] std::string getDescription() const
    {
        return description_;
    }

    /**
     * Returns a list of aliases of this command
     *
     * @return List of aliases
     */
    [[nodiscard]] std::vector<std::string> getAliases() const
    {
        return aliases_;
    }

    /**
     * Returns a list of usages of this command
     *
     * @return List of usages
     */
    [[nodiscard]] std::vector<std::string> getUsages() const
    {
        return usages_;
    }

    /**
     * Registers this command to a CommandMap.
     *
     * @param command_map the CommandMap to register to
     * @return true if the registration was successful, false otherwise
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
     * Unregisters this command from a CommandMap
     *
     * @param command_map the CommandMap to unregister from
     * @return true if the unregistration was successful, false otherwise
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

    std::string name_;
    std::string description_;
    std::vector<std::string> aliases_;
    std::vector<std::string> usages_;
    CommandMap *command_map_ = nullptr;
};
}  // namespace endstone
