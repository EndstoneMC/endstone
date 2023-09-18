// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/command/bedrock/bedrock_command_sender.h"

#include "endstone/command/bedrock/server_command_sender.h"
#include "endstone/endstone_server.h"
#include "endstone/logger.h"
#include "endstone/server.h"

std::unique_ptr<CommandSender> BedrockCommandSender::fromCommandOrigin(std::unique_ptr<CommandOrigin> origin) noexcept
{
    // TODO(command): support player command origin
    switch (origin->getOriginType()) {
    case CommandOriginType::Server:
        return std::move(std::make_unique<ServerCommandSender>(std::move(origin)));

    default:
        EndstoneServer::getInstance().getLogger().error("Command origin type: {} is not supported.",
                                                        static_cast<char>(origin->getOriginType()));
        return nullptr;
    }
}

Server &BedrockCommandSender::getServer() const noexcept
{
    return EndstoneServer::getInstance();
}

std::string BedrockCommandSender::getName() const noexcept
{
    return origin_->getName();
}

const std::unique_ptr<CommandOrigin> &BedrockCommandSender::getOrigin() const noexcept
{
    return origin_;
}

std::unique_ptr<CommandOrigin> BedrockCommandSender::takeOrigin() noexcept
{
    return std::move(origin_);
}
