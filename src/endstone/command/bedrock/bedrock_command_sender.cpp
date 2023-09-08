//
// Created by Vincent on 07/09/2023.
//

#include "bedrock_command_sender.h"

#include "bedrock_server_command_sender.h"
#include "endstone/endstone.h"

std::unique_ptr<CommandSender> BedrockCommandSender::fromCommandOrigin(std::unique_ptr<CommandOrigin> origin)
{
    switch (origin->getOriginType()) {
    case CommandOriginType::Server:
        return std::move(std::make_unique<BedrockServerCommandSender>(std::move(origin)));
    default:
        throw std::runtime_error("Command origin type is not supported.");
    }
}

Server &BedrockCommandSender::getServer() const
{
    return Endstone::getServer();
}

std::string BedrockCommandSender::getName() const
{
    return origin_->getName();
}

const std::unique_ptr<CommandOrigin> &BedrockCommandSender::getOrigin() const
{
    return origin_;
}

std::unique_ptr<CommandOrigin> BedrockCommandSender::takeOrigin()
{
    return std::move(origin_);
}
