//
// Created by Vincent on 07/09/2023.
//

#ifndef ENDSTONE_BEDROCK_SERVER_COMMAND_SENDER_H
#define ENDSTONE_BEDROCK_SERVER_COMMAND_SENDER_H

#include "bedrock_command_sender.h"
class BedrockServerCommandSender : public BedrockCommandSender {
public:
    using BedrockCommandSender::BedrockCommandSender;

public:
    void sendMessage(const std::string &message) const override;
};

#endif // ENDSTONE_BEDROCK_SERVER_COMMAND_SENDER_H
