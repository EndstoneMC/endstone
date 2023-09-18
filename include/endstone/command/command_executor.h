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

#include <string>
#include <vector>

#include "endstone/command/command.h"
#include "endstone/command/command_sender.h"

/**
 * Represents a class which contains a single method for executing commands
 */
class CommandExecutor {
public:
    virtual ~CommandExecutor() noexcept = default;

    /**
     * Executes the given command, returning its success.
     *
     * If false is returned, then the command usages
     * (if defined) will be sent to the player.
     *
     * @param sender Source of the command
     * @param command Command which was executed
     * @param label Alias of the command which was used
     * @param args Passed command arguments
     * @return true if a valid command, otherwise false
     */
    virtual bool onCommand(const CommandSender &sender, const Command &command, const std::string &label,
                           const std::vector<std::string> &args) noexcept = 0;
};
