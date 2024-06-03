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

#include <entt/entt.hpp>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/event/player/player_command_event.h"
#include "endstone/event/server/server_command_event.h"

using endstone::detail::EndstoneServer;

MCRESULT MinecraftCommands::executeCommand(CommandContext &ctx, bool suppress_output) const
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto *sender = ctx.getOrigin().toEndstone();

    switch (ctx.getOrigin().getOriginType()) {
    case CommandOriginType::Player: {
        endstone::Player &player = *static_cast<endstone::Player *>(sender);
        server.getLogger().info("{} issued server command: {}", player.getName(), ctx.getCommand());

        endstone::PlayerCommandEvent event(player, ctx.getCommand());
        server.getPluginManager().callEvent(event);

        if (event.isCancelled()) {
            return MCRESULT_CommandNotFound;
        }
        break;
    }
    case CommandOriginType::DedicatedServer: {
        endstone::ServerCommandEvent event(server.getCommandSender(), ctx.getCommand());
        server.getPluginManager().callEvent(event);

        if (event.isCancelled()) {
            return MCRESULT_CommandNotFound;
        }
        break;
    }
    default:
        break;
    }

    auto command_line = ctx.getCommand();
    if (!command_line.empty() && command_line[0] == '/') {
        command_line = command_line.substr(1);
    }
    auto command_name = command_line.substr(0, command_line.find_first_of(' '));
    auto *command = server.getCommandMap().getCommand(std::string(command_name));
    if (!command || !sender || !command->testPermissionSilently(*sender)) {
        return MCRESULT_CommandNotFound;
    }

    MCRESULT result;
#ifdef _WIN32
    ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&MinecraftCommands::executeCommand, result, this, ctx, suppress_output);
#else
    result = ENDSTONE_HOOK_CALL_ORIGINAL(&MinecraftCommands::executeCommand, this, ctx, suppress_output);
#endif
    return result;
}
