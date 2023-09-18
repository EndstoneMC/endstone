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
#include <vector>

#include "endstone/command/command.h"

class CommandMap {
public:
    virtual ~CommandMap() noexcept = default;

    /**
     * Registers all the commands belonging to a certain plugin.
     * <p>
     * Caller can use:
     * <ul>
     *  <li>command.getName() to determine the label registered for this
     *      command
     *  <li>command.getAliases() to determine the aliases which where
     *      registered
     * </ul>
     *
     * @param fallback_prefix a prefix which is prepended to each command with
     *     a ':' one or more times to make the command unique
     * @param commands a list of commands to register
     */
    virtual void registerAll(const std::string &fallback_prefix,
                             const std::vector<std::shared_ptr<Command>> &commands) noexcept = 0;

    /**
     * Registers a command. Returns true on success; false if name is already
     * taken and fallback had to be used.
     * <p>
     * Caller can use:
     * <ul>
     * <li>command.getName() to determine the label registered for this
     *     command
     * <li>command.getAliases() to determine the aliases which where
     *     registered
     * </ul>
     *
     * @param label the label of the command, without the '/'-prefix.
     * @param fallback_prefix a prefix which is prepended to the command with a
     *     ':' one or more times to make the command unique
     * @param command the command to register
     * @return true if command was registered with the passed in label, false
     *     otherwise, which indicates the fallbackPrefix was used one or more
     *     times
     */
    virtual bool registerCommand(std::string label, std::string fallback_prefix,
                                 std::shared_ptr<Command> command) noexcept = 0;

    /**
     * Registers a command. Returns true on success; false if name is already
     * taken and fallback had to be used.
     * <p>
     * Caller can use:
     * <ul>
     * <li>command.getName() to determine the label registered for this
     *     command
     * <li>command.getAliases() to determine the aliases which where
     *     registered
     * </ul>
     *
     * @param fallback_prefix a prefix which is prepended to the command with a
     *     ':' one or more times to make the command unique
     * @param command the command to register, from which label is determined
     *     from the command name
     * @return true if command was registered with the passed in label, false
     *     otherwise, which indicates the fallbackPrefix was used one or more
     *     times
     */
    virtual bool registerCommand(const std::string &fallback_prefix, std::shared_ptr<Command> command) noexcept = 0;

    /**
     * Looks for the requested command and executes it if found.
     *
     * @param sender The command's sender
     * @param command_line command + arguments. Example: "/test abc 123"
     * @return returns false if no target is found, true otherwise.
     */
    virtual bool dispatch(CommandSender &sender, const std::string &command_line) const noexcept = 0;

    /**
     * Clears all registered commands.
     */
    [[maybe_unused]] virtual void clearCommands() noexcept = 0;

    /**
     * Gets the command registered to the specified name
     *
     * @param name Name of the command to retrieve
     * @return Command with the specified name or nullptr if a command with that
     *     label doesn't exist
     */
    [[nodiscard]] virtual Command *getCommand(std::string name) const noexcept = 0;

    static std::string getCommandName(const std::string &command_line) noexcept
    {
        const std::size_t start = command_line[0] == '/' ? 1 : 0;
        std::size_t end = command_line.find(' ');
        if (end == std::string::npos) {
            end = command_line.length();
        }

        return command_line.substr(start, end - start);
    }
};
