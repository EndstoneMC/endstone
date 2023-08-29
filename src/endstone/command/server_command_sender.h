//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_SERVER_COMMAND_SENDER_H
#define ENDSTONE_SERVER_COMMAND_SENDER_H

#include "endstone/command/command_sender.h"

class ServerCommandSender : public CommandSender {

public:
    ServerCommandSender() = default;

    void sendMessage(const std::string &message) const override;
    Server &getServer() const override;
    std::string_view getName() const override;
};

#endif // ENDSTONE_SERVER_COMMAND_SENDER_H
