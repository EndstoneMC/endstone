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
#include "endstone/core/server.h"
#include "endstone/event/player/player_command_event.h"
#include "endstone/event/server/server_command_event.h"
#include "endstone/runtime/hook.h"
#include "endstone/runtime/symbol.h"

using endstone::core::EndstoneServer;

MCRESULT MinecraftCommands::executeCommand(CommandContext &ctx, bool suppress_output) const
{
    const auto &server = entt::locator<EndstoneServer>::value();
    if (const auto sender = ctx.getOrigin().getEndstoneSender(); sender) {
        auto command_line = ctx.getCommand();

        if (auto *player = sender->asPlayer(); player) {
            endstone::PlayerCommandEvent event(*player, ctx.getCommand());
            server.getPluginManager().callEvent(event);

            if (event.isCancelled()) {
                return MCRESULT_CommandsDisabled;
            }
            command_line = event.getCommand();
            server.getLogger().info("{} issued server command: {}", player->getName(), command_line);
        }

        if (auto *console = sender->asConsole(); console) {
            endstone::ServerCommandEvent event(*console, command_line);
            server.getPluginManager().callEvent(event);

            if (event.isCancelled()) {
                return MCRESULT_CommandsDisabled;
            }
            command_line = event.getCommand();
        }

        if (server.dispatchCommand(*sender, command_line)) {
            return MCRESULT_Success;
        }
        return MCRESULT_CommandNotFound;
    }

    // For other types of sender we don't support yet, fallback to the original dispatching route
    return ENDSTONE_HOOK_CALL_ORIGINAL(&MinecraftCommands::executeCommand, this, ctx, suppress_output);
}

Command *MinecraftCommands::compileCommand(HashedString const &command_str, CommandOrigin &origin,
                                           CurrentCmdVersion command_version,
                                           std::function<void(const std::string &)> on_parser_error)
{
    return ENDSTONE_SYMCALL(&MinecraftCommands::compileCommand, this, command_str, origin, command_version,
                            on_parser_error);
}
