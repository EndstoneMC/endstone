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

#include "bedrock/server/commands/minecraft_commands.h"
#include "endstone/command/command.h"
#include "endstone/command/command_map.h"
#include "endstone/command/command_node.h"

namespace endstone::core {

class EndstoneServer;
class EndstoneCommandMap : public CommandMap {
public:
    explicit EndstoneCommandMap(EndstoneServer &server);
    using CommandMap::registerCommand;
    bool registerCommand(NotNull<Command> command) override;
    bool registerCommand(NotNull<LiteralCommandNode> root, Plugin &owner) override;
    void setSuggestions(std::string name, std::vector<std::string> values) override;
    bool dispatch(const NotNull<CommandSender> &sender, std::string command_line) const override;
    void clearCommands() override;
    [[nodiscard]] Nullable<Command> getCommand(std::string name) const override;
    [[nodiscard]] ::MinecraftCommands &getHandle() const;

private:
    friend class EndstoneServer;
    void setDefaultCommands();
    void setPluginCommands();
    void unregisterCommand(std::string name);
    void clearEnumValues(const std::string &enum_name);
    void removeEnumValueFromExisting(const std::string &enum_name, const std::string &enum_value);

    EndstoneServer &server_;
    std::recursive_mutex mutex_;
    std::unordered_map<std::string, NotNull<Command>> custom_commands_;

    static const std::unordered_map<std::string, CommandRegistry::HardNonTerminal> TYPE_SYMBOLS;
};

}  // namespace endstone::core
