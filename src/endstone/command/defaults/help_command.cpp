//
// Created by Vincent on 01/09/2023.
//

#include "help_command.h"

#include "endstone/command/command_map.h"
#include "endstone/common.h"

HelpCommand::HelpCommand() : EndstoneCommand("help")
{
    description_ = "Provides help/list of commands.";
    usages_ = {"/help <page: int>", "/help [command: CommandName]"};
    setAliases({"?"});
}

bool HelpCommand::execute(CommandSender &sender, const std::string &label, const std::vector<std::string> &args) const
{
    // TODO: testPermission

    // TODO: implement this
    printf("Help command is called\n");
    return false;

    //    if (args.size() >= 2) {
    //        return false;
    //    }
    //
    //    std::string command;
    //    int pageNumber = 1;
    //
    //    if (!args.empty()) {
    //        auto arg0 = args.front();
    //        if (std::all_of(arg0.begin(), arg0.end(), isdigit)) {
    //            try {
    //                pageNumber = std::stoi(args.back());
    //            }
    //            catch (std::invalid_argument &exception) {
    //                pageNumber = 1;
    //            }
    //            if (pageNumber <= 0) {
    //                pageNumber = 1;
    //            }
    //        }
    //        else {
    //            command = arg0;
    //        }
    //    }
    //
    //    auto &commands = getCommandMap().getCommands();
    //    std::vector<std::shared_ptr<Command>> filtered;
    //    std::copy_if(commands.begin(), commands.end(), std::back_inserter(filtered), [](std::shared_ptr<Command> cmd)
    //    {
    //        return command.empty() || cmd->getLabel().find(command.begin(), command.size()) != npos;
    //    });
    //
    //    std::vector<std::string> full;
}
