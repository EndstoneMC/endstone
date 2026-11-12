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
#include <string>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/core/command/command_map.h"
#include "endstone/core/command/console_command_sender.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/command/unknown_command_event.h"
#include "endstone/event/player/player_command_event.h"
#include "endstone/event/server/server_command_event.h"
#include "endstone/runtime/hook.h"

using endstone::core::EndstoneServer;

MCRESULT MinecraftCommands::executeCommand(CommandContext &ctx, bool suppress_output) const
{
    const auto &server = EndstoneServer::getInstance();
    std::optional<endstone::NotNull<endstone::CommandSender>> sender;
    std::string command_line;
    switch (ctx.getOrigin().getOriginType()) {
    case CommandOriginType::Player: {
        command_line = ctx.getCommand();
        auto player = ctx.getOrigin().getEntity()->getEndstoneActor<endstone::core::EndstonePlayer>();

        const auto e = std::make_shared<endstone::PlayerCommandEvent>(player, command_line);
        server.getPluginManager().callEvent(e);
        if (e->isCancelled()) {
            return MCRESULT_CommandsDisabled;
        }

        command_line = e->getCommand();
        if (server.logCommands()) {
            server.getLogger().info("{} issued server command: {}", player->getName(), command_line);
        }
        sender = player;
        break;
    }
    case CommandOriginType::DedicatedServer: {
        command_line = ctx.getCommand();
        auto console = server.getCommandSender();

        const auto e = std::make_shared<endstone::ServerCommandEvent>(console, command_line);
        server.getPluginManager().callEvent(e);
        if (e->isCancelled()) {
            return MCRESULT_CommandsDisabled;
        }

        command_line = e->getCommand();
        sender = console;
        break;
    }
    default:
        // For other types of sender we don't support yet, fallback to the original dispatching route
        return ENDSTONE_HOOK_CALL_ORIGINAL(&MinecraftCommands::executeCommand, this, ctx, suppress_output);
    }

    if (const auto handled = server.getCommandMap().tryDispatch(*sender, command_line)) {
        return *handled ? MCRESULT_Success : MCRESULT_CommandNotFound;
    }

    if (command_line.starts_with('/')) {
        command_line.erase(0, 1);
    }
    endstone::Message message{
        endstone::Translatable("commands.generic.unknown", {command_line.substr(0, command_line.find(' '))})};
    if (server.getEndstonePluginManager().isEventRegistered<endstone::UnknownCommandEvent>()) {
        const auto event = std::make_shared<endstone::UnknownCommandEvent>(*sender, command_line, std::move(message));
        server.getPluginManager().callEvent(event);
        if (const auto result = event->getMessage()) {
            (*sender)->sendErrorMessage(*result);
        }
    }
    else {
        (*sender)->sendErrorMessage(message);
    }
    return MCRESULT_CommandNotFound;
}
