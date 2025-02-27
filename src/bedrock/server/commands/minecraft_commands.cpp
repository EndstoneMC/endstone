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

#include "bedrock/server/commands/minecraft_commands.h"

#include "bedrock/symbol.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/level/level.h"

Command *MinecraftCommands::compileCommand(HashedString const &command_str, CommandOrigin &origin,
                                           CurrentCmdVersion command_version,
                                           std::function<void(const std::string &)> on_parser_error)
{
    return BEDROCK_CALL(&MinecraftCommands::compileCommand, this, command_str, origin, command_version,
                        on_parser_error);
}

CommandOutputType MinecraftCommands::getOutputType(const CommandOrigin &origin)
{
    switch (origin.getOriginType()) {
    case CommandOriginType::Test:
    case CommandOriginType::AutomationPlayer:
    case CommandOriginType::ClientAutomation:
    case CommandOriginType::Script: {
        return CommandOutputType::DataSet;
    }
    case CommandOriginType::DedicatedServer: {
        return CommandOutputType::AllOutput;
    }
    case CommandOriginType::CommandBlock:
    case CommandOriginType::MinecartCommandBlock: {
        if (auto *level = origin.getLevel(); level) {
            if (!level->getGameRules().getBool(GameRuleId(GameRules::COMMAND_BLOCK_OUTPUT), false)) {
                return CommandOutputType::LastOutput;
            }
        }
        return CommandOutputType::AllOutput;
    }
    default: {
        if (auto *level = origin.getLevel(); level) {
            if (!level->getGameRules().getBool(GameRuleId(GameRules::SEND_COMMAND_FEEDBACK), false)) {
                return CommandOutputType::Silent;
            }
        }
        return CommandOutputType::AllOutput;
    }
    }
}
