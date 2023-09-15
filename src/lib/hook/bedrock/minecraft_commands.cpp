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

#include "bedrock/minecraft_commands.h"

#include "bedrock/command_context.h"
#include "endstone/command/bedrock/bedrock_command_sender.h"
#include "endstone/command/command_map.h"
#include "endstone/endstone.h"
#include "endstone/endstone_server.h"
#include "hook/hook.h"

MCRESULT MinecraftCommands::executeCommand(CommandContext &command_ctx, bool flag) const
{
    constexpr static auto func_decorated_name = __FUNCDNAME__;

    if (!vanilla_dispatcher.has_value()) {
        vanilla_dispatcher = [this, version = command_ctx.getCommandVersion(), flag](auto &command_line,
                                                                                     auto command_origin) -> bool {
            MCRESULT result = {};
            CommandContext ctx = {command_line, std::move(command_origin), version};
            auto func = endstone::hook::get_function_rvo(&MinecraftCommands::executeCommand, func_decorated_name);
            result = *func(this, &result, ctx, flag);
            return result.is_success;
        };
    }

    constexpr static MCRESULT success = {true};
    constexpr static MCRESULT failed = {false};

    auto command_name = CommandMap::getCommandName(command_ctx.getCommandLine());

    auto &server = dynamic_cast<EndstoneServer &>(Endstone::getServer());
    auto command = server.getCommandMap().getCommand(command_name);

    MCRESULT result;

    auto sender = BedrockCommandSender::fromCommandOrigin(command_ctx.takeOrigin());
    if (server.dispatchCommand(*sender, command_ctx.getCommandLine())) {
        result = success;
    }
    else {
        result = failed;
    }

    return result;
}
