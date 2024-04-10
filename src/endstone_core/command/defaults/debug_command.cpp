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

#include "endstone/detail/command/defaults/debug_command.h"

#include <sstream>
#include <string>
#include <vector>

#include <entt/entt.hpp>

#include "endstone/color_format.h"
#include "endstone/detail/command/command_map.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

DebugCommand::DebugCommand() : EndstoneCommand("debug")
{
    setDescription("Toggle the debug mode");
    setUsages("/debug <toggle: bool>");
    setPermissions("endstone.command.debug");
}

bool DebugCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    auto &server = entt::locator<EndstoneServer>::value();
    if (&sender != &server.getCommandSender()) {
        return true;
    }

    auto enabled = args[0];
    std::transform(enabled.begin(), enabled.end(), enabled.begin(), [](unsigned char c) { return std::tolower(c); });

    if (enabled == "true") {
        server.getLogger().setLevel(Logger::Level::Debug);
        sender.sendMessage("Debug mode is enabled.");
    }
    else {
        server.getLogger().setLevel(Logger::Level::Info);
        sender.sendMessage("Debug mode is disabled.");
    }
    return true;
}

};  // namespace endstone::detail
