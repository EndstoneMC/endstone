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

#include "endstone/detail/command/defaults/pardon_ip_command.h"

#include <string>
#include <vector>

#include "bedrock/deps/raknet/socket_includes.h"
#include "endstone/detail/server.h"

namespace endstone::core {

PardonIpCommand::PardonIpCommand() : EndstoneCommand("pardon-ip")
{
    setDescription("Remove IP addresses from banlist.");
    setUsages("/pardon-ip <address: str>");
    setAliases("unban-ip");
    setPermissions("endstone.command.unbanip");
}

bool PardonIpCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    if (args.empty()) {
        return false;
    }

    const auto &server = entt::locator<EndstoneServer>::value();
    auto &ban_list = server.getIpBanList();
    const auto &address = args.front();

    if (sockaddr_in sa_v4{}; inet_pton(AF_INET, address.c_str(), &(sa_v4.sin_addr)) == 1) {
        // valid ipv4 address
    }
    else if (sockaddr_in6 sa_v6{}; inet_pton(AF_INET6, address.c_str(), &(sa_v6.sin6_addr)) == 1) {
        // valid ipv6 address
    }
    else {
        sender.sendErrorMessage(Translatable{"commands.unbanip.invalid"});
        return true;
    }

    const auto *entry = ban_list.getBanEntry(address);
    if (!entry) {
        sender.sendErrorMessage("Nothing changed. That IP is not banned.");
        return true;
    }

    sender.sendMessage(Translatable{"commands.unbanip.success", {entry->getAddress()}});
    ban_list.removeBan(address);
    return true;
}

};  // namespace endstone::core
