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
    explicit Command(CommandMap &command_map) : command_map_(command_map) {}
    virtual ~Command() noexcept = default;

public:
    /**
     * Executes the command, returning its success
     *
     * @param sender Source of the command
     * @param args Arguments passed to the command
     * @return true if the execution was successful, otherwise false
     */
    [[nodiscard]] virtual bool execute(CommandSender &sender,
                                       const std::map<std::string, std::string> &args) const noexcept = 0;

    /**
     * Returns the name of this command
     *
     * @return Name of this command
     */
    [[nodiscard]] virtual std::string getName() const noexcept = 0;

    /**
     * Gets a brief description of this command
     *
     * @return Description of this command
     */
    [[nodiscard]] virtual std::string getDescription() const noexcept = 0;

    /**
     * Returns a list of aliases of this command
     *
     * @return List of aliases
     */
    [[nodiscard]] virtual std::vector<std::string> getAliases() const noexcept = 0;

private:
    CommandMap &command_map_;
};
}  // namespace endstone
