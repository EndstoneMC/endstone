//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_BEDROCK_COMMAND_H
#define ENDSTONE_BEDROCK_COMMAND_H

#include "bedrock/command_context.h"
#include "bedrock/minecraft_commands.h"
#include "endstone_command.h"

class BedrockCommandPlaceHolder : public EndstoneCommand {
public:
    explicit BedrockCommandPlaceHolder(const std::string &name);

    bool execute(const CommandSender &sender, const std::string &label,
                 const std::vector<std::string> &args) const override;
};

#endif // ENDSTONE_BEDROCK_COMMAND_H
