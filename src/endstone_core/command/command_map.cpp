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
#include <string>
#include <unordered_map>
#include <vector>

#include "bedrock/command/command.h"
#include "bedrock/command/command_registry.h"
#include "bedrock/i18n.h"
#include "bedrock/type_id.h"
#include "endstone/detail/command/command_adapter.h"
#include "endstone/detail/command/command_usage_parser.h"
#include "endstone/detail/command/command_view.h"
#include "endstone/detail/command/defaults/plugins_command.h"
#include "endstone/detail/command/defaults/version_command.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstoneCommandMap::EndstoneCommandMap(EndstoneServer &server) : server_(server)
{
    setMinecraftCommands();
    setDefaultCommands();
}

// void EndstoneCommandMap::clearCommands()
// {
//     std::lock_guard lock(mutex_);
//     for (const auto &item : known_commands_) {
//         item.second->unregisterFrom(*this);
//     }
//     known_commands_.clear();
//     setDefaultCommands();
// }

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
    registerCommand(std::make_unique<PluginsCommand>());
    registerCommand(std::make_unique<VersionCommand>());
}

void EndstoneCommandMap::setMinecraftCommands()
{
    auto &commands = server_.getMinecraftCommands();
    auto &registry = commands.getRegistry();

    std::unordered_map<std::string, std::vector<std::string>> command_aliases;
    for (const auto &[alias, command_name] : registry.aliases) {
        auto it = command_aliases.emplace(command_name, std::vector<std::string>()).first;
        it->second.push_back(alias);
    }

    for (const auto &[command_name, signature] : registry.commands) {
        auto description = I18n::get(signature.description, {}, nullptr);

        std::vector<std::string> usages;
        usages.reserve(signature.overloads.size());
        for (const auto &overload : signature.overloads) {
            usages.push_back(registry.describe(signature, overload));
        }

        std::vector<std::string> aliases;
        auto it = command_aliases.find(command_name);
        if (it != command_aliases.end()) {
            aliases.insert(aliases.end(), it->second.begin(), it->second.end());
        }

        auto command = std::make_unique<CommandView>(command_name, description, usages, aliases);
        command->registerTo(*this);

        known_commands_.emplace(signature.name, std::move(command));
    }
}

namespace {
std::unordered_map<std::string, CommandRegistry::Symbol> gTypeSymbols = {
    {"int", CommandRegistry::Symbol::INT},
    {"float", CommandRegistry::Symbol::FLOAT},
    {"actor", CommandRegistry::Symbol::SELECTOR},
    {"player", CommandRegistry::Symbol::SELECTOR},
    {"target", CommandRegistry::Symbol::SELECTOR},
    {"string", CommandRegistry::Symbol::STRING},
    {"str", CommandRegistry::Symbol::STRING},
    {"block_pos", CommandRegistry::Symbol::POSITION},
    {"vec3i", CommandRegistry::Symbol::POSITION},
    {"pos", CommandRegistry::Symbol::POSITION_FLOAT},
    {"vec3", CommandRegistry::Symbol::POSITION_FLOAT},
    {"vec3f", CommandRegistry::Symbol::POSITION_FLOAT},
    {"message", CommandRegistry::Symbol::MESSAGE},
    {"json", CommandRegistry::Symbol::JSON},
};
}  // namespace

bool EndstoneCommandMap::registerCommand(std::shared_ptr<Command> command)
{
    std::lock_guard lock(mutex_);

    if (!command) {
        return false;
    }

    auto name = command->getName();
    {
        auto it = known_commands_.find(name);
        if (it != known_commands_.end() && it->second->getName() == it->first) {
            return false;  // the name was registered and is not an alias, we don't replace it
        }
    }

    auto &registry = server_.getMinecraftCommands().getRegistry();
    // TODO(permission): configure the permission level and flags
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

    for (const auto &usage : command->getUsages()) {
        auto parser = CommandUsageParser(usage);
        std::string command_name;
        std::vector<CommandUsageParser::Parameter> parameters;
        std::string error_message;
        if (parser.parse(command_name, parameters, error_message)) {
            if (command_name != name) {
                server_.getLogger().warning("Unexpected command name '{}' in usage '{}', do you mean '{}'?",
                                            command_name, usage, name);
            }

            std::vector<CommandParameterData> param_data;
            for (const auto &parameter : parameters) {
                auto data = CommandParameterData({0}, &CommandRegistry::parse<CommandAdapter>, parameter.name.c_str(),
                                                 CommandParameterDataType::Default, nullptr, nullptr, 0,
                                                 parameter.optional, -1);

                if (parameter.is_enum) {
                    auto symbol = registry.addEnumValues(parameter.type, parameter.values);
                    auto it = registry.enum_symbol_index.find(parameter.type);
                    if (it == registry.enum_symbol_index.end()) {
                        server_.getLogger().error("Unable to register enum '{}'.", parameter.type);
                        return false;
                    }
                    data.type = CommandParameterDataType::Enum;
                    data.enum_name = it->first.c_str();
                    data.enum_symbol = {symbol};
                }
                else {
                    auto it = gTypeSymbols.find(std::string(parameter.type));
                    if (it == gTypeSymbols.end()) {
                        server_.getLogger().error("Error occurred when registering usage '{}'. Unsupported type '{}'.",
                                                  usage, parameter.type);
                        return false;
                    }
                    data.fallback_symbol = it->second;
                }

                param_data.push_back(data);
            }

            registry.registerOverload<CommandAdapter>(name.c_str(), {1, INT_MAX}, param_data);
        }
        else {
            server_.getLogger().error("Error occurred when parsing usage '{}'. {}", usage, error_message);
            continue;
        }
    }

    command->setAliases(registered_alias);
    command->registerTo(*this);
    return true;
}

void EndstoneCommandMap::addEnumValues(const std::string &name, const std::vector<std::string> &values)
{
    server_.getMinecraftCommands().getRegistry().addEnumValues(name, values);
}

}  // namespace endstone::detail
