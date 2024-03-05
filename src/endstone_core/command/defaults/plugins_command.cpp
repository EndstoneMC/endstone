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

#include "endstone/detail/command/defaults/plugins_command.h"

#include <sstream>
#include <string>
#include <vector>

#include "endstone/detail/command/command_map.h"
#include "endstone/detail/server.h"
#include "endstone/detail/singleton.h"
#include "endstone/util/color_format.h"

namespace endstone::detail {
PluginsCommand::PluginsCommand() : Command("plugins")
{
    setDescription("Gets a list of plugins running on the server");
    setUsages("/plugins");
}

bool PluginsCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    sender.sendMessage("Plugins {}", getPluginList());
    return true;
}

std::string PluginsCommand::getPluginList() const
{
    std::stringstream ss;
    auto &server = Singleton<EndstoneServer>::getInstance();
    auto plugins = server.getPluginManager().getPlugins();

    int i = 0;
    for (auto *plugin : plugins) {
        if (i > 0) {
            ss << ColorFormat::WHITE;
            ss << ", ";
        }

        ss << (plugin->isEnabled() ? ColorFormat::GREEN : ColorFormat::RED);
        ss << plugin->getDescription().getName();
        i++;
    }

    return "(" + std::to_string(plugins.size()) + "): " + ss.str();
}

};  // namespace endstone::detail
