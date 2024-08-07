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

#include "endstone/detail/command/defaults/reload_command.h"

#include <entt/entt.hpp>

#include "endstone/color_format.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

ReloadCommand::ReloadCommand() : EndstoneCommand("reload")
{
    setDescription("Reloads the server configuration, functions, scripts and plugins.");
    setUsages("/reload");
    setPermissions("endstone.command.reload");
    setAliases("rl");
}

bool ReloadCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    if (!sender.asConsole()) {
        sender.sendErrorMessage("This command can only be executed from the console");
        return true;
    }

    auto &server = entt::locator<EndstoneServer>::value();
    server.reload();
    server.broadcast(ColorFormat::Green + "Reload complete.", Server::BroadcastChannelAdmin);
    return true;
}

}  // namespace endstone::detail
