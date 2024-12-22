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

#include "endstone/detail/command/defaults/ban_command.h"

#include <string>
#include <vector>

#include "endstone/detail/server.h"

namespace endstone::detail {

BanCommand::BanCommand() : EndstoneCommand("ban")
{
    setDescription("commands.ban.description");
    setUsages("/ban <name: str> [reason: message]", "/ban <name: player> [reason: message]");
    setPermissions("endstone.command.ban");
}

bool BanCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    if (args.empty()) {
        return false;
    }

    const auto &server = entt::locator<EndstoneServer>::value();
    auto &ban_list = server.getBanList();
    auto name = args.front();

    if (ban_list.isBanned(name)) {
        sender.sendErrorMessage("Nothing changed. The player is already banned.");
        return true;
    }

    std::optional<std::string> reason;
    if (args.size() > 1) {
        reason = args[1];
    }

    std::optional<UUID> uuid;
    std::optional<std::string> xuid;

    const auto *player = server.getPlayer(name);
    if (player) {
        name = player->getName();
        uuid = player->getUniqueId();
        if (!player->getXuid().empty()) {
            xuid = player->getXuid();
        }
    }

    ban_list.addBan(name, uuid, xuid, reason, std::nullopt, sender.getName());
    sender.sendMessage(Translatable{"commands.ban.success", {name}});

    if (player) {
        player->kick("You have been banned from this server.");
    }
    return true;
}

}  // namespace endstone::detail
