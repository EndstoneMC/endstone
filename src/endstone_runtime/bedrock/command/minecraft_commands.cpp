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

#include <entt/entt.hpp>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/event/player/player_command_event.h"
#include "endstone/event/server/server_command_event.h"

using endstone::detail::EndstoneServer;

MCRESULT MinecraftCommands::executeCommand(CommandContext &ctx, bool flag) const
{
    auto &server = entt::locator<EndstoneServer>::value();

    switch (ctx.getOrigin().getOriginType()) {
    case CommandOriginType::Player: {
        auto *entity = ctx.getOrigin().getEntity();
        if (!entity->isPlayer()) {
            throw std::runtime_error("Command was executed by an non-player entity");
        }
        endstone::Player &player = static_cast<::Player *>(entity)->getEndstonePlayer();
        server.getLogger().info("{} issued server command: {}", player.getName(), ctx.getCommandLine());

        endstone::PlayerCommandEvent event(player, ctx.getCommandLine());
        server.getPluginManager().callEvent(event);

        if (event.isCancelled()) {
            return MCRESULT_CommandsDisabled;
        }
        break;
    }
    case CommandOriginType::DedicatedServer: {
        endstone::ServerCommandEvent event(server.getCommandSender(), ctx.getCommandLine());
        server.getPluginManager().callEvent(event);

        if (event.isCancelled()) {
            return MCRESULT_CommandsDisabled;
        }
        break;
    }
    default:
        break;
    }

    MCRESULT result;
#ifdef _WIN32
    ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&MinecraftCommands::executeCommand, result, this, ctx, flag);
#else
    result = ENDSTONE_HOOK_CALL_ORIGINAL(&MinecraftCommands::executeCommand, this, ctx, flag);
#endif
    return result;
}
