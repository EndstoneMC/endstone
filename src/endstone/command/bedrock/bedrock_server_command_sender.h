//
// Created by Vincent on 07/09/2023.
//

#pragma once

#include "bedrock_command_sender.h"
class BedrockServerCommandSender : public BedrockCommandSender {
public:
    using BedrockCommandSender::BedrockCommandSender;

public:
    void sendMessage(const std::string &message) const override;
};

