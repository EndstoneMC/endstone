//
// Created by Vincent on 01/09/2023.
//

#ifndef ENDSTONE_HELP_COMMAND_H
#define ENDSTONE_HELP_COMMAND_H

#include "endstone/command/endstone_command.h"

class HelpCommand : public EndstoneCommand {

public:
    HelpCommand();
    bool execute(CommandSender &sender, const std::string &label, const std::vector<std::string> &args) const override;
};

#endif // ENDSTONE_HELP_COMMAND_H
