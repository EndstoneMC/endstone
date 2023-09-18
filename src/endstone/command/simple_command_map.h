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
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "endstone/command/command_map.h"

class SimpleCommandMap : public CommandMap {
public:
    explicit SimpleCommandMap(Server &server);

    void registerAll(const std::string &fallback_prefix,
                     const std::vector<std::shared_ptr<Command>> &commands) noexcept override;
    bool registerCommand(std::string label, std::string fallback_prefix,
                         std::shared_ptr<Command> command) noexcept override;
    bool registerCommand(const std::string &fallback_prefix, std::shared_ptr<Command> command) noexcept override;
    bool dispatch(CommandSender &sender, const std::string &command_line) const noexcept override;
    void clearCommands() noexcept override;
    [[nodiscard]] Command *getCommand(std::string name) const noexcept override;

    void setFallbackCommands();
    [[nodiscard]] std::vector<Command *> getCommands() const noexcept;

protected:
    std::unordered_map<std::string, std::shared_ptr<Command>> known_commands_;

private:
    void setDefaultCommands();

    /**
     * Registers a command with the given name is possible. Also uses
     * fallbackPrefix to create a unique name.
     *
     * @param label the name of the command, without the '/'-prefix.
     * @param command the command to register
     * @param is_alias whether the command is an alias
     * @param fallback_prefix a prefix which is prepended to the command for a
     *     unique address
     * @return true if command was registered, false otherwise.
     */
    bool registerCommand(const std::string &label, const std::shared_ptr<Command> &command, bool is_alias,
                         const std::string &fallback_prefix) noexcept;

    Server &server_;
    std::mutex mutex_;
};
