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

#include "endstone/core/command/minecraft_command.h"

#include <utility>

#include "endstone/core/level/level.h"
#include "endstone/core/server.h"

namespace endstone::core {

MinecraftCommand::MinecraftCommand(std::string name, std::string description, std::vector<std::string> usages,
                                   std::vector<std::string> aliases, std::vector<std::string> permissions)
    : Command(std::move(name), std::move(description), std::move(usages), std::move(aliases), std::move(permissions))
{
    setPermissions("minecraft.command." + getName());
}

bool MinecraftCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    throw std::runtime_error("MinecraftCommand::execute is not supposed to be called. Something is wrong.");
}

}  // namespace endstone::core
