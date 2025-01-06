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

#include <mutex>
#include <unordered_map>

#include "endstone/command/command.h"
#include "endstone/command/command_map.h"
#include "endstone/detail/command/command_wrapper.h"

namespace endstone::core {

class EndstoneServer;
class EndstoneCommandMap : public CommandMap {
public:
    explicit EndstoneCommandMap(EndstoneServer &server);
    bool registerCommand(std::shared_ptr<Command> command) override;
    bool dispatch(CommandSender &sender, std::string command_line) const override;
    void clearCommands() override;
    [[nodiscard]] Command *getCommand(std::string name) const override;

private:
    friend class EndstoneServer;
    void setDefaultCommands();
    void setMinecraftCommands();
    void setPluginCommands();

    void patchCommandRegistry();
    void saveCommandRegistryState() const;
    void restoreCommandRegistryState() const;

    EndstoneServer &server_;
    std::recursive_mutex mutex_;
    std::unordered_map<std::string, std::shared_ptr<CommandWrapper>> known_commands_;
};

}  // namespace endstone::core
