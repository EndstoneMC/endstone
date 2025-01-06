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

#include "endstone/detail/command/defaults/ban_ip_command.h"

#include <string>
#include <vector>

#include "bedrock/deps/raknet/socket_includes.h"
#include "endstone/detail/server.h"

namespace endstone::core {

BanIpCommand::BanIpCommand() : EndstoneCommand("ban-ip")
{
    setDescription("commands.banip.description");
    setUsages("/ban-ip <address: str> [reason: message]", "/ban-ip <name: player> [reason: message]");
    setPermissions("endstone.command.banip");
}

bool BanIpCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    if (args.empty()) {
        return false;
    }

    const auto &server = entt::locator<EndstoneServer>::value();
    auto &ban_list = server.getIpBanList();
    const auto &name_or_address = args.front();
    std::string address;
    const Player *player = nullptr;

    if (sockaddr_in sa_v4{}; inet_pton(AF_INET, name_or_address.c_str(), &(sa_v4.sin_addr)) == 1) {
        address = name_or_address;
    }
    else if (sockaddr_in6 sa_v6{}; inet_pton(AF_INET6, name_or_address.c_str(), &(sa_v6.sin6_addr)) == 1) {
        address = name_or_address;
    }
    else if (player = server.getPlayer(name_or_address); player) {
        address = player->getAddress().getHostname();
    }
    else {
        sender.sendErrorMessage(Translatable{"commands.banip.invalid"});
        return true;
    }

    if (ban_list.isBanned(address)) {
        sender.sendErrorMessage("Nothing changed. That IP is already banned.");
        return true;
    }

    std::optional<std::string> reason;
    if (args.size() > 1) {
        reason = args[1];
    }

    ban_list.addBan(address, reason, std::nullopt, sender.getName());
    if (player) {
        sender.sendMessage(Translatable{"commands.banip.success.players", {address, player->getName()}});
        player->kick("You have been IP banned from this server.");
    }
    else {
        sender.sendMessage(Translatable{"commands.banip.success", {address}});
    }

    for (const auto &online_player : server.getOnlinePlayers()) {
        if (online_player->getAddress().getHostname() == address) {
            online_player->kick("You have been IP banned from this server.");
        }
    }
    return true;
}

}  // namespace endstone::core
