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

class Command;

namespace endstone {
class CommandMap {
public:
    virtual ~CommandMap() = default;

    /**
     * Registers a command. Returns true on success; false if name is already
     * taken and fallback had to be used.
     *
     * @param label the label of the command, without the '/'-prefix.
     * @param fallback_prefix a prefix which is prepended to the command with a ':' one or more times to make the
     * command unique
     * @param command the command to register
     * @return true if command was registered with the passed in label, false otherwise, which indicates the
     * fallbackPrefix was used one or more times
     */
    virtual bool registerCommand(std::string label, std::string fallback_prefix, std::shared_ptr<Command> command) = 0;

    // TODO: dispatch functions

    /**
     * Clears all registered commands.
     */
    [[maybe_unused]] virtual void clearCommands() = 0;

    /**
     * Gets the command registered to the specified name
     *
     * @param name Name of the command to retrieve
     * @return Command with the specified name or nullptr if a command with that label doesn't exist
     */
    [[nodiscard]] virtual Command *getCommand(std::string name) const = 0;
};
}  // namespace endstone
