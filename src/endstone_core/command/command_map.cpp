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

#include <boost/algorithm/string.hpp>

#include "bedrock/locale/i18n.h"
#include "bedrock/server/commands/command_registry.h"
#include "endstone/command/plugin_command.h"
#include "endstone/detail/command/command_usage_parser.h"
#include "endstone/detail/command/defaults/ban_command.h"
#include "endstone/detail/command/defaults/ban_ip_command.h"
#include "endstone/detail/command/defaults/ban_list_command.h"
#include "endstone/detail/command/defaults/pardon_command.h"
#include "endstone/detail/command/defaults/pardon_ip_command.h"
#include "endstone/detail/command/defaults/plugins_command.h"
#include "endstone/detail/command/defaults/reload_command.h"
#include "endstone/detail/command/defaults/status_command.h"
#include "endstone/detail/command/defaults/version_command.h"
#include "endstone/detail/command/minecraft_command.h"
#include "endstone/detail/command/minecraft_command_adapter.h"
#include "endstone/detail/devtools/devtools_command.h"
#include "endstone/detail/permissions/default_permissions.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstoneCommandMap::EndstoneCommandMap(EndstoneServer &server) : server_(server)
{
    patchCommandRegistry();
    saveCommandRegistryState();
    setMinecraftCommands();
    setDefaultCommands();
}

bool EndstoneCommandMap::dispatch(CommandSender &sender, std::string command_line) const
{
    if (!command_line.empty() && command_line[0] == '/') {
        command_line = command_line.substr(1);
    }

    std::vector<std::string> args;
    split(args, command_line, boost::is_any_of(" "), boost::token_compress_on);
    if (args.empty()) {
        return false;
    }

    const auto *target = getCommand(args[0]);
    if (!target) {
        sender.sendErrorMessage(Translatable("commands.generic.unknown", {args[0]}));
        return false;
    }

    try {
        return target->execute(sender, std::vector(args.begin() + 1, args.end()));
    }
    catch (const std::exception &e) {
        server_.getLogger().error("Unhandled exception executing '{}': {}", command_line, e.what());
        return false;
    }
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
    registerCommand(std::make_unique<BanCommand>());
    registerCommand(std::make_unique<BanIpCommand>());
    registerCommand(std::make_unique<BanListCommand>());
    registerCommand(std::make_unique<PardonCommand>());
    registerCommand(std::make_unique<PardonIpCommand>());
    registerCommand(std::make_unique<PluginsCommand>());
    registerCommand(std::make_unique<ReloadCommand>());
    registerCommand(std::make_unique<StatusCommand>());
    registerCommand(std::make_unique<VersionCommand>());
#ifdef ENDSTONE_WITH_DEVTOOLS
    registerCommand(std::make_unique<DevToolsCommand>());
#endif
}

void EndstoneCommandMap::setMinecraftCommands()
{
    auto &registry = server_.getMinecraftCommands().getRegistry();

    std::unordered_map<std::string, std::vector<std::string>> command_aliases;
    for (const auto &[alias, command_name] : registry.aliases_) {
        auto it = command_aliases.emplace(command_name, std::vector<std::string>()).first;
        it->second.push_back(alias);
    }

    auto *root = DefaultPermissions::registerPermission(
        "minecraft", nullptr, "Gives the user the ability to use all vanilla utilities and commands");
    auto *parent = DefaultPermissions::registerPermission(
        root->getName() + ".command", root, "Gives the user the ability to use all vanilla minecraft commands");

    for (const auto &[command_name, signature] : registry.signatures_) {
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

        auto command = std::make_shared<CommandWrapper>(
            server_.getMinecraftCommands(),
            std::make_unique<MinecraftCommand>(command_name, description, usages, aliases));
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
    {"block_states", CommandRegistry::HardNonTerminal::BlockStateArray},
};
}  // namespace

bool EndstoneCommandMap::registerCommand(std::shared_ptr<Command> command)
{
    std::lock_guard lock(mutex_);
    auto &registry = server_.getMinecraftCommands().getRegistry();

    if (!command) {
        server_.getLogger().error("Unable to register a null command.");
        return false;
    }

    auto wrapped = std::make_shared<CommandWrapper>(server_.getMinecraftCommands(), command);
    command = wrapped;

    // Check if the command name is available
    auto name = command->getName();
    if (auto it = known_commands_.find(name); it != known_commands_.end() && it->second->getName() == it->first) {
        server_.getLogger().error("Unable to register command '{}' as it already exists.", name);
        return false;  // the name was registered and is not an alias, we don't replace it
    }

    // Check for available aliases
    std::vector<std::string> pending_aliases;
    for (const auto &alias : command->getAliases()) {
        if (known_commands_.find(alias) == known_commands_.end()) {
            pending_aliases.push_back(alias);
        }
    }

    // Check for available usages
    auto usages = command->getUsages();
    if (usages.empty()) {
        usages.push_back("/" + name);
    }
    std::vector<std::vector<CommandParameterData>> pending_param_data;
    for (const auto &usage : usages) {
        auto parser = CommandUsageParser(usage);
        std::string command_name;
        std::vector<CommandUsageParser::Parameter> parameters;
        std::string error_message;
        if (parser.parse(command_name, parameters, error_message)) {
            if (command_name != name) {
                server_.getLogger().warning("Unexpected command name '{}' in usage '{}', do you mean '{}'?",
                                            command_name, usage, name);
            }

            bool success = true;
            std::vector<CommandParameterData> param_data;
            for (const auto &parameter : parameters) {
                auto data =
                    CommandParameterData({0}, &CommandRegistry::parse<MinecraftCommandAdapter>, parameter.name.c_str(),
                                         CommandParameterDataType::Basic, nullptr, nullptr, 0, parameter.optional, -1);

                if (parameter.is_enum) {
                    const auto &enum_name = parameter.type;

                    // Add suffix if the enum already exists
                    std::string enum_name_final = enum_name;
                    int i = 0;
                    while (registry.enum_lookup_.find(enum_name_final) != registry.enum_lookup_.end()) {
                        enum_name_final = fmt::format("{}_{}", enum_name, ++i);
                    }
                    if (enum_name_final != enum_name) {
                        server_.getLogger().warning("Enum '{}' already exists, '{}' will be registered instead.",
                                                    enum_name, enum_name_final);
                    }

                    // Add enum
                    auto symbol = registry.addEnumValues(enum_name_final, parameter.values);

                    // Check if the enum has been added
                    auto it = registry.enum_lookup_.find(enum_name_final);
                    if (it == registry.enum_lookup_.end()) {
                        server_.getLogger().error("Unable to register enum '{}'.", enum_name_final);
                        throw std::runtime_error("Unreachable");
                    }
                    data.param_type = CommandParameterDataType::Enum;
                    data.enum_name_or_postfix = it->first.c_str();
                    data.enum_or_postfix_symbol = symbol;
                    data.options = CommandParameterOption::EnumAutocompleteExpansion;
                }
                else if (parameter.type == "bool") {
                    static auto symbol = static_cast<std::uint32_t>(registry.addEnumValues("Boolean", {}));
                    data.param_type = CommandParameterDataType::Enum;
                    data.enum_name_or_postfix = "Boolean";
                    data.enum_or_postfix_symbol = symbol;
                }
                else if (parameter.type == "block") {
                    static auto symbol = static_cast<std::uint32_t>(registry.addEnumValues("Block", {}));
                    data.param_type = CommandParameterDataType::Enum;
                    data.enum_name_or_postfix = "Block";
                    data.enum_or_postfix_symbol = symbol;
                }
                else {
                    auto it = gTypeSymbols.find(std::string(parameter.type));
                    if (it == gTypeSymbols.end()) {
                        server_.getLogger().error(
                            "Unable to register command '{}'. Unsupported type '{}' in usage '{}'.", name,
                            parameter.type, usage);
                        success = false;
                        break;  // early stop if any of the param in the usage is invalid
                    }
                    data.chained_subcommand_symbol = static_cast<int>(it->second);
                }
                param_data.push_back(data);
            }

            if (success) {
                pending_param_data.push_back(param_data);
            }
        }
        else {
            server_.getLogger().error("Error occurred when parsing usage '{}'. {}", usage, error_message);
        }
    }

    if (pending_param_data.empty()) {
        server_.getLogger().info("Unable to register command '{}': no valid usage", name);
        return false;
    }

    // Actual registration starts from here
    registry.registerCommand(name, command->getDescription().c_str(), CommandPermissionLevel::Any,
                             {static_cast<CommandFlagSize>(CommandCheatFlag::NotCheat)}, {0});
    known_commands_.emplace(name, wrapped);
    for (const auto &alias : pending_aliases) {
        registry.registerAlias(name, alias);
        known_commands_.emplace(alias, wrapped);
    }
    for (const auto &param_data : pending_param_data) {
        registry.registerOverload<MinecraftCommandAdapter>(name.c_str(), {1, INT_MAX}, param_data);
    }

    command->setAliases(pending_aliases);
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

void EndstoneCommandMap::patchCommandRegistry()
{
    std::lock_guard lock(mutex_);
    auto &registry = server_.getMinecraftCommands().getRegistry();

    // remove the vanilla `/reload` command (to be replaced by ours)
    registry.signatures_.erase("reload");
}

void EndstoneCommandMap::saveCommandRegistryState() const
{
    auto &registry = server_.getMinecraftCommands().getRegistry();
    gCommandRegistryState.enums = registry.enums_;
    gCommandRegistryState.enum_lookup = registry.enum_lookup_;
    gCommandRegistryState.signatures = registry.signatures_;
    gCommandRegistryState.aliases = registry.aliases_;
}

void EndstoneCommandMap::restoreCommandRegistryState() const
{
    auto &registry = server_.getMinecraftCommands().getRegistry();
    registry.enums_ = gCommandRegistryState.enums;
    registry.enum_lookup_ = gCommandRegistryState.enum_lookup;
    registry.signatures_ = gCommandRegistryState.signatures;
    registry.aliases_ = gCommandRegistryState.aliases;
}

}  // namespace endstone::detail
