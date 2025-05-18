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

#include "endstone/command/command_sender.h"

#include <entt/entt.hpp>

#include "bedrock/server/commands/command_origin.h"
#include "bedrock/server/commands/command_origin_loader.h"
#include "bedrock/world/actor/actor.h"
#include "endstone/core/command/console_command_sender.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"

std::shared_ptr<endstone::CommandSender> CommandOrigin::getEndstoneSender() const
{
    auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    switch (getOriginType()) {
    case CommandOriginType::DedicatedServer: {
        return std::static_pointer_cast<endstone::ConsoleCommandSender>(
            static_cast<endstone::core::EndstoneConsoleCommandSender &>(server.getCommandSender()).shared_from_this());
    }
    case CommandOriginType::Player:
    case CommandOriginType::Entity: {
        return std::static_pointer_cast<endstone::Actor>(getEntity()->getEndstoneActor().shared_from_this());
    }
    case CommandOriginType::Virtual:
    case CommandOriginType::CommandBlock:
    case CommandOriginType::MinecartCommandBlock:
        // TODO(permission): add BlockCommandSender, Entity and CommandMinecart
    default:
        return nullptr;
    }
}
