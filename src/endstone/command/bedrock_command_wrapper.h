//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_BEDROCK_COMMAND_WRAPPER_H
#define ENDSTONE_BEDROCK_COMMAND_WRAPPER_H

#include "bedrock/command_context.h"
#include "bedrock/minecraft_commands.h"
#include "endstone_command.h"

class BedrockCommandWrapper : public EndstoneCommand {
public:
    explicit BedrockCommandWrapper(const std::string &name);

    bool execute(CommandSender &sender, const std::string &label, const std::vector<std::string> &args) const override;
    bool execute(MinecraftCommands &minecraft_commands, MinecraftCommands::Result &result, CommandContext &command_ctx,
                 bool flag);
};

#endif // ENDSTONE_BEDROCK_COMMAND_WRAPPER_H
