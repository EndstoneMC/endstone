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
    setDescription("Adds player to banlist.");
    setUsages("/ban <name: str>");
    setPermissions("endstone.command.ban");
}

bool BanCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    // TODO (ban): implement this
    return true;
}

};  // namespace endstone::detail
