//
// Created by Vincent on 28/08/2023.
//

#include "bedrock_command.h"

#include "bedrock/i18n.h"

bool BedrockCommandPlaceHolder::execute(const CommandSender &sender, const std::string &label,
                                        const std::vector<std::string> &vector) const
{
    throw std::runtime_error("BedrockCommandPlaceHolder is not an executable command.");
}
