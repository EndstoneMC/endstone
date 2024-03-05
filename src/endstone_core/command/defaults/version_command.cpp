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

bool VersionCommand::execute(CommandSender &sender, const std::map<std::string, std::string> &args) const
{
    sender.sendMessage(ColorFormat::GOLD + "This server is running Endstone version: {}",
                       Singleton<EndstoneServer>::getInstance().getVersion());
    return true;
}

};  // namespace endstone::detail
