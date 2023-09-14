//
// Created by Vincent on 07/09/2023.
//

#ifndef ENDSTONE_BEDROCK_COMMAND_SENDER_H
#define ENDSTONE_BEDROCK_COMMAND_SENDER_H

#include "bedrock/command_origin.h"
#include "endstone/command/command_sender.h"
#include "endstone/permission/permissible_base.h"

class BedrockCommandSender : public CommandSender {
public:
    explicit BedrockCommandSender(std::unique_ptr<CommandOrigin> origin) : origin_(std::move(origin)) {}

public:
    Server &getServer() const override;
    std::string getName() const override;

    const std::unique_ptr<CommandOrigin> &getOrigin() const;
    std::unique_ptr<CommandOrigin> takeOrigin();

public:
    static std::unique_ptr<CommandSender> fromCommandOrigin(std::unique_ptr<CommandOrigin> origin);

private:
    std::unique_ptr<CommandOrigin> origin_;
    PermissibleBase permissible_;
};

#endif // ENDSTONE_BEDROCK_COMMAND_SENDER_H
