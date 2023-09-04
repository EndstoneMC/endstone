//
// Created by Vincent on 01/09/2023.
//

#include "help_command.h"

#include "endstone/chat_color.h"
#include "endstone/command/command_map.h"
#include "endstone/common.h"

HelpCommand::HelpCommand(const SimpleCommandMap &command_map) : EndstoneCommand("help"), command_map_(command_map)
{
    description_ = "Provides help/list of commands.";
    usages_ = {"/{command} <page: int>", "/{command} [command: CommandName]"};
    setAliases({"?"});
}

bool HelpCommand::execute(const CommandSender &sender, const std::string &label,
                          const std::vector<std::string> &args) const
{
    // TODO: testPermission

    if (args.size() >= 2) {
        return false;
    }

    int page;
    std::string command;

    if (args.empty()) {
        command = "";
        page = 1;
    }
    else {
        try {
            page = std::stoi(args[0]);
        }
        catch (std::out_of_range &) {
            page = 1;
        }
        catch (std::invalid_argument &) {
            command = args[0];
        }
    }

    if (!command.empty()) {
        displayHelpCommand(sender, args[0]);
    }
    else {
        displayHelpPage(sender, page);
    }

    return true;
}

void HelpCommand::displayHelpPage(const CommandSender &sender, int page) const
{
    auto commands = command_map_.getCommands();
    std::unordered_set<std::string> help_set;

    for (const auto &command : commands) {
        // TODO: check if the caller has permission

        for (const auto &usage : command->getUsages()) {
            help_set.insert(fmt::format(usage, fmt::arg("command", command->getLabel())));

            for (const auto &alias : command->getAliases()) {
                help_set.insert(fmt::format(usage, fmt::arg("command", alias)));
            }
        }
    }

    std::vector<std::string> helps(help_set.begin(), help_set.end());
    std::sort(helps.begin(), helps.end());

    int total_pages = (helps.size() + COMMANDS_PER_PAGE - 1) / COMMANDS_PER_PAGE;
    if (page > total_pages) {
        page = total_pages;
    }
    else if (page < 1) {
        page = 1;
    }

    sender.sendMessage(ChatColor::Yellow + "--- Showing help page {} of {} ---", page, total_pages);
    auto begin = helps.begin() + (page - 1) * COMMANDS_PER_PAGE;
    auto end = std::min(helps.begin() + page * COMMANDS_PER_PAGE, helps.end());
    for (auto it = begin; it != end; ++it) {
        sender.sendMessage(*it);
    }
}

void HelpCommand::displayHelpCommand(const CommandSender &sender, const std::string &name) const
{
    auto command = command_map_.getCommand(name);
    if (!command) {
        sender.sendMessage(ChatColor::Red + "Unknown command: {}.", name);
        return;
    }

    sender.sendMessage(ChatColor::Yellow + "--- Showing help page of /{} ---", command->getLabel());

    sender.sendMessage("Description: {}", command->getDescription());

    auto usages = command->getUsages();
    if (!usages.empty()) {
        sender.sendMessage("Usage:");
        for (const auto &usage : command->getUsages()) {
            sender.sendMessage("- " + fmt::format(usage, fmt::arg("command", command->getLabel())));
        }
    }

    auto aliases = command->getAliases();
    if (!aliases.empty()) {
        sender.sendMessage("Aliases: {}", fmt::join(aliases.begin(), aliases.end(), ", "));
    }
}
