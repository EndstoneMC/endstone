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

#include <entt/entt.hpp>

#include "endstone/color_format.h"
#include "endstone/detail/command/command_map.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

VersionCommand::VersionCommand() : EndstoneCommand("version")
{
    setDescription("Gets the version of this server including any plugins in use.");
    setUsages("/version", "/version ()[plugin: PluginName]");
    setAliases("ver", "about");
    setPermissions("endstone.command.version");
}

bool VersionCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    auto &server = entt::locator<EndstoneServer>::value();
    if (args.empty()) {
        sender.sendMessage(ColorFormat::Gold + "This server is running {} version: {} (Minecraft: {})",
                           server.getName(), server.getVersion(), server.getMinecraftVersion());
    }
    else {
        auto target_name = args[0];
        std::transform(target_name.begin(), target_name.end(), target_name.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        auto plugins = server.getPluginManager().getPlugins();
        for (auto *plugin : plugins) {
            auto name = plugin->getName();
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

            if (name == target_name) {
                describeToSender(*plugin, sender);
                return true;
            }
        }

        sender.sendErrorMessage("This server is not running any plugin by that name.");
        sender.sendMessage("Use /plugins to get a list of plugins.");
        return false;
    }

    return true;
}

void VersionCommand::describeToSender(Plugin &plugin, CommandSender &sender) const
{
    const auto &desc = plugin.getDescription();
    sender.sendMessage(ColorFormat::Green + desc.getName() + ColorFormat::White + " v" + desc.getVersion());

    if (!desc.getDescription().empty()) {
        sender.sendMessage(desc.getDescription());
    }

    if (!desc.getWebsite().empty()) {
        sender.sendMessage("Website: " + ColorFormat::Green + desc.getWebsite());
    }

    if (!desc.getAuthors().empty()) {
        if (desc.getAuthors().size() == 1) {
            sender.sendMessage("Author: " + getNameList(desc.getAuthors()));
        }
        else {
            sender.sendMessage("Authors: " + getNameList(desc.getAuthors()));
        }
    }

    if (!desc.getContributors().empty()) {
        sender.sendMessage("Contributors: " + getNameList(desc.getContributors()));
    }
}

std::string VersionCommand::getNameList(const std::vector<std::string> &names) const
{
    std::string result;
    for (const auto &name : names) {
        if (!result.empty()) {
            result += ColorFormat::White;
            if (names.size() == 2) {
                result += ", ";
            }
            else {
                result += " and ";
            }
        }

        result += ColorFormat::Green;
        result += name;
    }

    return result;
}

};  // namespace endstone::detail
