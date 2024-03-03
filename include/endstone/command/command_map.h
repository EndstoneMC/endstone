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
#include <vector>

namespace endstone {

class Command;
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
    virtual bool registerCommand(std::shared_ptr<Command> command) = 0;

    /**
     * Clears all registered commands.
     */
    // virtual void clearCommands() = 0;

    /**
     * Gets the command registered to the specified name
     *
     * @param name Name of the command to retrieve
     * @return Command with the specified name or nullptr if a command with that label doesn't exist
     */
    [[nodiscard]] virtual Command *getCommand(std::string name) const = 0;
};
}  // namespace endstone
