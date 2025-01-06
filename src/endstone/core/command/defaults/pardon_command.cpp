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

#include "endstone/core/command/defaults/pardon_command.h"

#include <string>
#include <vector>

#include "endstone/core/server.h"

namespace endstone::core {

PardonCommand::PardonCommand() : EndstoneCommand("pardon")
{
    setDescription("Remove players from banlist.");
    setUsages("/pardon <name: str>");
    setAliases("unban");
    setPermissions("endstone.command.unban");
}

bool PardonCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    if (args.empty()) {
        return false;
    }

    const auto &server = entt::locator<EndstoneServer>::value();
    auto &ban_list = server.getBanList();
    const auto &name = args.front();

    const auto *entry = ban_list.getBanEntry(name);

    if (!entry) {
        sender.sendErrorMessage("Nothing changed. The player is not banned.");
        return true;
    }

    sender.sendMessage(Translatable{"commands.unban.success", {entry->getName()}});
    ban_list.removeBan(name);
    return true;
}

};  // namespace endstone::core
