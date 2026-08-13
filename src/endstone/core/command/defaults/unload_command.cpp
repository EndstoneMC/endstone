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

#include "endstone/core/command/defaults/unload_command.h"

#include <string>
#include <vector>

#include "endstone/core/server.h"

namespace endstone::core {

UnloadCommand::UnloadCommand() : EndstoneCommand("unload")
{
    setDescription("Unloads a plugin.");
    setUsages("/unload", "/unload <plugin: str>");
    setPermissions("endstone.command.unload");
}

bool UnloadCommand::execute(const NotNull<CommandSender> &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    auto &server = EndstoneServer::getInstance();
    if (args.empty()) {
        server.unloadAllPlugins();
        sender->sendMessage("All plugins unloaded.");
        return true;
    }

    if (!server.unloadPlugin(args[0])) {
        sender->sendErrorMessage("Failed to unload plugin '{}'.", args[0]);
        return true;
    }

    sender->sendMessage("Plugin '{}' unloaded.", args[0]);
    return true;
}

}  // namespace endstone::core
