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

#include <optional>
#include <string_view>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/core/command/console_command_sender.h"
#include "endstone/core/player.h"
#include "endstone/core/player_spawn_context.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_command_event.h"
#include "endstone/event/server/server_command_event.h"
#include "endstone/runtime/hook.h"

using endstone::core::EndstoneServer;

namespace {
bool is_spawnpoint_command(std::string_view command)
{
    if (!command.empty() && command.front() == '/') {
        command.remove_prefix(1);
    }
    const auto end = command.find_first_of(" \t\r\n");
    return command.substr(0, end) == "spawnpoint";
}
}  // namespace

MCRESULT MinecraftCommands::executeCommand(CommandContext &ctx, bool suppress_output) const
{
    const auto &server = EndstoneServer::getInstance();
    const auto origin_type = ctx.getOrigin().getOriginType();
    std::optional<endstone::core::PlayerSpawnContextScope> spawn_context_scope;
    if (origin_type != CommandOriginType::Player && origin_type != CommandOriginType::DedicatedServer &&
        is_spawnpoint_command(ctx.getCommand())) {
        spawn_context_scope.emplace(
            endstone::core::PlayerSpawnContext{nullptr, endstone::PlayerSetSpawnEvent::Cause::Command});
    }

    switch (origin_type) {
    case CommandOriginType::Player: {
        auto command_line = ctx.getCommand();
        auto player = ctx.getOrigin().getEntity()->getEndstoneActor<endstone::core::EndstonePlayer>();

        endstone::PlayerCommandEvent e(player, command_line);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return MCRESULT_CommandsDisabled;
        }

        command_line = e.getCommand();
        if (server.logCommands()) {
            server.getLogger().info("{} issued server command: {}", player->getName(), command_line);
        }
        if (server.dispatchCommand(player, command_line)) {
            return MCRESULT_Success;
        }
        return MCRESULT_CommandNotFound;
    }
    case CommandOriginType::DedicatedServer: {
        auto command_line = ctx.getCommand();
        auto console = server.getCommandSender();

        endstone::ServerCommandEvent e(console, command_line);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return MCRESULT_CommandsDisabled;
        }

        command_line = e.getCommand();
        if (server.dispatchCommand(console, command_line)) {
            return MCRESULT_Success;
        }
        return MCRESULT_CommandNotFound;
    }
    default:
        break;
    }

    // For other types of sender we don't support yet, fallback to the original dispatching route
    return ENDSTONE_HOOK_CALL_ORIGINAL(&MinecraftCommands::executeCommand, this, ctx, suppress_output);
}
