// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/detail/command/defaults/ban_list_command.h"

#include <string>
#include <vector>

#include "endstone/detail/server.h"

namespace endstone::core {

BanListCommand::BanListCommand() : EndstoneCommand("banlist")
{
    setDescription("Lists the banned players or IP addresses.");
    setUsages("/banlist (ips|players)[type: BanListType]");
    setPermissions("endstone.command.banlist");
}

bool BanListCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    const bool show_players = args.empty() || args[0] == "players";
    const bool show_ips = args.empty() || args[0] == "ips";

    const auto &server = entt::locator<EndstoneServer>::value();

    if (show_players) {
        const auto entries = server.getBanList().getEntries();
        sender.sendMessage(Translatable{"commands.banlist.players", {std::to_string(entries.size())}});
        for (const auto &entry : entries) {
            sender.sendMessage("- {} was banned by {}: {}", entry->getName(), entry->getSource(), entry->getReason());
        }
    }

    if (show_ips) {
        const auto entries = server.getIpBanList().getEntries();
        sender.sendMessage(Translatable{"commands.banlist.ips", {std::to_string(entries.size())}});
        for (const auto &entry : entries) {
            sender.sendMessage("- {} was banned by {}: {}", entry->getAddress(), entry->getSource(),
                               entry->getReason());
        }
    }

    return true;
}

};  // namespace endstone::core
