//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_CONSOLE_COMMAND_SENDER_H
#define ENDSTONE_CONSOLE_COMMAND_SENDER_H

#include "endstone/command/command_sender.h"

class ConsoleCommandSender : public CommandSender {

public:
    ConsoleCommandSender() = default;

    void sendMessage(const std::string &message) const override;
    Server &getServer() const override;
    std::string_view getName() const override;
};

#endif // ENDSTONE_CONSOLE_COMMAND_SENDER_H
