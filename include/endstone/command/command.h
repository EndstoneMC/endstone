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

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

#include "endstone/command/command_map.h"
#include "endstone/command/command_sender.h"

namespace endstone {
/**
 * Represents a Command, which executes various tasks upon user input
 */
class Command {
protected:
    explicit Command(std::string name, std::string description = "", std::vector<std::string> usages = {},
                     std::vector<std::string> aliases = {})
    {
        setName(std::move(name));
        setDescription(std::move(description));
        setUsages(std::move(usages));
        setAliases(std::move(aliases));
    }

public:
    virtual ~Command() = default;

    /**
     * Executes the command, returning its success
     *
     * @param sender Source of the command
     * @param args Arguments passed to the command
     * @return true if the execution was successful, otherwise false
     */
    [[nodiscard]] virtual bool execute(CommandSender &sender, const std::vector<std::string> &args) const
    {
        return false;
    }

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
     * Sets the name of this command.
     *
     * May only be used before registering the command.
     *
     * @param name New command name
     * @return returns true if the name is changed, false otherwise
     */
    bool setName(std::string name)
    {
        if (!isRegistered()) {
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
            name_ = std::move(name);
            return true;
        }

        return false;
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
     * Sets a brief description of this command.
     *
     * @param description new command description
     * @return this command object, for chaining
     */
    Command &setDescription(std::string description)
    {
        if (!isRegistered()) {
            description_ = std::move(description);
        }

        return *this;
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
     * Sets the list of aliases to request on registration for this command.
     *
     * @param aliases aliases to register to this command
     * @return this command object, for chaining
     */
    template <typename... Alias>
    Command &setAliases(Alias... aliases)
    {
        std::vector<std::string> all_aliases = {aliases...};
        if (!isRegistered()) {
            aliases_.clear();
            for (auto alias : all_aliases) {
                std::transform(alias.begin(), alias.end(), alias.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                aliases_.push_back(alias);
            }
        }
        return *this;
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
     * Sets the example usage of this command
     *
     * @param usages new example usage
     * @return this command object, for chaining
     */
    template <typename... Usage>
    Command &setUsages(Usage... usages)
    {
        std::vector<std::string> all_usages = {usages...};
        if (!isRegistered()) {
            if (all_usages.empty()) {
                all_usages.push_back("/" + getName());
            }
            usages_ = std::move(all_usages);
        }
        return *this;
    }

    /**
     * Registers this command to a CommandMap.
     *
     * @param command_map the CommandMap to register to
     * @return true if the registration was successful, false otherwise
     */
    bool registerTo(CommandMap &command_map)
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
    bool unregisterFrom(CommandMap &command_map)
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
    [[nodiscard]] bool isRegistered() const
    {
        return command_map_ != nullptr;
    }

private:
    bool allowChangesFrom(CommandMap &command_map)
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
