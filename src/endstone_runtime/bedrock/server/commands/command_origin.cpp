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

#include <entt/entt.hpp>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/command/command_sender.h"
#include "endstone/detail/server.h"

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
    case CommandOriginType::Entity:
    case CommandOriginType::CommandBlock:
    case CommandOriginType::MinecartCommandBlock:
        // TODO(permission): add BlockCommandSender, Entity and CommandMinecart
    default:
        return nullptr;
    }
}
