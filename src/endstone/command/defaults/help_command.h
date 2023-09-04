//
// Created by Vincent on 01/09/2023.
//

#ifndef ENDSTONE_HELP_COMMAND_H
#define ENDSTONE_HELP_COMMAND_H

#include "endstone/command/endstone_command.h"
#include "endstone/command/simple_command_map.h"

class HelpCommand : public EndstoneCommand {

private:
    const int COMMANDS_PER_PAGE = 8;

public:
    HelpCommand(const SimpleCommandMap &command_map);
    bool execute(const CommandSender &sender, const std::string &label,
                 const std::vector<std::string> &args) const override;

private:
    void displayHelpPage(const CommandSender &sender, int page) const;
    void displayHelpCommand(const CommandSender &sender, const std::string &name) const;

    const SimpleCommandMap &command_map_;
};

#endif // ENDSTONE_HELP_COMMAND_H
