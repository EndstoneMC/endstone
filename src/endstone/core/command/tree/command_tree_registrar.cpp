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

#include "endstone/core/command/tree/command_tree_registrar.h"

#include <format>

#include "endstone/core/command/tree/tree_command.h"

namespace endstone::core {

CommandParameterData CommandTreeRegistrar::basicParameter(const std::string &name, TreeCommand &command)
{
    return CommandParameterData({0}, &CommandRegistry::ParseRuleFor<TreeCommandAdapter>::instance, command.intern(name),
                                CommandParameterDataType::Basic, nullptr, nullptr, 0, false, -1);
}

std::string CommandTreeRegistrar::uniqueEnumName(const std::string &wanted, const CommandRegistry &registry)
{
    std::string name = wanted;
    int suffix = 0;
    while (true) {
        const auto it = registry.enum_lookup_.find(name);
        if (it == registry.enum_lookup_.end() || registry.enums_.at(it->second).values.empty()) {
            return name;
        }
        name = std::format("{}_{}", wanted, ++suffix);
    }
}

nonstd::expected<CommandParameterData, std::string> CommandTreeRegistrar::existingEnumParameter(
    const std::string &name, const char *enum_name, TreeCommand &command, CommandRegistry &registry)
{
    const auto it = registry.enum_lookup_.find(enum_name);
    if (it == registry.enum_lookup_.end()) {
        return nonstd::make_unexpected(std::format("The command registry has no enum named '{}'.", enum_name));
    }
    auto data = basicParameter(name, command);
    data.param_type = CommandParameterDataType::Enum;
    data.enum_name_or_postfix = it->first.c_str();
    data.enum_or_postfix_symbol = CommandRegistry::Symbol::fromEnumIndex(it->second).value();
    return data;
}

nonstd::expected<CommandParameterData, std::string> CommandTreeRegistrar::newEnumParameter(
    const std::string &name, const std::string &wanted_enum_name, const std::vector<std::string> &values,
    TreeCommand &command, CommandRegistry &registry)
{
    if (values.empty()) {
        return nonstd::make_unexpected(std::format("Enum '{}' has no values.", wanted_enum_name));
    }

    const auto enum_name = uniqueEnumName(wanted_enum_name, registry);
    const auto symbol = registry.addEnumValues(enum_name, values);
    const auto it = registry.enum_lookup_.find(enum_name);
    if (it == registry.enum_lookup_.end()) {
        return nonstd::make_unexpected(std::format("Unable to register enum '{}'.", enum_name));
    }

    auto data = basicParameter(name, command);
    data.param_type = CommandParameterDataType::Enum;
    data.enum_name_or_postfix = it->first.c_str();
    data.enum_or_postfix_symbol = symbol;
    data.options = CommandParameterOption::EnumAutocompleteExpansion;
    return data;
}

std::uint32_t CommandTreeRegistrar::findOrCreateSoftEnum(const std::string &name, CommandRegistry &registry)
{
    const auto it = registry.soft_enum_lookup_.find(name);
    if (it != registry.soft_enum_lookup_.end()) {
        return it->second;
    }
    const auto index = static_cast<std::uint32_t>(registry.soft_enums_.size());
    registry.soft_enums_.push_back({name, {}});
    registry.soft_enum_lookup_.emplace(name, index);
    return index;
}

nonstd::expected<CommandParameterData, std::string> CommandTreeRegistrar::softEnumParameter(
    const std::string &name, const std::string &enum_name, TreeCommand &command, CommandRegistry &registry)
{
    if (enum_name.empty()) {
        return nonstd::make_unexpected(std::format("Argument '{}' is a soft enum without a name.", name));
    }
    const auto index = findOrCreateSoftEnum(enum_name, registry);

    auto data = basicParameter(name, command);
    data.param_type = CommandParameterDataType::SoftEnum;
    data.enum_name_or_postfix = registry.soft_enums_.at(index).name.c_str();
    data.enum_or_postfix_symbol = CommandRegistry::Symbol::fromSoftEnumIndex(index).value();
    return data;
}

nonstd::expected<CommandParameterData, std::string> CommandTreeRegistrar::hardNonTerminalParameter(
    const std::string &name, CommandRegistry::HardNonTerminal symbol, TreeCommand &command)
{
    auto data = basicParameter(name, command);
    data.chained_subcommand_symbol = static_cast<int>(symbol);
    return data;
}

nonstd::expected<CommandParameterData, std::string> CommandTreeRegistrar::makeLiteralParameter(
    const std::string &literal, TreeCommand &command, CommandRegistry &registry)
{
    return newEnumParameter(literal, literal, {literal}, command, registry);
}

nonstd::expected<CommandParameterData, std::string> CommandTreeRegistrar::makeArgumentParameter(
    const std::string &name, const ArgumentType &type, TreeCommand &command, CommandRegistry &registry)
{
    using HardNonTerminal = CommandRegistry::HardNonTerminal;

    switch (type.getKind()) {
    case ArgumentKind::Boolean:
        return existingEnumParameter(name, "Boolean", command, registry);
    case ArgumentKind::BlockType:
        return existingEnumParameter(name, "Block", command, registry);
    case ArgumentKind::EntityType:
        return existingEnumParameter(name, "EntityType", command, registry);
    case ArgumentKind::Integer:
        return hardNonTerminalParameter(name, HardNonTerminal::Int, command);
    case ArgumentKind::Float:
        return hardNonTerminalParameter(name, HardNonTerminal::Val, command);
    case ArgumentKind::String:
        return hardNonTerminalParameter(name, HardNonTerminal::Id, command);
    case ArgumentKind::Message:
        return hardNonTerminalParameter(name, HardNonTerminal::MessageRoot, command);
    case ArgumentKind::RawText:
        return hardNonTerminalParameter(name, HardNonTerminal::RawText, command);
    case ArgumentKind::Json:
        return hardNonTerminalParameter(name, HardNonTerminal::JsonObject, command);
    case ArgumentKind::Enumeration: {
        const auto &builtin = static_cast<const BuiltinArgumentType &>(type);
        return newEnumParameter(name, builtin.getName(), builtin.getValues(), command, registry);
    }
    case ArgumentKind::SoftEnum: {
        const auto &builtin = static_cast<const BuiltinArgumentType &>(type);
        return softEnumParameter(name, builtin.getName(), command, registry);
    }
    case ArgumentKind::Custom: {
        const auto native = type.getNativeType();
        if (!native) {
            return nonstd::make_unexpected(std::format(
                "Argument '{}' is a custom type but does not name a native type to be presented as.", name));
        }
        return makeArgumentParameter(name, *native, command, registry);
    }
    default:
        return nonstd::make_unexpected(std::format("Argument '{}' uses a type that is not supported yet.", name));
    }
}

bool CommandTreeRegistrar::setSoftEnumValues(const std::string &name, std::vector<std::string> values,
                                             CommandRegistry &registry)
{
    findOrCreateSoftEnum(name, registry);
    registry.setSoftEnumValues(name, std::move(values));
    return true;
}

}  // namespace endstone::core
