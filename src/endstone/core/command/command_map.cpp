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
#include <cctype>
#include <format>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "bedrock/locale/i18n.h"
#include "bedrock/server/commands/command_registry.h"
#include "bedrock/server/server_instance.h"
#include "endstone/command/plugin_command.h"
#include "endstone/core/command/command_usage_parser.h"
#include "endstone/core/command/defaults/ban_command.h"
#include "endstone/core/command/defaults/ban_ip_command.h"
#include "endstone/core/command/defaults/ban_list_command.h"
#include "endstone/core/command/defaults/pardon_command.h"
#include "endstone/core/command/defaults/pardon_ip_command.h"
#include "endstone/core/command/defaults/plugins_command.h"
#include "endstone/core/command/defaults/reload_command.h"
#include "endstone/core/command/defaults/restart_command.h"
#include "endstone/core/command/defaults/seed_command.h"
#include "endstone/core/command/defaults/status_command.h"
#include "endstone/core/command/defaults/version_command.h"
#include "endstone/core/command/minecraft_command_adapter.h"
#include "endstone/core/command/minecraft_command_wrapper.h"
#include "endstone/core/devtools/devtools_command.h"
#include "endstone/core/permissions/default_permissions.h"
#include "endstone/core/server.h"
#include "endstone/event/command/unknown_command_event.h"

namespace endstone::core {

EndstoneCommandMap::EndstoneCommandMap(EndstoneServer &server) : server_(server)
{
    setDefaultCommands();
}

void EndstoneCommandMap::attach()
{
    std::lock_guard lock(mutex_);
    unregisterCommand("reload");
    allowCommandsWithoutCheats();
    attached_ = true;

    for (auto &deferred : std::exchange(deferred_commands_, {})) {
        const auto &command = deferred.command;
        std::erase_if(custom_commands_, [&](const auto &entry) { return entry.second.get() == command.get(); });
        command->unregisterFrom(*this);
        command->setName(deferred.label);
        command->setAliases(deferred.aliases);
        registerCommand(deferred.fallback_prefix, command);
    }
}

bool EndstoneCommandMap::registerDeferred(const std::string &fallback_prefix, NotNull<Command> command)
{
    const auto label = command->getName();
    const auto aliases = command->getAliases();
    const auto claim = claimNames(fallback_prefix, command);
    if (!claim) {
        return false;
    }

    custom_commands_.emplace(claim->name, command);
    for (const auto &alias : claim->aliases) {
        custom_commands_.emplace(alias, command);
    }
    for (const auto &alias : claim->prefixed) {
        custom_commands_.insert_or_assign(alias, command);
    }
    command->setName(claim->name);
    command->setAliases(claim->aliases);
    command->registerTo(*this);
    deferred_commands_.push_back({fallback_prefix, label, aliases, command});
    return claim->name == boost::to_lower_copy(boost::trim_copy(label));
}

std::optional<EndstoneCommandMap::Claim> EndstoneCommandMap::claimNames(const std::string &fallback_prefix,
                                                                        const NotNull<Command> &command) const
{
    const auto label = boost::to_lower_copy(boost::trim_copy(command->getName()));
    const auto qualified_label = fallback_prefix + ":" + label;
    const auto is_name = [&](const std::string &key) {
        const auto held = getCommand(key);
        return held != nullptr && held->getName() == key;
    };
    const bool label_free = getCommand(label) == nullptr;
    const bool qualified_label_is_name = is_name(qualified_label);
    if (!label_free && qualified_label_is_name) {
        server_.getLogger().error("Unable to register command '{}' as it already exists.", label);
        return std::nullopt;
    }

    Claim claim;
    claim.name = label_free ? label : qualified_label;
    if (qualified_label != claim.name && !qualified_label_is_name) {
        claim.prefixed.push_back(qualified_label);
    }
    for (auto alias : command->getAliases()) {
        alias = boost::to_lower_copy(boost::trim_copy(alias));
        const auto qualified_alias = fallback_prefix + ":" + alias;
        if (alias != label && getCommand(alias) == nullptr &&
            std::ranges::find(claim.aliases, alias) == claim.aliases.end()) {
            claim.aliases.push_back(alias);
        }
        if (qualified_alias != claim.name && !is_name(qualified_alias) &&
            std::ranges::find(claim.prefixed, qualified_alias) == claim.prefixed.end()) {
            claim.prefixed.push_back(qualified_alias);
        }
    }
    return claim;
}

void EndstoneCommandMap::allowCommandsWithoutCheats()
{
    // Like Paper, permissions decide who may run a command, not whether the world has cheats on.
    auto &registry = getHandle().getRegistry();
    for (auto &signature : registry.signatures_ | std::views::values) {
        signature.flags |= CommandCheatFlag::NotCheat;
    }
    for (auto &value : registry.constrained_values_) {
        std::erase_if(value.constraints, [&](auto index) {
            return registry.semantic_constraints_[index] == SemanticConstraint::RequiresCheatsEnabled;
        });
    }
}

bool EndstoneCommandMap::dispatch(const NotNull<CommandSender> &sender, std::string command_line) const
{
    return tryDispatch(sender, std::move(command_line)).has_value();
}

std::optional<bool> EndstoneCommandMap::tryDispatch(const NotNull<CommandSender> &sender,
                                                    std::string command_line) const
{
    if (!command_line.empty() && command_line[0] == '/') {
        // remove the leading slash if present
        command_line = command_line.substr(1);
    }

    // split the command line by space
    std::vector<std::string> args;
    split(args, command_line, boost::is_any_of(" "), boost::token_compress_on);
    if (args.empty()) {
        return std::nullopt;
    }

    // get the command by name
    auto name = args[0];
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    const auto command = getCommand(name);
    if (!command) {
        return std::nullopt;
    }

    if (!attached_) {
        try {
            return command->execute(sender, std::vector(args.begin() + 1, args.end()));
        }
        catch (const std::exception &e) {
            server_.getLogger().error("Unhandled exception executing '{}': {}", command_line, e.what());
            return false;
        }
    }

    if (!custom_commands_.contains(name)) {
        // This is a vanilla command
        try {
            return command->execute(sender, std::vector(args.begin() + 1, args.end()));
        }
        catch (const std::exception &e) {
            server_.getLogger().error("Unhandled exception executing '{}': {}", command_line, e.what());
            return false;
        }
    }
    else {
        // This is a custom command
        const auto command_origin = MinecraftCommandWrapper::getCommandOrigin(sender);
        if (!command_origin) {
            sender->sendErrorMessage("Unsupported sender type!");
            return false;
        }
        const auto *compiled = getHandle().compileCommand(  //
            command_line, *command_origin, CurrentCmdVersion::Latest,
            [&sender](auto const &err) { sender->sendErrorMessage(err); });
        if (!compiled) {
            return false;
        }
        try {
            return command->execute(sender, static_cast<const MinecraftCommandAdapter *>(compiled)->args_);
        }
        catch (const std::exception &e) {
            server_.getLogger().error("Unhandled exception executing '{}': {}", command_line, e.what());
            return false;
        }
    }
}

void EndstoneCommandMap::clearCommands()
{
    std::lock_guard lock(mutex_);
    for (const auto &command : custom_commands_ | std::views::values) {
        command->unregisterFrom(*this);
        if (attached_) {
            unregisterCommand(command->getName());
        }
    }
    custom_commands_.clear();
    deferred_commands_.clear();
    setDefaultCommands();
}

Nullable<Command> EndstoneCommandMap::getCommand(std::string name) const
{
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    // Try to find the custom command we've registered
    if (const auto it = custom_commands_.find(name); it != custom_commands_.end()) {
        return it->second;
    }

    if (!attached_) {
        return nullptr;
    }

    // Custom command not found, let's find it in CommandRegistry
    const auto &registry = getHandle().getRegistry();
    if (const auto *signature = registry.findCommand(name)) {
        std::lock_guard lock(mutex_);
        if (const auto it = vanilla_commands_.find(signature->name); it != vanilla_commands_.end()) {
            return it->second;
        }
        auto command = std::make_shared<MinecraftCommandWrapper>(getHandle(), *signature);
        command->registerTo(*this);
        vanilla_commands_.emplace(signature->name, command);
        return command;
    }
    return nullptr;
}

MinecraftCommands &EndstoneCommandMap::getHandle() const
{
    return server_.getServer().getMinecraft()->getCommands();
}

void EndstoneCommandMap::setDefaultCommands()
{
    registerCommand<BanCommand>();
    registerCommand<BanIpCommand>();
    registerCommand<BanListCommand>();
    registerCommand<PardonCommand>();
    registerCommand<PardonIpCommand>();
    registerCommand<PluginsCommand>();
    registerCommand<ReloadCommand>();
    registerCommand<RestartCommand>();
    registerCommand<SeedCommand>();
    registerCommand<StatusCommand>();
    registerCommand<VersionCommand>();
#ifdef ENDSTONE_WITH_DEVTOOLS
    registerCommand<DevToolsCommand>();
#endif
}

void EndstoneCommandMap::setPluginNames()
{
    const auto plugins = server_.getPluginManager().getPlugins();
    std::vector<std::string> plugin_names;
    for (const auto &plugin : plugins) {
        auto name = plugin->getName();
        std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
        plugin_names.emplace_back(name);
    }
    clearEnumValues("PluginName");
    getHandle().getRegistry().addEnumValues("PluginName", plugin_names);
}

void EndstoneCommandMap::unregisterCommand(std::string name)
{
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    auto &registry = getHandle().getRegistry();
    const auto *signature = registry.findCommand(name);
    if (!signature) {
        return;
    }

    // Remove enums introduced by custom commands
    if (const auto it = custom_enums_.find(name); it != custom_enums_.end()) {
        for (const auto &enum_name : it->second) {
            clearEnumValues(enum_name);
        }
        custom_enums_.erase(it);
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

bool EndstoneCommandMap::registerCommand(NotNull<Command> command)
{
    std::string fallback_prefix = "endstone";
    if (command->is<PluginCommand>()) {
        if (const auto plugin = command.cast<PluginCommand>()->getPlugin()) {
            fallback_prefix = plugin->getName();
        }
    }
    return registerCommand(std::move(fallback_prefix), command);
}

bool EndstoneCommandMap::registerCommand(std::string fallback_prefix, NotNull<Command> command)
{
    std::lock_guard lock(mutex_);

    if (command.get() == nullptr) {
        server_.getLogger().error("Unable to register a null command.");
        return false;
    }

    boost::trim(fallback_prefix);
    std::ranges::transform(fallback_prefix, fallback_prefix.begin(), [](unsigned char c) { return std::tolower(c); });

    if (!attached_) {
        return registerDeferred(fallback_prefix, command);
    }

    auto &registry = getHandle().getRegistry();

    const auto name = command->getName();
    const auto claim = claimNames(fallback_prefix, command);
    if (!claim) {
        return false;
    }

    // Check for available usages
    auto usages = command->getUsages();
    if (usages.empty()) {
        usages.push_back("/" + name);
    }
    std::vector<std::vector<CommandParameterData>> pending_param_data;
    std::vector<std::string> created_enums;
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
        std::vector<std::string> usage_enums;
        for (const auto &parameter : parameters) {
            auto data = CommandParameterData({0}, &CommandRegistry::ParseRuleFor<MinecraftCommandAdapter>::instance,
                                             parameter.name.c_str(), CommandParameterDataType::Basic, nullptr, nullptr,
                                             0, parameter.optional, -1);

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
                    enum_name_final = std::format("{}_{}", enum_name, ++i);
                }
                if (enum_name_final != enum_name) {
                    server_.getLogger().warning("Enum '{}' already exists, '{}' will be registered instead.", enum_name,
                                                enum_name_final);
                }

                // Add enum
                auto symbol = registry.addEnumValues(enum_name_final, parameter.values);
                usage_enums.push_back(enum_name_final);

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
            else if (parameter.type == "entity_type") {
                static auto enum_index = registry.enum_lookup_.at("EntityType");
                data.param_type = CommandParameterDataType::Enum;
                data.enum_name_or_postfix = "EntityType";
                data.enum_or_postfix_symbol = CommandRegistry::Symbol::fromEnumIndex(enum_index).value();
            }
            else {
                if (parameter.type == "message" && (&parameter != &parameters.back())) {
                    server_.getLogger().error(
                        "Unable to register command '{}'. Argument '{}' of 'message' type must be "
                        "the last argument in usage '{}'.",
                        name, parameter.name, usage);
                    success = false;
                    break;
                }

                auto it = TYPE_SYMBOLS.find(std::string(parameter.type));
                if (it == TYPE_SYMBOLS.end()) {
                    server_.getLogger().error("Unable to register command '{}'. Unsupported type '{}' in usage '{}'.",
                                              name, parameter.type, usage);
                    success = false;
                    break;  // early stop if any of the param in the usage is invalid
                }
                data.chained_subcommand_symbol = static_cast<int>(it->second);
                if (parameter.type == "message") {
                    data.is_optional = false;
                }
            }
            param_data.push_back(data);
        }

        if (success) {
            pending_param_data.push_back(param_data);
            created_enums.insert(created_enums.end(), usage_enums.begin(), usage_enums.end());
        }
        else {
            for (const auto &enum_name : usage_enums) {
                clearEnumValues(enum_name);
            }
        }
    }

    if (pending_param_data.empty()) {
        server_.getLogger().info("Unable to register command '{}': no valid usage", name);
        return false;
    }

    // Actual registration starts from here
    registry.registerCommand(claim->name, command->getDescription().c_str(), CommandPermissionLevel::Any,
                             CommandCheatFlag::NotCheat, CommandUsageFlag::Normal);
    custom_commands_.emplace(claim->name, command);
    custom_enums_[claim->name] = std::move(created_enums);
    for (const auto &alias : claim->aliases) {
        registry.registerAlias(claim->name, alias);
        custom_commands_.emplace(alias, command);
    }
    for (const auto &alias : claim->prefixed) {
        if (registry.aliases_.erase(alias) > 0) {
            removeEnumValueFromExisting("CommandName", alias);
            removeEnumValueFromExisting("CommandAliases", alias);
        }
        registry.registerAlias(claim->name, alias);
        custom_commands_.insert_or_assign(alias, command);
    }
    for (const auto &param_data : pending_param_data) {
        registry.registerOverload<MinecraftCommandAdapter>(claim->name.c_str(), {1, INT_MAX}, param_data);
    }

    command->setName(claim->name);
    command->setAliases(claim->aliases);
    command->registerTo(*this);
    return claim->name == boost::to_lower_copy(boost::trim_copy(name));
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
