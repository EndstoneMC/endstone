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

#include "bedrock/server/commands/command_origin.h"
#include "bedrock/server/commands/command_origin_loader.h"
#include "bedrock/world/actor/actor.h"
#include "command_origin_wrapper.h"
#include "endstone/core/command/block_command_sender.h"
#include "endstone/core/command/console_command_sender.h"
#include "endstone/core/command/proxied_command_sender.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"

endstone::NotNull<endstone::CommandSender> CommandOrigin::getEndstoneSender(
    std::shared_ptr<CommandOutput *> output) const
{
    auto &server = endstone::core::EndstoneServer::getInstance();
    switch (getOriginType()) {
    case CommandOriginType::DedicatedServer: {
        return server.getCommandSender();
    }
    case CommandOriginType::Player:
    case CommandOriginType::Entity: {
        auto *entity = getEntity();
        if (!entity) {
            throw std::runtime_error("Trying to access an actor that is no longer valid.");
        }
        return entity->getEndstoneActor();
    }
    case CommandOriginType::CommandBlock: {
        return std::make_shared<endstone::core::EndstoneBlockCommandSender>(*this, std::move(output));
    }
    case CommandOriginType::Virtual: {
        return std::make_shared<endstone::core::EndstoneProxiedCommandSender>(
            static_cast<const VirtualCommandOrigin &>(*this), std::move(output));
    }
    case CommandOriginType::MinecartCommandBlock:
        // TODO(permission): add CommandMinecart
    default: {
        // Fallback to command origin via a wrapper for unsupported types
        auto sender = std::make_shared<endstone::core::CommandOriginWrapper>(*this, std::move(output));
        sender->recalculatePermissions();
        return sender;
    }
    }
}
