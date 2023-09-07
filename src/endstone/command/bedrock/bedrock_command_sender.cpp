//
// Created by Vincent on 07/09/2023.
//

#include "bedrock_command_sender.h"

#include "bedrock_server_command_sender.h"
#include "endstone/endstone.h"

std::unique_ptr<CommandSender> BedrockCommandSender::fromCommandOrigin(const CommandOrigin &origin)
{
    switch (origin.getOriginType()) {
    case CommandOriginType::Server:
        return std::move(std::make_unique<BedrockServerCommandSender>(origin));
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
    return command_origin_.getName();
}
