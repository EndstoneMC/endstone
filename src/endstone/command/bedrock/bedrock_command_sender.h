//
// Created by Vincent on 07/09/2023.
//

#ifndef ENDSTONE_BEDROCK_COMMAND_SENDER_H
#define ENDSTONE_BEDROCK_COMMAND_SENDER_H

#include "bedrock/command_origin.h"
#include "endstone/command/command_sender.h"

class BedrockCommandSender : public CommandSender {
public:
    explicit BedrockCommandSender(const CommandOrigin &command_origin) : command_origin_(command_origin) {}

public:
    Server &getServer() const override;
    std::string getName() const override;

public:
    static std::unique_ptr<CommandSender> fromCommandOrigin(const CommandOrigin &origin);

private:
    const CommandOrigin &command_origin_;
};

#endif // ENDSTONE_BEDROCK_COMMAND_SENDER_H
