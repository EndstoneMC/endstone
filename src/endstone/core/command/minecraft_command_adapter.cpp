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

#include "endstone/core/command/minecraft_command_adapter.h"

#include <optional>
#include <string>
#include <vector>

#include "endstone/core/command/command_origin_wrapper.h"
#include "endstone/core/command/parse_token_text.h"

namespace endstone::core {

void MinecraftCommandAdapter::execute(const CommandOrigin &origin, CommandOutput &output) const
{
    const auto &server = EndstoneServer::getInstance();
    const auto &command_map = server.getCommandMap();
    const auto command_name = getCommandName();
    const auto command = command_map.getCommand(command_name);
    if (!command) {
        throw std::runtime_error("Command not found");
    }

    auto sender = origin.getEndstoneSender(output);
    if (command->execute(sender, args_)) {
        output.success();
    }
}

}  // namespace endstone::core

template <>
bool CommandRegistry::parse<endstone::core::MinecraftCommandAdapter>(void *storage, const ParseToken &token,
                                                                     const CommandOrigin &origin, int version,
                                                                     std::string &error,
                                                                     std::vector<std::string> &error_params) const
{
    if (!storage) {
        return false;
    }
    auto &output = static_cast<endstone::core::MinecraftCommandAdapter *>(storage)->args_;
    if (auto result = endstone::core::parseNode(token)) {
        output.emplace_back(*result);
    }
    return true;
}

template <>
const CommandRegistry::ParamParseRule CommandRegistry::ParseRuleFor<endstone::core::MinecraftCommandAdapter>::instance{
    &CommandRegistry::parse<endstone::core::MinecraftCommandAdapter>, {}};
