//
// Created by Vincent on 28/08/2023.
//

#include "console_command_sender.h"

#include "endstone/chat_color.h"
#include "endstone/endstone.h"

void ConsoleCommandSender::sendMessage(const std::string &message) const
{
    getServer().getLogger()->info(message);
}

Server &ConsoleCommandSender::getServer() const
{
    return Endstone::getServer();
}

std::string_view ConsoleCommandSender::getName() const
{
    return "CONSOLE";
}
