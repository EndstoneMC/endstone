//
// Created by Vincent on 28/08/2023.
//

#include "bedrock_command_wrapper.h"

#include "bedrock/minecraft_commands.h"

BedrockCommandWrapper::BedrockCommandWrapper(const std::string &name) : EndstoneCommand(name) {}

bool BedrockCommandWrapper::execute(CommandSender &sender, const std::string &label,
                                    const std::vector<std::string> &vector) const
{
    return true;
}
