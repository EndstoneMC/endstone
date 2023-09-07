//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_BEDROCK_COMMAND_H
#define ENDSTONE_BEDROCK_COMMAND_H

#include "bedrock/command_context.h"
#include "bedrock/minecraft_commands.h"
#include "endstone/command/endstone_command.h"

class BedrockCommand : public EndstoneCommand {
public:
    using EndstoneCommand::EndstoneCommand;

    bool execute(const CommandSender &sender, const std::string &label,
                 const std::vector<std::string> &args) const override
    {
        throw std::runtime_error("BedrockCommand is not executable.");
    }
};

#endif // ENDSTONE_BEDROCK_COMMAND_H
