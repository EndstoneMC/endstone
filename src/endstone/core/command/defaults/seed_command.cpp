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

#include "endstone/core/command/defaults/seed_command.h"

#include <entt/entt.hpp>

#include "endstone/color_format.h"
#include "endstone/core/command/command_map.h"
#include "endstone/core/server.h"

namespace endstone::core {

SeedCommand::SeedCommand() : EndstoneCommand("seed")
{
    setDescription("Outputs the world seed.");
    setUsages("/seed");
    setPermissions("endstone.command.seed");
}

bool SeedCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    const auto &server = EndstoneServer::getInstance();
    sender.sendMessage("Seed: {}", server.getLevel()->getSeed());
    return true;
}

};  // namespace endstone::core
