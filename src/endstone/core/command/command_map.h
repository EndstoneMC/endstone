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

#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "bedrock/network/packet/update_soft_enum_packet.h"
#include "bedrock/server/commands/minecraft_commands.h"
#include "endstone/command/command.h"
#include "endstone/command/command_map.h"
#include "endstone/command/soft_enum.h"
#include "endstone/core/command/commands.h"
#include "endstone/core/command/compiler/command_compiler.h"

namespace endstone::core {

class EndstoneServer;
class EndstoneSoftEnum;
class EndstoneCommandMap : public CommandMap, public CommandRegistrar {
public:
    explicit EndstoneCommandMap(EndstoneServer &server);
    using CommandMap::registerCommand;
    bool registerCommand(NotNull<Command> command) override;
    bool registerCommand(std::string fallback_prefix, NotNull<Command> command) override;
    bool dispatch(const NotNull<CommandSender> &sender, std::string command_line) const override;
    [[nodiscard]] std::optional<bool> tryDispatch(const NotNull<CommandSender> &sender, std::string command_line);
    void clearCommands() override;
    [[nodiscard]] Nullable<Command> getCommand(std::string name) const override;
    [[nodiscard]] Nullable<SoftEnum> getSoftEnum(std::string name) const override;
    NotNull<SoftEnum> registerSoftEnum(std::string owner, std::string name, std::vector<std::string> values) override;
    std::set<std::string> registerTree(std::string owner,
                                       const NotNull<brigadier::LiteralCommandNode<CommandSource>> &node,
                                       std::string description, std::vector<std::string> aliases) override;
    [[nodiscard]] std::shared_ptr<brigadier::LiteralCommandNode<CommandSource>> getTree(const std::string &name) const;
    [[nodiscard]] std::shared_ptr<brigadier::LiteralCommandNode<CommandSource>> getTree(
        const std::string &name, const RegisteredOverload &registered) const;
    [[nodiscard]] std::vector<bool> getAdvertised(const std::string &name) const;
    [[nodiscard]] std::vector<std::string> getSoftEnumValues(const SoftEnum &soft_enum) const;
    void updateSoftEnum(const SoftEnum &soft_enum, SoftEnumUpdateType type, std::vector<std::string> values);
    void addDimension(std::string name, std::uint64_t value);
    [[nodiscard]] ::MinecraftCommands &getHandle() const;
    [[nodiscard]] std::optional<CommandPermissionLevel> getPermissionLevel(const std::string &name) const;
    [[nodiscard]] std::vector<std::string> getVanillaCommandNames() const;
    bool flush();

private:
    friend class EndstoneServer;
    void attach();
    struct Claim {
        std::string name;
        std::vector<std::string> aliases;
        std::vector<std::string> prefixed;
    };
    struct CommandRegistration {
        std::string fallback_prefix;
        std::string label;
        std::vector<std::string> aliases;
        NotNull<Command> command;
        Claim claim;
        std::string description;
        CommandPermissionLevel permission_level;
        CommandFlag flags;
        CompiledCommand compiled;
        std::shared_ptr<brigadier::LiteralCommandNode<CommandSource>> tree;
        std::vector<RegisteredOverload> emitted;
    };
    struct SoftEnumRegistration {
        std::string owner;
        std::string name;
        std::vector<std::string> values;
        std::shared_ptr<EndstoneSoftEnum> handle;
    };
    struct RegistrySnapshot {
        std::vector<CommandRegistry::ParseRule> rules;
        std::vector<CommandRegistry::OptionalParameterChain> optionals;
        std::vector<std::string> enum_values;
        std::vector<CommandRegistry::Enum> enums;
        std::vector<std::string> chained_subcommand_values;
        std::vector<CommandRegistry::ChainedSubcommand> chained_subcommands;
        std::vector<CommandRegistry::Factorization> factorizations;
        std::vector<std::string> postfixes;
        std::map<std::string, std::uint32_t> enum_lookup;
        std::map<std::string, std::uint64_t> enum_value_lookup;
        std::map<std::string, std::uint32_t> chained_subcommand_lookup;
        std::map<std::string, std::uint64_t> chained_subcommand_value_lookup;
        std::vector<CommandRegistry::Symbol> command_symbols;
        std::map<std::string, CommandRegistry::Signature> signatures;
        std::map<Bedrock::typeid_t<CommandRegistry>, std::int32_t> type_lookup;
        std::map<std::string, std::string> aliases;
        std::vector<SemanticConstraint> semantic_constraints;
        std::map<SemanticConstraint, std::uint8_t> semantic_constraint_lookup;
        std::vector<CommandRegistry::ConstrainedValue> constrained_values;
        std::map<std::pair<std::uint64_t, std::uint32_t>, std::uint32_t> constrained_value_lookup;
        std::vector<CommandRegistry::SoftEnum> soft_enums;
        std::map<std::string, std::uint32_t> soft_enum_lookup;
        std::unordered_set<int> skip_on_eps_autocomplete_symbols;
        std::unordered_set<int> allow_empty_symbols;
    };
    [[nodiscard]] std::optional<Claim> claimNames(const std::string &fallback_prefix,
                                                  const NotNull<Command> &command) const;
    void applyClaim(CommandRegistration &registration, Claim claim);
    void setDefaultCommands();
    void allowCommandsWithoutCheats();
    void takeSnapshot();
    std::vector<CommandRegistration> commit();
    void sendSoftEnumUpdate(std::string name, SoftEnumUpdateType type, std::vector<std::string> values);

    EndstoneServer &server_;
    mutable std::recursive_mutex mutex_;
    std::vector<CommandRegistration> registrations_;
    std::vector<SoftEnumRegistration> soft_enums_;
    std::unordered_map<std::string, NotNull<Command>> custom_commands_;
    mutable std::unordered_map<std::string, NotNull<Command>> vanilla_commands_;
    mutable std::unordered_map<std::string, NotNull<SoftEnum>> vanilla_soft_enums_;
    std::optional<RegistrySnapshot> snapshot_;
    std::map<const CommandRegistry::ParamParseRule *, Bedrock::typeid_t<CommandRegistry>> type_ids_;
    std::set<std::string> replayed_commands_;
    std::set<std::string> replayed_aliases_;
    std::set<std::string> replayed_soft_enums_;
    std::vector<std::pair<std::string, std::uint64_t>> dimensions_;
    std::vector<std::unordered_map<HashedString, std::unique_ptr<::Command>>> retired_commands_;
    bool dirty_ = false;
    bool committed_ = false;
};

}  // namespace endstone::core
