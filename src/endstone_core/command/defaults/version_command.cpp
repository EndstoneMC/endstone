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

#include "endstone/detail/command/defaults/version_command.h"

#include "endstone/detail/command/command_map.h"
#include "endstone/detail/server.h"
#include "endstone/detail/singleton.h"
#include "endstone/util/color_format.h"

namespace endstone::detail {
VersionCommand::VersionCommand() : Command("version")
{
    setDescription("Gets the version of this server including any plugins in use.");
    setUsages("/version [plugin_name: string]");
    setAliases("ver", "about");
}

bool VersionCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (args.empty()) {
        sender.sendMessage(ColorFormat::GOLD + "This server is running Endstone version: {}",
                           Singleton<EndstoneServer>::getInstance().getVersion());
    }
    else {
        auto target_name = args[0];
        std::transform(target_name.begin(), target_name.end(), target_name.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        auto &server = Singleton<EndstoneServer>::getInstance();
        auto plugins = server.getPluginManager().getPlugins();
        for (auto *plugin : plugins) {
            auto name = plugin->getName();
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

            if (name == target_name) {
                const auto &desc = plugin->getDescription();
                sender.sendMessage(ColorFormat::GREEN + desc.getName() + ColorFormat::WHITE + " v" + desc.getVersion());
                return true;
            }
        }

        sender.sendErrorMessage("This server is not running any plugin by that name.");
        sender.sendMessage("Use /plugins to get a list of plugins.");
        return false;
    }

    return true;
}


};  // namespace endstone::detail
