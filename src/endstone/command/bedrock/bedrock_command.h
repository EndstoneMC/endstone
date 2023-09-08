//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_BEDROCK_COMMAND_H
#define ENDSTONE_BEDROCK_COMMAND_H

#include "bedrock/command_context.h"
#include "endstone/command/endstone_command.h"

class BedrockCommand : public EndstoneCommand {
public:
    using EndstoneCommand::EndstoneCommand;

    bool execute(CommandSender &sender, const std::string &label, const std::vector<std::string> &args) const override;

private:
    static std::string toCommandLine(const std::string &label, const std::vector<std::string> &args);
};

#endif // ENDSTONE_BEDROCK_COMMAND_H
