// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in coMirrorliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or iMirrorlied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <climits>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "bedrock/server/commands/command.h"
#include "bedrock/server/commands/command_registry.h"

static_assert(!std::is_trivially_copyable_v<CommandRegistry::ParseRule>);
static_assert(!std::is_trivially_copyable_v<CommandRegistry::Enum>);
static_assert(!std::is_trivially_copyable_v<CommandRegistry::ChainedSubcommand>);
static_assert(!std::is_trivially_copyable_v<CommandRegistry::ConstrainedValue>);
static_assert(!std::is_trivially_copyable_v<CommandRegistry::Factorization>);
static_assert(alignof(CommandRegistry::Factorization) == 4);
static_assert(alignof(CommandRegistry::ConstrainedValue) == 8);
static_assert(alignof(CommandRegistry::Enum) == 8);
static_assert(alignof(CommandRegistry::ChainedSubcommand) == 8);

namespace {

struct RegistryTables {
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
};

void *get_slot(Command *, int)
{
    return nullptr;
}

bool *get_slot_is_set(Command *, int)
{
    return nullptr;
}

CommandRegistry::Overload make_overload(const char *param_name)
{
    CommandRegistry::Overload overload({1, INT_MAX}, []() -> std::unique_ptr<Command> { return nullptr; });
    overload.params.emplace_back(Bedrock::typeid_t<CommandRegistry>{1}, nullptr, param_name,
                                 CommandParameterDataType::Basic, nullptr, 0, false, &get_slot, &get_slot_is_set);
    overload.params_symbol.emplace_back(CommandRegistry::HardNonTerminal::Id);
    return overload;
}

CommandRegistry::ParseRule make_rule(CommandRegistry::Symbol non_terminal, CommandRegistry::Symbol derivation,
                                     CommandVersion versions)
{
    CommandRegistry::ParseRule rule;
    rule.non_terminal = non_terminal;
    rule.process = [](CommandRegistry::ParseToken &token, CommandRegistry::Symbol) {
        return &token;
    };
    rule.derivation = {derivation};
    rule.versions = versions;
    return rule;
}

CommandRegistry::Enum make_enum(const std::string &name, std::vector<std::pair<std::uint64_t, std::uint64_t>> values)
{
    CommandRegistry::Enum result;
    result.name = name;
    result.type = {1};
    result.parse = nullptr;
    result.values = std::move(values);
    return result;
}

CommandRegistry::Signature make_signature(const std::string &name, int first_rule)
{
    CommandRegistry::Signature signature{.name = name,
                                         .description = "test",
                                         .overloads = {},
                                         .chained_subcommand_indexes = {},
                                         .permission_level = CommandPermissionLevel::Any,
                                         .command_symbol = CommandRegistry::Symbol::fromEnumValueIndex(0),
                                         .command_alias_enum = CommandRegistry::Symbol{0},
                                         .flags = CommandFlag{},
                                         .first_rule = first_rule,
                                         .first_factorization = 0,
                                         .first_optional = 0,
                                         .runnable = true,
                                         .rule_counter = 0};
    signature.overloads.push_back(make_overload("target"));
    return signature;
}

RegistryTables make_tables()
{
    RegistryTables tables;
    tables.rules.push_back(
        make_rule(CommandRegistry::HardNonTerminal::Command, CommandRegistry::HardNonTerminal::Id, {0, INT_MAX}));
    tables.optionals.push_back({1, 0, CommandRegistry::HardNonTerminal::Int});
    tables.enum_values = {"true", "false"};
    tables.enums.push_back(make_enum("Boolean", {{0, 1}, {1, 0}}));
    tables.enum_lookup["Boolean"] = 0;
    tables.enum_value_lookup["true"] = 0;
    tables.enum_value_lookup["false"] = 1;
    tables.chained_subcommand_values = {"run"};
    auto &chained = tables.chained_subcommands.emplace_back();
    chained.name = "ExecuteChainedOption_0";
    chained.type = {1};
    chained.parse = nullptr;
    chained.values = {{0, 0}};
    tables.chained_subcommand_lookup["ExecuteChainedOption_0"] = 0;
    tables.chained_subcommand_value_lookup["run"] = 0;
    tables.factorizations.emplace_back().command_symbol = CommandRegistry::Symbol::fromEnumValueIndex(0);
    tables.postfixes = {"L"};
    tables.command_symbols.push_back(CommandRegistry::Symbol::fromEnumValueIndex(0));
    tables.signatures.emplace("say", make_signature("say", 0));
    tables.aliases["msg"] = "tell";
    tables.semantic_constraints.push_back(SemanticConstraint::RequiresCheatsEnabled);
    tables.semantic_constraint_lookup[SemanticConstraint::RequiresCheatsEnabled] = 0;
    auto &constrained = tables.constrained_values.emplace_back();
    constrained.value = CommandRegistry::Symbol::fromEnumValueIndex(0);
    constrained.enum_symbol = CommandRegistry::Symbol::fromEnumIndex(0);
    constrained.constraints = {0};
    tables.constrained_value_lookup[{0, 0}] = 0;
    tables.soft_enums.push_back({"TagValues", {"a", "b"}});
    tables.soft_enum_lookup["TagValues"] = 0;
    return tables;
}

template <typename T>
std::ptrdiff_t offset_of(const T &object, const void *member)
{
    return static_cast<const char *>(member) - reinterpret_cast<const char *>(&object);
}

}  // namespace

TEST(CommandParameterDataTest, CustomStorageConstructorKeepsIndexAndAccessors)
{
    CommandParameterData data(Bedrock::typeid_t<CommandRegistry>{7}, nullptr, "slot", CommandParameterDataType::Basic,
                              nullptr, 3, true, &get_slot, &get_slot_is_set);
    EXPECT_EQ(data.type_index.id, 7);
    EXPECT_EQ(data.name, "slot");
    EXPECT_EQ(data.offset, 3);
    EXPECT_EQ(data.set_offset, 3);
    EXPECT_TRUE(data.is_optional);
    EXPECT_EQ(data.value_get_fn, &get_slot);
    EXPECT_EQ(data.value_is_set_fn, &get_slot_is_set);
    EXPECT_EQ(data.parse_override, nullptr);
    EXPECT_EQ(data.enum_or_postfix_symbol, -1);
    EXPECT_EQ(data.chained_subcommand_symbol, -1);
}

TEST(CommandParameterDataTest, MembersSitAtBdsOffsets)
{
    CommandParameterData data(Bedrock::typeid_t<CommandRegistry>{0}, nullptr, "x", CommandParameterDataType::Basic,
                              nullptr, 0, false, nullptr, nullptr);
    EXPECT_EQ(offset_of(data, &data.parse_rule), 8);
    EXPECT_EQ(offset_of(data, &data.parse_override), 16);
#ifdef _WIN32
    EXPECT_EQ(offset_of(data, &data.name), 24);
#else
    EXPECT_EQ(offset_of(data, &data.name), 32);
#endif
    EXPECT_EQ(offset_of(data, &data.enum_name_or_postfix), 56);
    EXPECT_EQ(offset_of(data, &data.enum_or_postfix_symbol), 64);
    EXPECT_EQ(offset_of(data, &data.chained_subcommand), 72);
    EXPECT_EQ(offset_of(data, &data.chained_subcommand_symbol), 80);
    EXPECT_EQ(offset_of(data, &data.param_type), 84);
    EXPECT_EQ(offset_of(data, &data.offset), 88);
    EXPECT_EQ(offset_of(data, &data.set_offset), 92);
    EXPECT_EQ(offset_of(data, &data.is_optional), 96);
    EXPECT_EQ(offset_of(data, &data.options), 97);
    EXPECT_EQ(offset_of(data, &data.value_get_fn), 104);
    EXPECT_EQ(offset_of(data, &data.value_is_set_fn), 112);
}

TEST(CommandRegistryTest, SignatureMembersSitAtBdsOffsets)
{
    const auto signature = make_signature("say", 0);
#ifdef _WIN32
    constexpr std::ptrdiff_t base = 112;
#else
    constexpr std::ptrdiff_t base = 96;
#endif
    EXPECT_EQ(offset_of(signature, &signature.permission_level), base);
    EXPECT_EQ(offset_of(signature, &signature.command_symbol), base + 4);
    EXPECT_EQ(offset_of(signature, &signature.command_alias_enum), base + 8);
    EXPECT_EQ(offset_of(signature, &signature.flags), base + 12);
    EXPECT_EQ(offset_of(signature, &signature.first_rule), base + 16);
    EXPECT_EQ(offset_of(signature, &signature.first_factorization), base + 20);
    EXPECT_EQ(offset_of(signature, &signature.first_optional), base + 24);
    EXPECT_EQ(offset_of(signature, &signature.runnable), base + 28);
    EXPECT_EQ(offset_of(signature, &signature.rule_counter), base + 32);
}

TEST(CommandRegistryTest, TablesRestoreFromCopyAfterMutation)
{
    auto live = make_tables();
    const auto snapshot = live;

    live.rules.push_back(make_rule(CommandRegistry::HardNonTerminal::Message,
                                   CommandRegistry::HardNonTerminal::MessageRoot, {1, INT_MAX}));
    live.enum_values.emplace_back("warp");
    live.enums.push_back(make_enum("WarpAction", {{2, 0}}));
    live.enum_lookup["WarpAction"] = 1;
    live.signatures.emplace("warp", make_signature("warp", 1));
    live.signatures.at("say").overloads.push_back(make_overload("message"));
    live.soft_enums.push_back({"WarpNames", {"spawn"}});
    live.soft_enum_lookup["WarpNames"] = 1;
    live.soft_enums.at(0).values.emplace_back("c");
    ASSERT_EQ(live.rules.size(), 2);
    ASSERT_EQ(live.signatures.size(), 2);

    live = snapshot;
    const auto expected = make_tables();

    ASSERT_EQ(live.rules.size(), expected.rules.size());
    for (std::size_t i = 0; i < live.rules.size(); ++i) {
        EXPECT_EQ(live.rules[i].non_terminal, expected.rules[i].non_terminal);
        EXPECT_EQ(live.rules[i].derivation, expected.rules[i].derivation);
        EXPECT_EQ(live.rules[i].versions.from, expected.rules[i].versions.from);
        EXPECT_EQ(live.rules[i].versions.to, expected.rules[i].versions.to);
        EXPECT_TRUE(static_cast<bool>(live.rules[i].process));
    }
    EXPECT_EQ(live.enum_values, expected.enum_values);
    ASSERT_EQ(live.enums.size(), expected.enums.size());
    EXPECT_EQ(live.enums[0].name, expected.enums[0].name);
    EXPECT_EQ(live.enums[0].values, expected.enums[0].values);
    EXPECT_EQ(live.enum_lookup, expected.enum_lookup);
    EXPECT_EQ(live.enum_value_lookup, expected.enum_value_lookup);
    EXPECT_EQ(live.chained_subcommand_values, expected.chained_subcommand_values);
    ASSERT_EQ(live.chained_subcommands.size(), expected.chained_subcommands.size());
    EXPECT_EQ(live.chained_subcommands[0].values, expected.chained_subcommands[0].values);
    EXPECT_EQ(live.chained_subcommand_lookup, expected.chained_subcommand_lookup);
    EXPECT_EQ(live.chained_subcommand_value_lookup, expected.chained_subcommand_value_lookup);
    ASSERT_EQ(live.factorizations.size(), expected.factorizations.size());
    EXPECT_EQ(live.factorizations[0].command_symbol, expected.factorizations[0].command_symbol);
    EXPECT_EQ(live.postfixes, expected.postfixes);
    EXPECT_EQ(live.command_symbols, expected.command_symbols);
    ASSERT_EQ(live.signatures.size(), expected.signatures.size());
    const auto &say = live.signatures.at("say");
    EXPECT_EQ(say.name, "say");
    EXPECT_EQ(say.first_rule, expected.signatures.at("say").first_rule);
    ASSERT_EQ(say.overloads.size(), 1);
    ASSERT_EQ(say.overloads[0].params.size(), 1);
    EXPECT_EQ(say.overloads[0].params[0].name, "target");
    EXPECT_EQ(say.overloads[0].params[0].value_get_fn, &get_slot);
    EXPECT_EQ(say.overloads[0].params_symbol, expected.signatures.at("say").overloads[0].params_symbol);
    EXPECT_TRUE(live.type_lookup.empty());
    EXPECT_EQ(live.aliases, expected.aliases);
    EXPECT_EQ(live.semantic_constraints, expected.semantic_constraints);
    EXPECT_EQ(live.semantic_constraint_lookup, expected.semantic_constraint_lookup);
    ASSERT_EQ(live.constrained_values.size(), expected.constrained_values.size());
    EXPECT_EQ(live.constrained_values[0].value, expected.constrained_values[0].value);
    EXPECT_EQ(live.constrained_values[0].enum_symbol, expected.constrained_values[0].enum_symbol);
    EXPECT_EQ(live.constrained_values[0].constraints, expected.constrained_values[0].constraints);
    EXPECT_EQ(live.constrained_value_lookup, expected.constrained_value_lookup);
    ASSERT_EQ(live.soft_enums.size(), expected.soft_enums.size());
    EXPECT_EQ(live.soft_enums[0].name, expected.soft_enums[0].name);
    EXPECT_EQ(live.soft_enums[0].values, expected.soft_enums[0].values);
    EXPECT_EQ(live.soft_enum_lookup, expected.soft_enum_lookup);
}
