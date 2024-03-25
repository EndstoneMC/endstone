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

#include "bedrock/command/minecraft_commands.h"

#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/detail/singleton.h"
#include "endstone/event/server/server_command_event.h"

using endstone::detail::EndstoneServer;
using endstone::detail::Singleton;

MCRESULT MinecraftCommands::executeCommand(CommandContext &ctx, bool flag) const
{
    MCRESULT result;
    if (ctx.getOrigin().getOriginType() != CommandOriginType::DedicatedServer) {
        ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&MinecraftCommands::executeCommand, result, this, ctx, flag);
        return result;
    }

    auto &server = Singleton<EndstoneServer>::getInstance();
    endstone::ServerCommandEvent event(server.getCommandSender(), ctx.getCommandLine());
    server.getPluginManager().callEvent(event);

    if (event.isCancelled()) {
        result = MCRESULT_CommandsDisabled;
    }
    else {
        ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&MinecraftCommands::executeCommand, result, this, ctx, flag);
    }
    return result;
}
