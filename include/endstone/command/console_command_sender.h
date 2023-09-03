//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_CONSOLE_COMMAND_SENDER_H
#define ENDSTONE_CONSOLE_COMMAND_SENDER_H

#include "endstone/command/command_sender.h"
#include "endstone/endstone.h"

class ConsoleCommandSender : public CommandSender {

public:
    ConsoleCommandSender() = default;

    void sendMessage(const std::string &message) const override
    {
        getServer().getLogger()->info(message);
    }

    Server &getServer() const override
    {
        return Endstone::getServer();
    }

    std::string_view getName() const override
    {
        return "CONSOLE";
    }
};

#endif // ENDSTONE_CONSOLE_COMMAND_SENDER_H
