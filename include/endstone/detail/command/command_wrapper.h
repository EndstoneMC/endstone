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

#include "bedrock/server/commands/minecraft_commands.h"
#include "endstone/command/command.h"

namespace endstone::detail {

class CommandWrapper : public Command {
public:
    CommandWrapper(MinecraftCommands &minecraft_commands, std::shared_ptr<Command> command);

    [[nodiscard]] bool execute(CommandSender &sender, const std::vector<std::string> &args) const override;
    [[nodiscard]] PluginCommand *asPluginCommand() const override;
    [[nodiscard]] Command &unwrap() const;

    static std::unique_ptr<CommandOrigin> getCommandOrigin(CommandSender &sender);

private:
    MinecraftCommands &minecraft_commands_;
    std::shared_ptr<Command> command_;
};

}  // namespace endstone::detail
