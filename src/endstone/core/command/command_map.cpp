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

#include "endstone/core/command/command_map.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "bedrock/locale/i18n.h"
#include "bedrock/server/commands/command_registry.h"
#include "endstone/command/plugin_command.h"
#include "endstone/core/command/command_usage_parser.h"
#include "endstone/core/command/defaults/ban_command.h"
#include "endstone/core/command/defaults/ban_ip_command.h"
#include "endstone/core/command/defaults/ban_list_command.h"
#include "endstone/core/command/defaults/pardon_command.h"
#include "endstone/core/command/defaults/pardon_ip_command.h"
#include "endstone/core/command/defaults/plugins_command.h"
#include "endstone/core/command/defaults/reload_command.h"
#include "endstone/core/command/defaults/seed_command.h"
#include "endstone/core/command/defaults/status_command.h"
#include "endstone/core/command/defaults/version_command.h"
#include "endstone/core/command/minecraft_command_adapter.h"
#include "endstone/core/command/minecraft_command_wrapper.h"
#include "endstone/core/devtools/devtools_command.h"
#include "endstone/core/permissions/default_permissions.h"
#include "endstone/core/server.h"

namespace endstone::core {

EndstoneCommandMap::EndstoneCommandMap(EndstoneServer &server) : server_(server)
{
    unregisterCommand("reload");
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

    const auto &registry = getHandle().getRegistry();
    const auto *signature = registry.findCommand(args[0]);
    if (!signature) {
        sender.sendErrorMessage(Translatable("commands.generic.unknown", {args[0]}));
        return false;
    }

    const auto target =
        std::make_shared<MinecraftCommandWrapper>(const_cast<MinecraftCommands &>(getHandle()), *signature);
    target->registerTo(*this);

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
    for (const auto &[name, command] : custom_commands_) {
        command->unregisterFrom(*this);
        unregisterCommand(command->getName());
    }
    custom_commands_.clear();
    setDefaultCommands();
}

std::shared_ptr<Command> EndstoneCommandMap::getCommand(std::string name) const
{
    std::ranges::transform(name, name.begin(), ::tolower);

    // Try to find the custom command we've registered
    if (const auto it = custom_commands_.find(name); it != custom_commands_.end()) {
        return it->second;
    }

    // Custom command not found, let's find it in CommandRegistry
    const auto &registry = getHandle().getRegistry();
    if (const auto *signature = registry.findCommand(name)) {
        auto command =
            std::make_shared<MinecraftCommandWrapper>(const_cast<MinecraftCommands &>(getHandle()), *signature);
        command->registerTo(*this);
        return command;
    }

    return nullptr;
}
MinecraftCommands &EndstoneCommandMap::getHandle()
{
    return server_.getServer().getMinecraft()->getCommands();
}

const MinecraftCommands &EndstoneCommandMap::getHandle() const
{
    return server_.getServer().getMinecraft()->getCommands();
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
    registerCommand(std::make_unique<SeedCommand>());
    registerCommand(std::make_unique<StatusCommand>());
    registerCommand(std::make_unique<VersionCommand>());
#ifdef ENDSTONE_WITH_DEVTOOLS
    registerCommand(std::make_unique<DevToolsCommand>());
#endif
}

void EndstoneCommandMap::setPluginCommands()
{
    const auto plugins = server_.getPluginManager().getPlugins();
    std::vector<std::string> plugin_names;
    for (auto *plugin : plugins) {
        auto name = plugin->getName();
        std::ranges::transform(name, name.begin(), ::tolower);
        plugin_names.emplace_back(name);

        auto commands = plugin->getDescription().getCommands();
        for (const auto &command : commands) {
            registerCommand(std::make_unique<PluginCommand>(command, *plugin));
        }
    }
    clearEnumValues("PluginName");
    getHandle().getRegistry().addEnumValues("PluginName", plugin_names);
}

void EndstoneCommandMap::unregisterCommand(std::string name)
{
    std::ranges::transform(name, name.begin(), ::tolower);
    auto &registry = getHandle().getRegistry();
    const auto *signature = registry.findCommand(name);
    if (!signature) {
        return;
    }

    // Remove enums introduced by custom commands
    if (custom_commands_.contains(name)) {
        for (const auto &overload : signature->overloads) {
            for (const auto &param : overload.params) {
                if (param.param_type != CommandParameterDataType::Enum || !param.enum_name_or_postfix) {
                    continue;
                }
                if (std::string_view(param.enum_name_or_postfix) == "Boolean" ||
                    std::string_view(param.enum_name_or_postfix) == "Block") {
                    continue;
                }
                clearEnumValues(param.enum_name_or_postfix);
            }
        }
    }

    // TODO: remove enum value constraints for name in enum name CommandName

    // Remove aliases
    for (auto it = registry.aliases_.begin(); it != registry.aliases_.end();) {
        if (it->second == name) {
            const auto &alias = it->first;
            removeEnumValueFromExisting("CommandName", alias);
            removeEnumValueFromExisting("CommandAliases", alias);
            it = registry.aliases_.erase(it);
        }
        else {
            ++it;
        }
    }

    // Remove name from CommandName
    removeEnumValueFromExisting("CommandName", name);

    // Remove command signature
    registry.signatures_.erase(name);
}

void EndstoneCommandMap::clearEnumValues(const std::string &enum_name)
{
    auto &registry = getHandle().getRegistry();
    if (!registry.enum_lookup_.contains(enum_name)) {
        return;
    }

    const auto enum_index = registry.enum_lookup_.at(enum_name);
    auto &enum_data = registry.enums_.at(enum_index);
    enum_data.values.clear();
}

void EndstoneCommandMap::removeEnumValueFromExisting(const std::string &enum_name, const std::string &enum_value)
{
    auto &registry = getHandle().getRegistry();
    if (!registry.enum_lookup_.contains(enum_name) || !registry.enum_value_lookup_.contains(enum_value)) {
        return;
    }

    const auto enum_index = registry.enum_lookup_.at(enum_name);
    const auto enum_value_index = registry.enum_value_lookup_.at(enum_value);

    auto &enum_data = registry.enums_.at(enum_index);
    for (auto it = enum_data.values.begin(); it != enum_data.values.end();) {
        if (it->first == CommandRegistry::Symbol::fromEnumValueIndex(enum_value_index).value()) {
            it = enum_data.values.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool EndstoneCommandMap::registerCommand(std::shared_ptr<Command> command)
{
    std::lock_guard lock(mutex_);
    auto &registry = getHandle().getRegistry();

    if (!command) {
        server_.getLogger().error("Unable to register a null command.");
        return false;
    }

    // Check if the command name is available
    auto name = command->getName();
    if (getCommand(name) != nullptr) {
        server_.getLogger().error("Unable to register command '{}' as it already exists.", name);
        return false;  // the name was registered and is not an alias, we don't replace it
    }

    // Check for available aliases
    std::vector<std::string> pending_aliases;
    for (const auto &alias : command->getAliases()) {
        if (getCommand(alias) == nullptr) {
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
        auto result = parser.parse();
        if (!result.has_value()) {
            server_.getLogger().error("Error occurred when parsing usage '{}'. {}", usage, result.error());
            continue;
        }

        auto &[command_name, parameters] = result.value();
        if (command_name != name) {
            server_.getLogger().warning("Unexpected command name '{}' in usage '{}', do you mean '{}'?", command_name,
                                        usage, name);
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
                while (true) {
                    const auto it = registry.enum_lookup_.find(enum_name_final);
                    if (it == registry.enum_lookup_.end()) {
                        break;
                    }
                    const auto enum_index = it->second;
                    if (registry.enums_.at(enum_index).values.empty()) {
                        break;
                    }
                    enum_name_final = fmt::format("{}_{}", enum_name, ++i);
                }
                if (enum_name_final != enum_name) {
                    server_.getLogger().warning("Enum '{}' already exists, '{}' will be registered instead.", enum_name,
                                                enum_name_final);
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
                static auto enum_index = registry.enum_lookup_.at("Boolean");
                data.param_type = CommandParameterDataType::Enum;
                data.enum_name_or_postfix = "Boolean";
                data.enum_or_postfix_symbol = CommandRegistry::Symbol::fromEnumIndex(enum_index).value();
            }
            else if (parameter.type == "block") {
                static auto enum_index = registry.enum_lookup_.at("Block");
                data.param_type = CommandParameterDataType::Enum;
                data.enum_name_or_postfix = "Block";
                data.enum_or_postfix_symbol = CommandRegistry::Symbol::fromEnumIndex(enum_index).value();
            }
            else {
                auto it = TYPE_SYMBOLS.find(std::string(parameter.type));
                if (it == TYPE_SYMBOLS.end()) {
                    server_.getLogger().error("Unable to register command '{}'. Unsupported type '{}' in usage '{}'.",
                                              name, parameter.type, usage);
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

    if (pending_param_data.empty()) {
        server_.getLogger().info("Unable to register command '{}': no valid usage", name);
        return false;
    }

    // Actual registration starts from here
    registry.registerCommand(name, command->getDescription().c_str(), CommandPermissionLevel::Any,
                             CommandCheatFlag::NotCheat, CommandUsageFlag::Normal);
    custom_commands_.emplace(name, command);
    for (const auto &alias : pending_aliases) {
        registry.registerAlias(name, alias);
        custom_commands_.emplace(alias, command);
    }
    for (const auto &param_data : pending_param_data) {
        registry.registerOverload<MinecraftCommandAdapter>(name.c_str(), {1, INT_MAX}, param_data);
    }

    command->setAliases(pending_aliases);
    command->registerTo(*this);
    return true;
}

const std::unordered_map<std::string, CommandRegistry::HardNonTerminal> EndstoneCommandMap::TYPE_SYMBOLS = {
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

}  // namespace endstone::core
