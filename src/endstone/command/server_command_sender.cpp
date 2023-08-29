//
// Created by Vincent on 28/08/2023.
//

#include "server_command_sender.h"

#include "endstone/endstone.h"

void ServerCommandSender::sendMessage(const std::string &message) const
{
    getServer().getLogger()->info(message);
}

Server &ServerCommandSender::getServer() const
{
    return Endstone::getServer();
}

std::string_view ServerCommandSender::getName() const
{
    return "CONSOLE";
}
