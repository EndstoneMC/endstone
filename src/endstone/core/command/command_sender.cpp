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
#include "command_origin_wrapper.h"
#include "endstone/core/command/block_command_sender.h"
#include "endstone/core/command/console_command_sender.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"

std::shared_ptr<endstone::CommandSender> CommandOrigin::getEndstoneSender(CommandOutput &output) const
{
    auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    switch (getOriginType()) {
    case CommandOriginType::DedicatedServer: {
        return server.getCommandSenderPtr();
    }
    case CommandOriginType::Player:
    case CommandOriginType::Entity: {
        return getEntity()->getEndstoneActorPtr();
    }
    case CommandOriginType::CommandBlock: {
        return std::static_pointer_cast<endstone::BlockCommandSender>(
            std::make_shared<endstone::core::EndstoneBlockCommandSender>(*this, output));
    }
    case CommandOriginType::MinecartCommandBlock:
        // TODO(permission): add CommandMinecart
    default: {
        // Fallback to command origin via a wrapper for unsupported types
        auto sender = std::make_shared<endstone::core::CommandOriginWrapper>(*this, output);
        sender->recalculatePermissions();
        return sender;
    }
    }
}
