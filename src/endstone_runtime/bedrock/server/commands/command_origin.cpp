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

#include "bedrock/server/commands/command_origin.h"

#include <bedrock/nbt/nbt_io.h>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "bedrock/server/commands/command_origin_loader.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/command/command_sender.h"
#include "endstone/detail/level/level.h"
#include "endstone/detail/server.h"

using endstone::detail::EndstoneLevel;
using endstone::detail::EndstonePlayer;
using endstone::detail::EndstoneServer;

endstone::CommandSender *CommandOrigin::toEndstone() const
{
    auto &server = entt::locator<EndstoneServer>::value();
    switch (getOriginType()) {
    case CommandOriginType::DedicatedServer: {
        return &server.getCommandSender();
    }
    case CommandOriginType::Player: {
        auto *entity = getEntity();
        endstone::Player &player = static_cast<::Player *>(entity)->getEndstonePlayer();
        return &player;
    }
    case CommandOriginType::Entity: {
        auto *entity = getEntity();
        endstone::Actor &actor = entity->getEndstoneActor();
        return &actor;
    }
    case CommandOriginType::Virtual:
        // TODO(command): we need ProxiedCommandSender, getOrigin will return the callee, getOutputReceiver will return
        //   the caller
    case CommandOriginType::CommandBlock:
    case CommandOriginType::MinecartCommandBlock:
        // TODO(permission): add BlockCommandSender, Entity and CommandMinecart
    default:
        return nullptr;
    }
}

std::unique_ptr<CommandOrigin> CommandOrigin::fromEndstone(endstone::CommandSender &sender)
{
    auto &server = entt::locator<EndstoneServer>::value();

    if (auto *console = sender.asConsole(); console) {
        CompoundTag tag;
        {
            tag.putByte("OriginType", static_cast<std::uint8_t>(CommandOriginType::DedicatedServer));
            tag.putString("RequestId", "00000000-0000-0000-0000-000000000000");
            tag.putByte("CommandPermissionLevel", static_cast<std::uint8_t>(CommandPermissionLevel::Owner));
            tag.putString("DimensionId", "overworld");
        }
        auto *level = static_cast<EndstoneLevel *>(server.getLevel());
        return CommandOriginLoader::load(tag, static_cast<ServerLevel &>(level->getHandle()));
    }

    if (auto *player = static_cast<EndstonePlayer *>(sender.asPlayer()); player) {
        CompoundTag tag;
        {
            tag.putByte("OriginType", static_cast<std::uint8_t>(CommandOriginType::Player));
            tag.putInt64("PlayerId", player->getHandle().getOrCreateUniqueID().raw_id);
        }
        return CommandOriginLoader::load(tag, static_cast<ServerLevel &>(player->getHandle().getLevel()));
    }

    return nullptr;
}
