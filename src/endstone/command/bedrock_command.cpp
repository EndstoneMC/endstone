//
// Created by Vincent on 28/08/2023.
//

#include "bedrock_command.h"

#include "bedrock/minecraft_commands.h"

BedrockCommandPlaceHolder::BedrockCommandPlaceHolder(const std::string &name) : EndstoneCommand(name) {}

bool BedrockCommandPlaceHolder::execute(CommandSender &sender, const std::string &label,
                                        const std::vector<std::string> &vector) const
{
    return true;
}
