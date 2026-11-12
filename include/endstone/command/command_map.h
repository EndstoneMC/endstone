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
     * Registers a command, using the lowercased plugin name as the fallback prefix for a PluginCommand and `endstone`
     * otherwise.
     *
     * @param command the command to register
     * @return true if the command was registered under its own name, false if the name was already taken
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
     * @return true if the command was found and run, false if no such command exists
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
     * Registers a command under its name and under `fallback_prefix:name`.
     *
     * If the name is already taken, the command is only reachable as `fallback_prefix:name` and its name changes to
     * that. The command's aliases are registered the same way, and an alias that is already taken is dropped. A
     * `fallback_prefix:name` that another command already holds is taken over. Built-in Endstone commands use the
     * fallback prefix `endstone`.
     *
     * @param fallback_prefix a prefix which is prepended to the command name for a unique address
     * @param command the command to register
     * @return true if the command was registered under its own name, false if the name was already taken
     */
    virtual bool registerCommand(std::string fallback_prefix, NotNull<Command> command) = 0;
};
}  // namespace endstone
