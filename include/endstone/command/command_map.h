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

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "endstone/command/command_sender.h"
#include "endstone/util/pointers.h"

namespace endstone {

class Command;
class LiteralCommandNode;
class Plugin;

/**
 * Represents a command map that manages all commands of the Server.
 */
class CommandMap {
public:
    CommandMap() = default;
    CommandMap(const CommandMap &) = delete;
    CommandMap &operator=(const CommandMap &) = delete;
    CommandMap(CommandMap &&) = default;
    CommandMap &operator=(CommandMap &&) = default;

    virtual ~CommandMap() = default;

    /**
     * Registers a command.
     *
     * @param command the command to register
     * @return true on success, false if a command with the same name is already registered
     */
    virtual bool registerCommand(NotNull<Command> command) = 0;

    /**
     * Constructs a command of the given type and registers it.
     *
     * @tparam T the type of command to construct
     * @param args the arguments to construct the command with
     * @return true on success, false if a command with the same name is already registered
     */
    template <class T, class... Args>
        requires std::is_base_of_v<Command, T>
    bool registerCommand(Args &&...args)
    {
        return registerCommand(std::make_shared<T>(std::forward<Args>(args)...));
    }

    /**
     * Looks for the requested command and executes it if found.
     *
     * @param sender The command's sender
     * @param command_line command + arguments. Example: "/test abc 123"
     * @return true if execution is successful, false otherwise
     */
    virtual bool dispatch(const NotNull<CommandSender> &sender, std::string command_line) const = 0;

    /**
     * Clears all registered commands.
     */
    virtual void clearCommands() = 0;

    /**
     * Gets the command registered to the specified name.
     *
     * @param name Name of the command to retrieve
     * @return Command with the specified name, a null handle if a command with that label doesn't exist
     */
    [[nodiscard]] virtual Nullable<Command> getCommand(std::string name) const = 0;

    /**
     * Registers a command tree on behalf of a plugin.
     *
     * @param root the root of the command tree, from LiteralArgumentBuilder::build()
     * @param owner the plugin the command belongs to
     * @return true on success, false if a command with the same name is already registered
     */
    virtual bool registerCommand(NotNull<LiteralCommandNode> root, Plugin &owner) = 0;

    /**
     * Replaces the values of a named suggestion set.
     *
     * The set is what Arguments::softEnum() completes from. Every connected client is updated, so
     * players see the new values without rejoining. The values are only a hint: the server accepts
     * any string for a soft enum argument.
     *
     * @param name Name of the suggestion set
     * @param values New values
     */
    virtual void setSuggestions(std::string name, std::vector<std::string> values) = 0;
};
}  // namespace endstone
