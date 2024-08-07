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

#include "bedrock/locale/i18n.h"
#include "bedrock/server/commands/command_registry.h"
#include "endstone/command/plugin_command.h"
#include "endstone/detail/command/bedrock_command.h"
#include "endstone/detail/command/command_adapter.h"
#include "endstone/detail/command/command_usage_parser.h"
#include "endstone/detail/command/defaults/plugins_command.h"
#include "endstone/detail/command/defaults/reload_command.h"
#include "endstone/detail/command/defaults/status_command.h"
#include "endstone/detail/command/defaults/version_command.h"
#include "endstone/detail/devtools/devtools_command.h"
#include "endstone/detail/permissions/default_permissions.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstoneCommandMap::EndstoneCommandMap(EndstoneServer &server) : server_(server)
{
    saveCommandRegistryState();
    setMinecraftCommands();
    setDefaultCommands();
}

void EndstoneCommandMap::clearCommands()
{
    std::lock_guard lock(mutex_);
    for (const auto &[name, command] : known_commands_) {
        command->unregisterFrom(*this);
    }
    known_commands_.clear();
    restoreCommandRegistryState();
    setMinecraftCommands();
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
    registerCommand(std::make_unique<PluginsCommand>());
    registerCommand(std::make_unique<ReloadCommand>());
    registerCommand(std::make_unique<StatusCommand>());
    registerCommand(std::make_unique<VersionCommand>());
#ifdef ENDSTONE_DEVTOOLS
    registerCommand(std::make_unique<DevToolsCommand>());
#endif
}

void EndstoneCommandMap::setMinecraftCommands()
{
    auto &registry = server_.getMinecraftCommands().getRegistry();

    // Override commands
    registry.signatures.erase("reload");

    std::unordered_map<std::string, std::vector<std::string>> command_aliases;
    for (const auto &[alias, command_name] : registry.aliases) {
        auto it = command_aliases.emplace(command_name, std::vector<std::string>()).first;
        it->second.push_back(alias);
    }

    auto *root = DefaultPermissions::registerPermission(
        "minecraft", nullptr, "Gives the user the ability to use all vanilla utilities and commands");
    auto *parent = DefaultPermissions::registerPermission(
        root->getName() + ".command", root, "Gives the user the ability to use all vanilla minecraft commands");

    for (const auto &[command_name, signature] : registry.signatures) {
        auto description = getI18n().get(signature.description, {}, nullptr);

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

        auto command = std::make_shared<BedrockCommand>(command_name, description, usages, aliases);
        command->registerTo(*this);

        known_commands_.emplace(signature.name, command);
        for (const auto &alias : aliases) {
            known_commands_.emplace(alias, command);
        }

        DefaultPermissions::registerPermission(parent->getName() + "." + command_name, parent,
                                               "Gives the user the ability to use the /" + command_name + " command",
                                               signature.permission_level > CommandPermissionLevel::Any
                                                   ? PermissionDefault::Operator
                                                   : PermissionDefault::True);
    }

    parent->recalculatePermissibles();
    root->recalculatePermissibles();
}

void EndstoneCommandMap::setPluginCommands()
{
    auto plugins = server_.getPluginManager().getPlugins();
    for (auto *plugin : plugins) {
        auto name = plugin->getName();
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        server_.getMinecraftCommands().getRegistry().addEnumValues("PluginName", {name});

        auto commands = plugin->getDescription().getCommands();
        for (const auto &command : commands) {
            registerCommand(std::make_unique<PluginCommand>(command, *plugin));
        }
    }
}

namespace {
std::unordered_map<std::string, CommandRegistry::HardNonTerminal> gTypeSymbols = {
    {"int", CommandRegistry::HardNonTerminal::Int},
    {"float", CommandRegistry::HardNonTerminal::Val},
    {"actor", CommandRegistry::HardNonTerminal::Selection},
    {"entity", CommandRegistry::HardNonTerminal::Selection},
    {"player", CommandRegistry::HardNonTerminal::Selection},
    {"target", CommandRegistry::HardNonTerminal::Selection},
    {"string", CommandRegistry::HardNonTerminal::Id},
    {"str", CommandRegistry::HardNonTerminal::Id},
    {"block_pos", CommandRegistry::HardNonTerminal::Position},
    {"vec3i", CommandRegistry::HardNonTerminal::Position},
    {"pos", CommandRegistry::HardNonTerminal::PositionFloat},
    {"vec3", CommandRegistry::HardNonTerminal::PositionFloat},
    {"vec3f", CommandRegistry::HardNonTerminal::PositionFloat},
    {"message", CommandRegistry::HardNonTerminal::MessageRoot},
    {"json", CommandRegistry::HardNonTerminal::JsonObject},
};
}  // namespace

bool EndstoneCommandMap::registerCommand(std::shared_ptr<Command> command)
{
    std::lock_guard lock(mutex_);

    if (!command) {
        return false;
    }

    auto name = command->getName();
    if (auto it = known_commands_.find(name); it != known_commands_.end() && it->second->getName() == it->first) {
        return false;  // the name was registered and is not an alias, we don't replace it
    }

    auto &registry = server_.getMinecraftCommands().getRegistry();
    registry.registerCommand(name, command->getDescription().c_str(), CommandPermissionLevel::Any,
                             CommandFlag::NotCheat, CommandFlag::None);
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
                auto data =
                    CommandParameterData({0}, &CommandRegistry::parse<CommandAdapter>, parameter.name.c_str(),
                                         CommandParameterDataType::Basic, nullptr, nullptr, 0, parameter.optional, -1);

                if (parameter.is_enum) {
                    const auto &enum_name = parameter.type;

                    // Add suffix if the enum already exists
                    std::string enum_name_final = enum_name;
                    int i = 0;
                    while (registry.enum_lookup.find(enum_name_final) != registry.enum_lookup.end()) {
                        enum_name_final = fmt::format("{}_{}", enum_name, ++i);
                    }
                    if (enum_name_final != enum_name) {
                        server_.getLogger().warning(
                            "Unable to register enum '{}' as it already exists, '{}' was registered instead.",
                            enum_name, enum_name_final);
                    }

                    // Add enum
                    auto symbol = static_cast<std::uint32_t>(registry.addEnumValues(enum_name_final, parameter.values));

                    // Check if the enum has been added
                    auto it = registry.enum_lookup.find(enum_name_final);
                    if (it == registry.enum_lookup.end()) {
                        server_.getLogger().error("Unable to register enum '{}'.", enum_name_final);
                        return false;
                    }
                    data.param_type = CommandParameterDataType::Enum;
                    data.enum_name = it->first.c_str();
                    data.enum_symbol = CommandRegistry::Symbol{symbol};
                    data.options = CommandParameterOption::EnumAutocompleteExpansion;
                }
                else if (parameter.type == "bool") {
                    static auto symbol = static_cast<std::uint32_t>(registry.addEnumValues("Boolean", {}));
                    data.param_type = CommandParameterDataType::Enum;
                    data.enum_name = "Boolean";
                    data.enum_symbol = CommandRegistry::Symbol{symbol};
                    data.options = CommandParameterOption::EnumAutocompleteExpansion;
                }
                else {
                    auto it = gTypeSymbols.find(std::string(parameter.type));
                    if (it == gTypeSymbols.end()) {
                        server_.getLogger().error("Error occurred when registering usage '{}'. Unsupported type '{}'.",
                                                  usage, parameter.type);
                        return false;
                    }
                    data.fallback_symbol = CommandRegistry::Symbol{it->second};
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

namespace {
struct {
    std::vector<CommandRegistry::Enum> enums;
    std::map<std::string, std::uint32_t> enum_lookup;
    std::map<std::string, CommandRegistry::Signature> signatures;
    std::map<std::string, std::string> aliases;
} gCommandRegistryState;
}  // namespace

void EndstoneCommandMap::saveCommandRegistryState() const
{
    auto &registry = server_.getMinecraftCommands().getRegistry();
    gCommandRegistryState.enums = registry.enums;
    gCommandRegistryState.enum_lookup = registry.enum_lookup;
    gCommandRegistryState.signatures = registry.signatures;
    gCommandRegistryState.aliases = registry.aliases;
}

void EndstoneCommandMap::restoreCommandRegistryState() const
{
    auto &registry = server_.getMinecraftCommands().getRegistry();
    registry.enums = gCommandRegistryState.enums;
    registry.enum_lookup = gCommandRegistryState.enum_lookup;
    registry.signatures = gCommandRegistryState.signatures;
    registry.aliases = gCommandRegistryState.aliases;
}

}  // namespace endstone::detail
