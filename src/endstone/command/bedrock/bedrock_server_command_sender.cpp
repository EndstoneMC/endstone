//
// Created by Vincent on 07/09/2023.
//

#include "bedrock_server_command_sender.h"

#include "endstone/server.h"

void BedrockServerCommandSender::sendMessage(const std::string &message) const
{
    getServer().getLogger()->info(message);
}
