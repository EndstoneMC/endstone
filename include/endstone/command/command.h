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

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "endstone/chat_color.h"
#include "endstone/command/command_sender.h"

class CommandMap;

/**
 * Represents a Command, which executes various tasks upon user input
 */
class Command {
public:
    explicit Command(const std::string &name) noexcept : Command(name, "", {"/" + name}, {}) {}
    explicit Command(const std::string &name, std::string description, const std::vector<std::string> &usages,
                     const std::vector<std::string> &aliases) noexcept
        : name_(name), label_(name), next_label_(name), description_(std::move(description)), usages_(usages),
          aliases_(aliases), active_aliases_(std::vector<std::string>(aliases))
    {
    }
    virtual ~Command() noexcept = default;

protected:
    Command(const Command &command) noexcept = default;

public:
    /**
     * Executes the command, returning its success
     *
     * @param sender Source object which is executing this command
     * @param label The alias of the command used
     * @param args All arguments passed to the command, split via ' '
     * @return true if the command was successful, otherwise false
     */
    virtual bool execute(CommandSender &sender, const std::string &label,
                         const std::vector<std::string> &args) const noexcept
    {
        return false;
    }

    /**
     * Returns the name of this command
     *
     * @return Name of this command
     */
    [[nodiscard]] [[nodiscard]] const std::string &getName() const noexcept
    {
        return name_;
    }

    /**
     * Sets the name of this command.
     * <p>
     * May only be used before registering the command.
     * Will return true if the new name is set, and false
     * if the command has already been registered.
     *
     * @param name New command name
     * @return returns true if the name change happened instantly or false if
     *     the command was already registered
     */
    bool setName(const std::string &name) noexcept
    {
        if (!isRegistered()) {
            name_ = name;
            return true;
        }

        return false;
    }

    /**
     * Gets the permission required by users to be able to perform this command
     *
     * @return Permission name, or null if none
     */
    [[nodiscard]] const std::optional<std::string> &getPermission() const
    {
        return permission_;
    }

    /**
     * Sets the permission required by users to be able to perform this command
     *
     * @param permission Permission name
     */
    void setPermission(const std::optional<std::string> &permission)
    {
        permission_ = permission;
    }

    /**
     * Tests the given CommandSender to see if they can perform this command.
     *
     * If they do not have permission, they will be informed that they cannot
     * do this.
     *
     * @param target CommandSender to test
     * @return true if they can use it, otherwise false
     */
    [[nodiscard]] bool testPermission(const CommandSender &target) const noexcept
    {
        if (testPermissionSilently(target)) {
            return true;
        }

        target.sendMessage(ChatColor::Red + "You do not have permission to use this command.");
        return false;
    }

    /**
     * Tests the given CommandSender to see if they can perform this command.
     *
     * No error message is sent to the sender.
     *
     * @param target CommandSender to test
     * @return true if they can use it, otherwise false
     */
    [[nodiscard]] bool testPermissionSilently(const CommandSender &target) const noexcept
    {
        if (!permission_.has_value() || permission_.value().empty()) {
            return true;
        }

        return target.hasPermission(permission_.value());
    }

    /**
     * Returns the label for this command
     *
     * @return Label of this command
     */
    [[nodiscard]] const std::string &getLabel() const noexcept
    {
        return label_;
    }

    /**
     * Sets the label of this command.
     * <p>
     * May only be used before registering the command.
     * Will return true if the new name is set, and false
     * if the command has already been registered.
     *
     * @param name The command's name
     * @return returns true if the name change happened instantly or false if
     *     the command was already registered
     */
    bool setLabel(const std::string &name) noexcept
    {
        next_label_ = name;

        if (!isRegistered()) {
            label_ = name;
            return true;
        }

        return false;
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
            active_aliases_ = std::vector<std::string>(aliases_);
            label_ = next_label_;
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

    /**
     * Returns a list of aliases of this command
     *
     * @return List of aliases
     */
    [[nodiscard]] const std::vector<std::string> &getAliases() const noexcept
    {
        return active_aliases_;
    }

    /**
     * Gets a brief description of this command
     *
     * @return Description of this command
     */
    [[nodiscard]] const std::string &getDescription() const noexcept
    {
        return description_;
    }

    /**
     * Gets example usages of this command
     *
     * @return List of example usages
     */
    [[nodiscard]] const std::vector<std::string> &getUsages() const noexcept
    {
        return usages_;
    }

    /**
     * Sets the list of aliases to request on registration for this command.
     *
     * @param aliases aliases to register to this command
     * @return this command object, for chaining
     */
    Command &setAliases(const std::vector<std::string> &aliases) noexcept
    {
        aliases_ = aliases;
        if (!isRegistered()) {
            active_aliases_ = std::vector<std::string>(aliases);
        }

        return *this;
    }

    /**
     * Sets a brief description of this command.
     *
     * @param description new command description
     * @return this command object, for chaining
     */
    Command &setDescription(const std::string &description) noexcept
    {
        description_ = description;
        return *this;
    }

    /**
     * Sets the example usage of this command
     *
     * @param usages new example usage
     * @return this command object, for chaining
     */
    Command &setUsages(std::vector<std::string> usages) noexcept
    {
        usages_ = std::move(usages);
        return *this;
    }

private:
    bool allowChangesFrom(CommandMap &command_map) noexcept
    {
        return (!isRegistered() || command_map_ == &command_map);
    }

    std::string name_;
    std::string label_;
    std::string next_label_;
    std::vector<std::string> aliases_;
    std::vector<std::string> active_aliases_;
    std::optional<std::string> permission_;

protected:
    std::vector<std::string> usages_;
    std::string description_;

private:
    CommandMap *command_map_ = nullptr;
};
