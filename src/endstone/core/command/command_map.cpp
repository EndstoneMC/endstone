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
#include <climits>
#include <format>
#include <memory>
#include <optional>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "bedrock/locale/i18n.h"
#include "bedrock/server/commands/command_registry.h"
#include "bedrock/server/commands/command_selector.h"
#include "bedrock/server/server_instance.h"
#include "endstone/brigadier/exceptions.h"
#include "endstone/check.h"
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
#include "endstone/core/command/soft_enum.h"
#include "endstone/core/permissions/default_permissions.h"
#include "endstone/core/server.h"
#include "endstone/event/command/unknown_command_event.h"
#include "endstone/lang/translatable.h"
#include "endstone/player.h"

namespace endstone::core {

EndstoneCommandMap::EndstoneCommandMap(EndstoneServer &server) : server_(server)
{
    soft_enums_.push_back({"", "PluginNames", {}, std::make_shared<EndstoneSoftEnum>(*this, "PluginNames")});
    setDefaultCommands();
}

void EndstoneCommandMap::attach()
{
    std::vector<CommandRegistration> previous;
    std::lock_guard lock(mutex_);
    auto &registry = getHandle().getRegistry();
    std::vector<std::string> reload_names{"reload"};
    for (auto it = registry.aliases_.begin(); it != registry.aliases_.end();) {
        if (it->second == "reload") {
            reload_names.push_back(it->first);
            it = registry.aliases_.erase(it);
        }
        else {
            ++it;
        }
    }
    for (const auto &enum_name : {"CommandName", "CommandAliases"}) {
        const auto it = registry.enum_lookup_.find(enum_name);
        if (it == registry.enum_lookup_.end()) {
            continue;
        }
        std::erase_if(registry.enums_.at(it->second).values, [&](const auto &value) {
            const auto index = CommandRegistry::Symbol(value.first).toIndex();
            return index < registry.enum_values_.size() &&
                   std::ranges::find(reload_names, registry.enum_values_[index]) != reload_names.end();
        });
    }
    registry.signatures_.erase("reload");
    takeSnapshot();

    custom_commands_.clear();
    previous.swap(registrations_);
    for (auto &registration : previous) {
        auto &command = registration.command;
        command->unregisterFrom(*this);
        command->setName(registration.label);
        command->setAliases(registration.aliases);
        if (auto claim = claimNames(registration.fallback_prefix, command)) {
            applyClaim(registration, std::move(*claim));
            registrations_.push_back(std::move(registration));
        }
    }
    dirty_ = true;
}

void EndstoneCommandMap::takeSnapshot()
{
    auto &registry = getHandle().getRegistry();
    allowCommandsWithoutCheats();

    snapshot_ = RegistrySnapshot{
        .rules = registry.rules_,
        .optionals = registry.optionals_,
        .enum_values = registry.enum_values_,
        .enums = registry.enums_,
        .chained_subcommand_values = registry.chained_subcommand_values_,
        .chained_subcommands = registry.chained_subcommands_,
        .factorizations = registry.factorizations_,
        .postfixes = registry.postfixes_,
        .enum_lookup = registry.enum_lookup_,
        .enum_value_lookup = registry.enum_value_lookup_,
        .chained_subcommand_lookup = registry.chained_subcommand_lookup_,
        .chained_subcommand_value_lookup = registry.chained_subcommand_value_lookup_,
        .command_symbols = registry.command_symbols_,
        .signatures = registry.signatures_,
        .type_lookup = registry.type_lookup_,
        .aliases = registry.aliases_,
        .semantic_constraints = registry.semantic_constraints_,
        .semantic_constraint_lookup = registry.semantic_constraint_lookup_,
        .constrained_values = registry.constrained_values_,
        .constrained_value_lookup = registry.constrained_value_lookup_,
        .soft_enums = registry.soft_enums_,
        .soft_enum_lookup = registry.soft_enum_lookup_,
        .skip_on_eps_autocomplete_symbols = registry.skip_on_eps_autocomplete_symbols_,
        .allow_empty_symbols = registry.allow_empty_symbols_,
    };
    type_ids_.clear();
    for (const auto &signature : snapshot_->signatures | std::views::values) {
        for (const auto &overload : signature.overloads) {
            for (const auto &param : overload.params) {
                if (param.param_type == CommandParameterDataType::Basic && param.parse_rule != nullptr) {
                    type_ids_.emplace(param.parse_rule, param.type_index);
                }
            }
        }
    }
    std::erase_if(soft_enums_, [&](const SoftEnumRegistration &soft_enum) {
        if (!snapshot_->soft_enum_lookup.contains(soft_enum.name)) {
            return false;
        }
        server_.getLogger().error("Unable to register soft enum '{}' as the server already has one by that name.",
                                  soft_enum.name);
        return true;
    });
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

void EndstoneCommandMap::applyClaim(CommandRegistration &registration, Claim claim)
{
    const auto &command = registration.command;
    custom_commands_.emplace(claim.name, command);
    for (const auto &alias : claim.aliases) {
        custom_commands_.emplace(alias, command);
    }
    for (const auto &alias : claim.prefixed) {
        custom_commands_.insert_or_assign(alias, command);
    }
    command->setName(claim.name);
    command->setAliases(claim.aliases);
    command->registerTo(*this);
    registration.claim = std::move(claim);
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
    return server_.getCommandMap().tryDispatch(sender, std::move(command_line)).has_value();
}

std::optional<bool> EndstoneCommandMap::tryDispatch(const NotNull<CommandSender> &sender, std::string command_line)
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

    flush();

    // get the command by name
    auto name = args[0];
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    const auto command = getCommand(name);
    if (!command) {
        return std::nullopt;
    }

    if (!snapshot_) {
        if (command->is<TreeCommand>()) {
            return false;
        }
        try {
            return command->execute(sender, std::vector(args.begin() + 1, args.end()));
        }
        catch (const std::exception &e) {
            if (std::string_view(typeid(e).name()) == typeid(brigadier::CommandSyntaxException).name()) {
                sender->sendErrorMessage(e.what());
                return false;
            }
            server_.getLogger().error("Unhandled exception executing '{}': {}", command_line, e.what());
            return false;
        }
    }

    if (!custom_commands_.contains(name)) {
        // This is a vanilla command
        try {
            return static_cast<const MinecraftCommandWrapper &>(*command).dispatch(
                sender, name + command_line.substr(args[0].size()));
        }
        catch (const std::exception &e) {
            if (std::string_view(typeid(e).name()) == typeid(brigadier::CommandSyntaxException).name()) {
                sender->sendErrorMessage(e.what());
                return false;
            }
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
            const auto *adapter = CommandAdapter::find(compiled);
            if (adapter == nullptr) {
                sender->sendErrorMessage(Translatable("commands.generic.unknown", {args[0]}));
                return false;
            }
            return adapter->run(*command_origin, sender, command_line);
        }
        catch (const std::exception &e) {
            if (std::string_view(typeid(e).name()) == typeid(brigadier::CommandSyntaxException).name()) {
                sender->sendErrorMessage(e.what());
                return false;
            }
            server_.getLogger().error("Unhandled exception executing '{}': {}", command_line, e.what());
            return false;
        }
    }
}

void EndstoneCommandMap::clearCommands()
{
    std::unordered_map<std::string, NotNull<Command>> commands;
    std::vector<CommandRegistration> registrations;
    std::lock_guard lock(mutex_);
    for (const auto &command : custom_commands_ | std::views::values) {
        command->unregisterFrom(*this);
    }
    commands.swap(custom_commands_);
    registrations.swap(registrations_);
    std::erase_if(soft_enums_, [](const SoftEnumRegistration &soft_enum) { return !soft_enum.owner.empty(); });
    dirty_ = true;
    setDefaultCommands();
}

Nullable<Command> EndstoneCommandMap::getCommand(std::string name) const
{
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    // Try to find the custom command we've registered
    if (const auto it = custom_commands_.find(name); it != custom_commands_.end()) {
        return it->second;
    }

    if (!snapshot_) {
        return nullptr;
    }

    // Custom command not found, let's find it among the vanilla commands
    auto signature = snapshot_->signatures.find(name);
    if (const auto alias = snapshot_->aliases.find(name);
        signature == snapshot_->signatures.end() && alias != snapshot_->aliases.end()) {
        signature = snapshot_->signatures.find(alias->second);
    }
    if (signature == snapshot_->signatures.end()) {
        return nullptr;
    }
    std::lock_guard lock(mutex_);
    if (const auto it = vanilla_commands_.find(signature->first); it != vanilla_commands_.end()) {
        return it->second;
    }
    auto command = std::make_shared<MinecraftCommandWrapper>(getHandle(), signature->second);
    command->registerTo(*this);
    vanilla_commands_.emplace(signature->first, command);
    return command;
}

Nullable<SoftEnum> EndstoneCommandMap::getSoftEnum(std::string name) const
{
    std::lock_guard lock(mutex_);
    for (const auto &soft_enum : soft_enums_) {
        if (soft_enum.name == name) {
            return soft_enum.handle;
        }
    }
    if (!snapshot_ || !snapshot_->soft_enum_lookup.contains(name)) {
        return nullptr;
    }
    if (const auto it = vanilla_soft_enums_.find(name); it != vanilla_soft_enums_.end()) {
        return it->second;
    }
    auto soft_enum = std::make_shared<EndstoneSoftEnum>(server_.getCommandMap(), name);
    vanilla_soft_enums_.emplace(name, soft_enum);
    return soft_enum;
}

NotNull<SoftEnum> EndstoneCommandMap::registerSoftEnum(std::string owner, std::string name,
                                                       std::vector<std::string> values)
{
    Preconditions::checkArgument(!name.empty() &&
                                     std::ranges::none_of(name, [](unsigned char c) { return std::isspace(c); }),
                                 "Soft enum name '{}' is empty or holds whitespace.", name);
    for (const auto &value : values) {
        Preconditions::checkArgument(!value.empty() &&
                                         std::ranges::none_of(value, [](unsigned char c) { return std::isspace(c); }),
                                     "Soft enum value '{}' is empty or holds whitespace.", value);
    }

    std::shared_ptr<EndstoneSoftEnum> existing;
    bool send = false;
    {
        std::lock_guard lock(mutex_);
        for (auto &soft_enum : soft_enums_) {
            if (soft_enum.name != name) {
                continue;
            }
            Preconditions::checkArgument(soft_enum.owner == owner, "Soft enum '{}' is already registered by {}.", name,
                                         soft_enum.owner.empty() ? "Endstone" : soft_enum.owner);
            soft_enum.values = values;
            existing = soft_enum.handle;
            send = snapshot_.has_value() && !dirty_;
            break;
        }
        if (!existing) {
            Preconditions::checkArgument(!snapshot_ || !snapshot_->soft_enum_lookup.contains(name),
                                         "Soft enum '{}' belongs to the server.", name);
            auto handle = std::make_shared<EndstoneSoftEnum>(*this, name);
            soft_enums_.push_back({std::move(owner), std::move(name), std::move(values), handle});
            dirty_ = true;
            return handle;
        }
    }
    if (send) {
        sendSoftEnumUpdate(std::move(name), SoftEnumUpdateType::Replace, std::move(values));
    }
    return existing;
}

std::set<std::string> EndstoneCommandMap::registerTree(
    std::string owner, const NotNull<brigadier::LiteralCommandNode<CommandSource>> &node, std::string description,
    std::vector<std::string> aliases)
{
    std::lock_guard lock(mutex_);
    const auto label = boost::to_lower_copy(node->getLiteral());
    Preconditions::checkArgument(!label.empty() &&
                                     std::ranges::none_of(label, [](unsigned char c) { return std::isspace(c); }),
                                 "Command label '{}' is empty or holds whitespace.", node->getLiteral());
    CommandCompiler compiler(label);
    const auto error = compiler.addTree(*node);
    Preconditions::checkArgument(!error.has_value(), "Unable to register command '{}'. {}", label, error.value_or(""));

    auto fallback_prefix = boost::to_lower_copy(boost::trim_copy(owner));
    const NotNull<Command> command = std::make_shared<TreeCommand>(std::move(owner), label, description, aliases);
    auto claim = claimNames(fallback_prefix, command);
    if (!claim) {
        return {};
    }
    CommandRegistration registration{
        .fallback_prefix = fallback_prefix,
        .label = label,
        .aliases = std::move(aliases),
        .command = command,
        .claim = {},
        .description = std::move(description),
        .permission_level = CommandPermissionLevel::Any,
        .flags = CommandCheatFlag::NotCheat,
        .compiled = compiler.getCompiled(),
        .tree = node.get(),
    };
    applyClaim(registration, std::move(*claim));
    std::set<std::string> labels{registration.claim.name};
    labels.insert(registration.claim.aliases.begin(), registration.claim.aliases.end());
    labels.insert(registration.claim.prefixed.begin(), registration.claim.prefixed.end());
    registrations_.push_back(std::move(registration));
    dirty_ = true;
    return labels;
}

std::shared_ptr<brigadier::LiteralCommandNode<CommandSource>> EndstoneCommandMap::getTree(const std::string &name) const
{
    std::lock_guard lock(mutex_);
    for (const auto &registration : registrations_) {
        if (registration.tree && registration.claim.name == name) {
            return registration.tree;
        }
    }
    return nullptr;
}

std::shared_ptr<brigadier::LiteralCommandNode<CommandSource>> EndstoneCommandMap::getTree(
    const std::string &name, const RegisteredOverload &registered) const
{
    std::lock_guard lock(mutex_);
    for (const auto &registration : registrations_) {
        if (registration.tree && registration.claim.name == name &&
            std::ranges::find(registration.emitted, registered) != registration.emitted.end()) {
            return registration.tree;
        }
    }
    return nullptr;
}

std::vector<bool> EndstoneCommandMap::getAdvertised(const std::string &name) const
{
    std::lock_guard lock(mutex_);
    std::vector<bool> advertised;
    for (const auto &registration : registrations_) {
        if (registration.claim.name != name) {
            continue;
        }
        for (const auto &emitted : registration.emitted) {
            advertised.push_back(std::ranges::any_of(emitted.sources, [&](const CommandOverload &source) {
                return std::ranges::equal(source, emitted.parameters,
                                          [](const CommandParameter &a, const CommandParameter &b) {
                                              return a.sameSlot(b) && a.optional == b.optional;
                                          });
            }));
        }
    }
    return advertised;
}

std::vector<std::string> EndstoneCommandMap::getSoftEnumValues(const SoftEnum &soft_enum) const
{
    std::lock_guard lock(mutex_);
    for (const auto &registration : soft_enums_) {
        if (registration.handle.get() == &soft_enum) {
            return registration.values;
        }
    }
    const auto vanilla = vanilla_soft_enums_.find(soft_enum.getName());
    if (vanilla == vanilla_soft_enums_.end() || vanilla->second.get().get() != &soft_enum) {
        throw std::runtime_error("Trying to access a soft enum that is no longer valid.");
    }
    const auto &registry = getHandle().getRegistry();
    const auto it = registry.soft_enum_lookup_.find(soft_enum.getName());
    if (it == registry.soft_enum_lookup_.end()) {
        return {};
    }
    return registry.soft_enums_.at(it->second).values;
}

void EndstoneCommandMap::updateSoftEnum(const SoftEnum &soft_enum, SoftEnumUpdateType type,
                                        std::vector<std::string> values)
{
    for (const auto &value : values) {
        Preconditions::checkArgument(!value.empty() &&
                                         std::ranges::none_of(value, [](unsigned char c) { return std::isspace(c); }),
                                     "Soft enum value '{}' is empty or holds whitespace.", value);
    }

    std::string name;
    bool send = false;
    {
        std::lock_guard lock(mutex_);
        const auto registration = std::ranges::find_if(
            soft_enums_, [&](const SoftEnumRegistration &entry) { return entry.handle.get() == &soft_enum; });
        if (registration == soft_enums_.end()) {
            const auto vanilla = vanilla_soft_enums_.find(soft_enum.getName());
            Preconditions::checkArgument(vanilla == vanilla_soft_enums_.end() ||
                                             vanilla->second.get().get() != &soft_enum,
                                         "Soft enum '{}' belongs to the server and is read-only.", soft_enum.getName());
            throw std::runtime_error("Trying to access a soft enum that is no longer valid.");
        }
        Preconditions::checkArgument(!registration->owner.empty(),
                                     "Soft enum '{}' belongs to Endstone and is read-only.", registration->name);
        auto &current = registration->values;
        switch (type) {
        case SoftEnumUpdateType::Add:
            for (const auto &value : values) {
                if (std::ranges::find(current, value) == current.end()) {
                    current.push_back(value);
                }
            }
            break;
        case SoftEnumUpdateType::Remove:
            std::erase_if(current, [&](const auto &value) { return std::ranges::find(values, value) != values.end(); });
            break;
        case SoftEnumUpdateType::Replace:
            current = values;
            break;
        }
        name = registration->name;
        send = snapshot_.has_value() && !dirty_;
    }
    if (send) {
        sendSoftEnumUpdate(std::move(name), type, std::move(values));
    }
}

void EndstoneCommandMap::addDimension(std::string name, std::uint64_t value)
{
    std::lock_guard lock(mutex_);
    dimensions_.emplace_back(std::move(name), value);
    dirty_ = true;
}

void EndstoneCommandMap::sendSoftEnumUpdate(std::string name, SoftEnumUpdateType type, std::vector<std::string> values)
{
    if (!server_.isPrimaryThread()) {
        server_.getEndstoneScheduler().runTask([this, name] {
            std::vector<std::string> current;
            {
                std::lock_guard lock(mutex_);
                const auto it = std::ranges::find_if(
                    soft_enums_, [&](const SoftEnumRegistration &entry) { return entry.name == name; });
                if (dirty_ || it == soft_enums_.end()) {
                    return;
                }
                current = it->values;
            }
            getHandle().getRegistry().setSoftEnumValues(name, std::move(current));
        });
        return;
    }
    auto &registry = getHandle().getRegistry();
    switch (type) {
    case SoftEnumUpdateType::Add:
        registry.addSoftEnumValues(name, std::move(values));
        break;
    case SoftEnumUpdateType::Remove:
        registry.removeSoftEnumValues(name, std::move(values));
        break;
    case SoftEnumUpdateType::Replace:
        registry.setSoftEnumValues(name, std::move(values));
        break;
    }
}

MinecraftCommands &EndstoneCommandMap::getHandle() const
{
    return server_.getServer().getMinecraft()->getCommands();
}

std::optional<CommandPermissionLevel> EndstoneCommandMap::getPermissionLevel(const std::string &name) const
{
    std::lock_guard lock(mutex_);
    if (snapshot_) {
        if (const auto it = snapshot_->signatures.find(name); it != snapshot_->signatures.end()) {
            return it->second.permission_level;
        }
    }
    for (const auto &registration : registrations_) {
        if (registration.claim.name == name) {
            return registration.permission_level;
        }
    }
    return std::nullopt;
}

std::vector<std::string> EndstoneCommandMap::getVanillaCommandNames() const
{
    std::lock_guard lock(mutex_);
    if (!snapshot_) {
        return {};
    }
    const auto names = snapshot_->signatures | std::views::keys;
    return {names.begin(), names.end()};
}

bool EndstoneCommandMap::flush()
{
    if (!snapshot_ || !server_.isPrimaryThread()) {
        return false;
    }
    std::vector<std::string> plugin_names;
    for (const auto &plugin : server_.getPluginManager().getPlugins()) {
        if (plugin->isEnabled()) {
            plugin_names.push_back(boost::to_lower_copy(plugin->getName()));
        }
    }
    std::vector<CommandRegistration> dropped;
    bool send_plugin_names = false;
    bool committed = false;
    {
        std::lock_guard lock(mutex_);
        for (auto &soft_enum : soft_enums_) {
            if (soft_enum.owner.empty() && soft_enum.name == "PluginNames" && soft_enum.values != plugin_names) {
                soft_enum.values = plugin_names;
                send_plugin_names = !dirty_;
            }
        }
        if (dirty_) {
            dropped = commit();
            committed = true;
        }
    }
    if (send_plugin_names) {
        getHandle().getRegistry().setSoftEnumValues("PluginNames", std::move(plugin_names));
    }
    if (!committed) {
        return false;
    }
    for (const auto &player : server_.getOnlinePlayers()) {
        player->updateCommands();
    }
    return true;
}

std::vector<EndstoneCommandMap::CommandRegistration> EndstoneCommandMap::commit()
{
    dirty_ = false;
    if (!committed_) {
        takeSnapshot();
        committed_ = true;
    }
    auto &commands = getHandle();
    retired_commands_.push_back(std::move(commands.compiled_command_map_));
    commands.compiled_command_map_.clear();
    auto &registry = commands.getRegistry();
    auto &snapshot = *snapshot_;
    auto &logger = server_.getLogger();

    for (const auto &name : registry.signatures_ | std::views::keys) {
        if (!snapshot.signatures.contains(name) && !replayed_commands_.contains(name)) {
            logger.error("Command '{}' was added to the command registry after startup by something other than "
                         "Endstone, and the rebuild drops it.",
                         name);
        }
    }
    for (const auto &alias : registry.aliases_ | std::views::keys) {
        if (!snapshot.aliases.contains(alias) && !replayed_aliases_.contains(alias)) {
            logger.error("Command alias '{}' was added to the command registry after startup by something other than "
                         "Endstone, and the rebuild drops it.",
                         alias);
        }
    }
    std::vector<CommandRegistry::SoftEnum> carried;
    for (const auto &soft_enum : registry.soft_enums_) {
        if (std::ranges::any_of(soft_enums_, [&](const auto &own) { return own.name == soft_enum.name; })) {
            continue;
        }
        if (!snapshot.soft_enum_lookup.contains(soft_enum.name)) {
            if (replayed_soft_enums_.contains(soft_enum.name)) {
                continue;
            }
            logger.error("Soft enum '{}' was added to the command registry after startup by something other than "
                         "Endstone, and the rebuild drops it.",
                         soft_enum.name);
            continue;
        }
        carried.push_back(soft_enum);
    }

    registry.rules_ = snapshot.rules;
    registry.optionals_ = snapshot.optionals;
    registry.enum_values_ = snapshot.enum_values;
    registry.enums_ = snapshot.enums;
    registry.chained_subcommand_values_ = snapshot.chained_subcommand_values;
    registry.chained_subcommands_ = snapshot.chained_subcommands;
    registry.factorizations_ = snapshot.factorizations;
    registry.postfixes_ = snapshot.postfixes;
    registry.enum_lookup_ = snapshot.enum_lookup;
    registry.enum_value_lookup_ = snapshot.enum_value_lookup;
    registry.chained_subcommand_lookup_ = snapshot.chained_subcommand_lookup;
    registry.chained_subcommand_value_lookup_ = snapshot.chained_subcommand_value_lookup;
    registry.command_symbols_ = snapshot.command_symbols;
    registry.signatures_ = snapshot.signatures;
    registry.type_lookup_ = snapshot.type_lookup;
    registry.aliases_ = snapshot.aliases;
    registry.semantic_constraints_ = snapshot.semantic_constraints;
    registry.semantic_constraint_lookup_ = snapshot.semantic_constraint_lookup;
    registry.constrained_values_ = snapshot.constrained_values;
    registry.constrained_value_lookup_ = snapshot.constrained_value_lookup;
    registry.soft_enums_ = snapshot.soft_enums;
    registry.soft_enum_lookup_ = snapshot.soft_enum_lookup;
    registry.skip_on_eps_autocomplete_symbols_ = snapshot.skip_on_eps_autocomplete_symbols;
    registry.allow_empty_symbols_ = snapshot.allow_empty_symbols;
    registry.parse_tables_.clear();

    if (const auto it = registry.enum_lookup_.find("Dimension"); it != registry.enum_lookup_.end()) {
        for (const auto &[name, value] : dimensions_) {
            const auto &dimension_enum = registry.enums_.at(it->second);
            if (std::ranges::none_of(dimension_enum.values, [&](const auto &entry) {
                    const auto index = CommandRegistry::Symbol(entry.first).toIndex();
                    return index < registry.enum_values_.size() && registry.enum_values_[index] == name;
                })) {
                registry._addEnumValuesInternal("Dimension", {{name, value}}, dimension_enum.type,
                                                dimension_enum.parse);
            }
        }
    }

    for (auto &soft_enum : carried) {
        registry.soft_enums_.at(registry.soft_enum_lookup_.at(soft_enum.name)).values = std::move(soft_enum.values);
    }
    replayed_soft_enums_.clear();
    for (const auto &soft_enum : soft_enums_) {
        replayed_soft_enums_.insert(soft_enum.name);
        if (const auto it = registry.soft_enum_lookup_.find(soft_enum.name); it != registry.soft_enum_lookup_.end()) {
            registry.soft_enums_.at(it->second).values = soft_enum.values;
        }
        else {
            registry.addSoftEnum(soft_enum.name, soft_enum.values);
        }
    }

    std::vector<CommandRegistration> dropped;
    for (auto it = registrations_.begin(); it != registrations_.end();) {
        std::string missing;
        if (it->tree) {
            for (const auto &overload : it->compiled.overloads) {
                for (const auto &parameter : overload) {
                    if (parameter.kind == CommandParameter::Kind::SoftEnum &&
                        !registry.soft_enum_lookup_.contains(parameter.enum_name)) {
                        missing = parameter.enum_name;
                    }
                }
            }
        }
        if (missing.empty()) {
            ++it;
            continue;
        }
        logger.error("Unable to register command '{}': soft enum '{}' does not exist.", it->claim.name, missing);
        const auto &command = it->command;
        std::erase_if(custom_commands_, [&](const auto &entry) { return entry.second.get() == command.get(); });
        command->unregisterFrom(*this);
        dropped.push_back(std::move(*it));
        it = registrations_.erase(it);
    }

    std::map<std::string, std::vector<std::string>> taken;
    for (const auto &[name, index] : registry.enum_lookup_) {
        auto &values = taken[name];
        for (const auto &value : registry.enums_.at(index).values) {
            if (const auto text = CommandRegistry::Symbol(value.first).toIndex(); text < registry.enum_values_.size()) {
                values.push_back(registry.enum_values_[text]);
            }
        }
    }

    replayed_commands_.clear();
    replayed_aliases_.clear();
    const auto string_type = CommandRegistry::getTypeId<std::string>();
    for (auto &registration : registrations_) {
        const auto &name = registration.claim.name;
        registration.emitted.clear();
        std::unordered_map<std::string, std::string> enum_names;
        for (const auto &compiled_enum : registration.compiled.enums) {
            auto enum_name = CommandCompiler::nameEnum(compiled_enum.name, compiled_enum.values, taken);
            if (!taken.contains(enum_name)) {
                registry.addEnumValues(enum_name, compiled_enum.values);
                taken.emplace(enum_name, compiled_enum.values);
            }
            enum_names.emplace(compiled_enum.name, std::move(enum_name));
        }

        registry.registerCommand(name, registration.description.c_str(), registration.permission_level,
                                 registration.flags, CommandUsageFlag::Normal);
        auto *signature = const_cast<CommandRegistry::Signature *>(registry.findCommand(name));
        if (signature == nullptr) {
            logger.error("Unable to register command '{}'.", name);
            continue;
        }
        replayed_commands_.insert(name);
        for (const auto &aliases : {registration.claim.aliases, registration.claim.prefixed}) {
            for (const auto &alias : aliases) {
                if (const auto it = custom_commands_.find(alias);
                    it != custom_commands_.end() && it->second.get() == registration.command.get()) {
                    registry.registerAlias(name, alias);
                    replayed_aliases_.insert(alias);
                }
            }
        }

        auto overloads = registration.compiled.registered;
        if (const auto error = CommandCompiler::captureWords(
                overloads, registration.compiled.overloads,
                [&](const CommandParameter &word, const CommandParameter &argument) {
                    const auto values = taken.find(argument.enum_name);
                    return word.isWord() && argument.isEnum() && argument.foreign && argument.values.empty() &&
                           values != taken.end() && std::ranges::any_of(word.values, [&](const std::string &value) {
                               return std::ranges::any_of(values->second, [&](const std::string &runtime) {
                                   return boost::algorithm::iequals(value, runtime);
                               });
                           });
                },
                "/" + name, registration.tree != nullptr)) {
            logger.error("Unable to register command '{}'. {}", name, *error);
            continue;
        }
        for (const auto &registered : overloads) {
            const auto &overload = registered.parameters;
            std::vector<CommandParameterData> params;
            for (const auto &parameter : overload) {
                using Kind = CommandParameter::Kind;
                const CommandRegistry::ParamParseRule *rule = CommandRegistry::getParseRule<std::string>();
                auto type = CommandParameterDataType::Basic;
                const char *enum_name = nullptr;
                int symbol = -1;
                auto options = CommandParameterOption::None;
                switch (parameter.kind) {
                case Kind::Keyword:
                case Kind::Enum:
                case Kind::Dimension:
                case Kind::BlockType:
                case Kind::ItemType:
                case Kind::ActorType: {
                    const auto display = enum_names.find(parameter.enum_name);
                    const auto it =
                        registry.enum_lookup_.find(display != enum_names.end() ? display->second : parameter.enum_name);
                    if (it == registry.enum_lookup_.end()) {
                        logger.error("Unable to register an overload of command '{}': enum '{}' does not exist.", name,
                                     parameter.enum_name);
                        break;
                    }
                    type = CommandParameterDataType::Enum;
                    enum_name = it->first.c_str();
                    symbol = CommandRegistry::Symbol::fromEnumIndex(it->second).value();
                    if (!parameter.foreign) {
                        options = CommandParameterOption::EnumAutocompleteExpansion;
                    }
                    break;
                }
                case Kind::SoftEnum: {
                    const auto it = registry.soft_enum_lookup_.find(parameter.enum_name);
                    if (it == registry.soft_enum_lookup_.end()) {
                        logger.error("Unable to register an overload of command '{}': soft enum '{}' does not exist.",
                                     name, parameter.enum_name);
                        break;
                    }
                    type = CommandParameterDataType::SoftEnum;
                    enum_name = it->first.c_str();
                    symbol = CommandRegistry::Symbol::fromSoftEnumIndex(it->second).value();
                    break;
                }
                case Kind::Int:
                    rule = CommandRegistry::getParseRule<int>();
                    break;
                case Kind::Float:
                    rule = CommandRegistry::getParseRule<float>();
                    break;
                case Kind::String:
                    break;
                case Kind::Actor:
                    rule = CommandRegistry::getParseRule<CommandSelector<::Actor>>();
                    break;
                case Kind::Player:
                    rule = CommandRegistry::getParseRule<CommandSelector<::Player>>();
                    break;
                case Kind::BlockPos:
                    rule = CommandRegistry::getParseRule<CommandPosition>();
                    break;
                case Kind::Pos:
                    rule = CommandRegistry::getParseRule<CommandPositionFloat>();
                    break;
                case Kind::Message:
                    rule = CommandRegistry::getParseRule<CommandMessage>();
                    break;
                case Kind::RawText:
                    rule = CommandRegistry::getParseRule<CommandRawText>();
                    break;
                case Kind::Json:
                    rule = CommandRegistry::getParseRule<Json::Value>();
                    break;
                case Kind::BlockStates:
                    rule = &CommandRegistry::ParseRuleFor<MinecraftCommandAdapter>::instance;
                    break;
                }
                if ((parameter.isEnum() || parameter.kind == Kind::SoftEnum) && enum_name == nullptr) {
                    params.clear();
                    break;
                }
                if (registration.tree) {
                    auto type_id = string_type;
                    if (parameter.kind == Kind::Int || parameter.kind == Kind::Float ||
                        parameter.kind == Kind::Message || parameter.kind == Kind::Actor ||
                        parameter.kind == Kind::Player || parameter.kind == Kind::BlockPos ||
                        parameter.kind == Kind::Pos) {
                        const auto it = type_ids_.find(rule);
                        if (it == type_ids_.end()) {
                            logger.error("Unable to register an overload of command '{}': no command of the server has "
                                         "a parameter of the type of '{}' to take the type id from.",
                                         name, parameter.name);
                            params.clear();
                            break;
                        }
                        type_id = it->second;
                    }
                    auto &data = params.emplace_back(
                        type_id, rule, parameter.name.c_str(), type, enum_name, static_cast<int>(params.size()),
                        parameter.optional, &BrigadierCommandAdapter::getSlot, &BrigadierCommandAdapter::isSlotSet);
                    if (parameter.isParsedAsText()) {
                        data.parse_override = &CommandRegistry::parse<BrigadierCommandAdapter>;
                    }
                    data.enum_or_postfix_symbol = symbol;
                    data.options = options;
                    continue;
                }
                auto &data = params.emplace_back(string_type, rule, parameter.name.c_str(), type, enum_name, nullptr, 0,
                                                 parameter.optional, -1);
                data.parse_override = &CommandRegistry::parse<MinecraftCommandAdapter>;
                data.enum_or_postfix_symbol = symbol;
                data.options = options;
            }
            if (params.size() != overload.size()) {
                continue;
            }
            auto &added = registration.tree
                            ? signature->overloads.emplace_back(
                                  CommandVersion{1, INT_MAX},
                                  [name, registered]() {
                                      return std::unique_ptr<::Command>(
                                          std::make_unique<BrigadierCommandAdapter>(name, registered));
                                  })
                            : signature->overloads.emplace_back(CommandVersion{1, INT_MAX}, [name, registered]() {
                                  return std::unique_ptr<::Command>(
                                      std::make_unique<MinecraftCommandAdapter>(name, registered));
                              });
            added.params = std::move(params);
            registry.registerOverloadInternal(*signature, added);
            registration.emitted.push_back(registered);
        }
    }
    return dropped;
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

    const auto name = command->getName();
    const auto label = boost::to_lower_copy(boost::trim_copy(name));
    auto claim = claimNames(fallback_prefix, command);
    if (!claim) {
        return false;
    }

    std::set<std::string> soft_enums;
    for (const auto &soft_enum : soft_enums_) {
        soft_enums.insert(soft_enum.name);
    }
    if (snapshot_) {
        for (const auto &soft_enum : snapshot_->soft_enum_lookup | std::views::keys) {
            soft_enums.insert(soft_enum);
        }
    }
    auto usages = command->getUsages();
    if (usages.empty()) {
        usages.push_back("/" + name);
    }
    CommandCompiler compiler(label);
    for (const auto &usage : usages) {
        auto result = CommandUsageParser(usage).parse();
        if (!result.has_value()) {
            server_.getLogger().error("Error occurred when parsing usage '{}'. {}", usage, result.error());
            continue;
        }
        if (result->command_name != name) {
            server_.getLogger().warning("Unexpected command name '{}' in usage '{}', do you mean '{}'?",
                                        result->command_name, usage, name);
        }
        if (const auto error = compiler.addUsage(*result, soft_enums)) {
            server_.getLogger().error("Unable to register usage '{}' of command '{}'. {}", usage, name, *error);
        }
    }
    if (compiler.getCompiled().overloads.empty()) {
        server_.getLogger().error("Unable to register command '{}': no valid usage", name);
        return false;
    }

    CommandRegistration registration{
        .fallback_prefix = fallback_prefix,
        .label = name,
        .aliases = command->getAliases(),
        .command = command,
        .claim = {},
        .description = command->getDescription(),
        .permission_level = CommandPermissionLevel::Any,
        .flags = CommandCheatFlag::NotCheat,
        .compiled = compiler.getCompiled(),
        .tree = nullptr,
    };
    applyClaim(registration, std::move(*claim));
    const bool registered = registration.claim.name == label;
    registrations_.push_back(std::move(registration));
    dirty_ = true;
    return registered;
}

}  // namespace endstone::core
