// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "endstone/command/defaults/help_command.h"

#include <algorithm>
#include <unordered_set>

#include "bedrock/i18n.h"
#include "endstone/chat_color.h"
#include "endstone/command/command_map.h"

HelpCommand::HelpCommand(const SimpleCommandMap &command_map) noexcept
    : EndstoneCommand("help"), command_map_(command_map)
{
    description_ = "Provides help/list of commands.";
    usages_ = {"/{command} [command: CommandName]", "/{command} <page: int>"};
    setAliases({"?"});
}

bool HelpCommand::execute(CommandSender &sender, const std::string &label,
                          const std::vector<std::string> &args) const noexcept
{
    if (!testPermission(sender)) {
        return true;
    }

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

void HelpCommand::displayHelpPage(const CommandSender &sender, int page) const noexcept
{
    auto commands = command_map_.getCommands();
    std::unordered_set<std::string> help_set;

    for (const auto &command : commands) {
        if (!command->testPermissionSilently(sender)) {
            continue;
        }

        help_set.insert(fmt::format("/{}: {}", command->getLabel(), command->getDescription()));
        for (const auto &alias : command->getAliases()) {
            help_set.insert(fmt::format("/{}: {}", alias, command->getDescription()));
        }
    }

    std::vector<std::string> helps(help_set.begin(), help_set.end());
    std::sort(helps.begin(), helps.end());

    int total_pages = (static_cast<int>(helps.size()) + NumCommandPerPage - 1) / NumCommandPerPage;
    if (page > total_pages) {
        page = total_pages;
    }
    else if (page < 1) {
        page = 1;
    }

    sender.sendMessage(ChatColor::Yellow + "--- Showing help page {} of {} ---", page, total_pages);
    auto begin = helps.begin() + (page - 1) * NumCommandPerPage;
    auto end = std::min(helps.begin() + page * NumCommandPerPage, helps.end());
    for (auto it = begin; it != end; ++it) {
        sender.sendMessage(*it);
    }
}

void HelpCommand::displayHelpCommand(const CommandSender &sender, const std::string &name) const noexcept
{
    auto *command = command_map_.getCommand(name);
    if (!command || !command->testPermissionSilently(sender)) {
        sender.sendMessage(
            ChatColor::Red +
                "Unknown command: {}. Please check that the command exists and that you have permission to use it.",
            name);
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
