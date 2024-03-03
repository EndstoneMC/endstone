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

#include "endstone/detail/command/command_map.h"

#include <algorithm>

#include "bedrock/command/command.h"
#include "bedrock/command/command_registry.h"
#include "endstone/detail/command/command_adapter.h"
#include "endstone/detail/command/defaults/version_command.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstoneCommandMap::EndstoneCommandMap(EndstoneServer &server) : server_(server)
{
    setDefaultCommands();
}

void EndstoneCommandMap::clearCommands()
{
    std::lock_guard lock(mutex_);
    for (const auto &item : known_commands_) {
        item.second->unregisterFrom(*this);
    }
    known_commands_.clear();
    setDefaultCommands();
}

Command *EndstoneCommandMap::getCommand(std::string name) const
{
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

    auto it = known_commands_.find(name);
    if (it == known_commands_.end()) {
        return nullptr;
    }

    return it->second.get();
}

void EndstoneCommandMap::setDefaultCommands()
{
    registerCommand(std::make_unique<VersionCommand>());
}

bool EndstoneCommandMap::registerCommand(std::shared_ptr<Command> command)
{
    std::lock_guard lock(mutex_);

    if (!command) {
        return false;
    }

    auto name = command->getName();
    auto it = known_commands_.find(name);
    if (it != known_commands_.end() && it->second->getName() == it->first) {
        // the name was registered and is not an alias
        return false;
    }

    auto &registry = server_.getMinecraftCommands().getRegistry();
    // TODO: configure the permission level and flags
    registry.registerCommand(name, command->getDescription().c_str(), CommandPermissionLevel::Any, {128}, {0});
    known_commands_.emplace(name, command);

    std::vector<std::string> registered_alias;
    for (const auto &alias : command->getAliases()) {
        if (known_commands_.find(alias) == known_commands_.end()) {
            registry.registerAlias(name, alias);
            known_commands_.emplace(alias, command);
            registered_alias.push_back(alias);
        }
    }

    // TODO: register the overloads from usage
    registry.registerOverload<CommandAdapter>(name.c_str(), {1, INT_MAX});

    command->setAliases(registered_alias);
    command->registerTo(*this);
    return true;
}
}  // namespace endstone::detail
