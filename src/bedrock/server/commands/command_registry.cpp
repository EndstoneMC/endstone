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

#include "bedrock/server/commands/command_registry.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

#include "bedrock/server/commands/command.h"
#include "bedrock/symbol.h"

void CommandRegistry::registerCommand(const std::string &name, const char *description, CommandPermissionLevel level,
                                      CommandFlag flag1, CommandFlag flag2)
{
    BEDROCK_CALL(&CommandRegistry::registerCommand, this, name, description, level, flag1, flag2);
}

void CommandRegistry::registerAlias(std::string name, std::string alias)
{
    BEDROCK_CALL(&CommandRegistry::registerAlias, this, std::move(name), std::move(alias));
}

const CommandRegistry::Signature *CommandRegistry::findCommand(const std::string &name) const
{
    if (signatures_.contains(name)) {
        return &signatures_.at(name);
    }
    if (aliases_.contains(name)) {
        return &signatures_.at(aliases_.at(name));
    }
    return nullptr;
}

CommandRegistry::NonTerminal CommandRegistry::findEnum(const std::string &name) const
{
    const auto it = enum_lookup_.find(name);
    if (it == enum_lookup_.end()) {
        return Symbol{0};
    }
    return Symbol::fromEnumIndex(it->second);
}

CommandRegistry::Terminal CommandRegistry::findEnumValue(const std::string &name) const
{
    const auto it = enum_value_lookup_.find(name);
    if (it == enum_value_lookup_.end()) {
        return Symbol{0};
    }
    return Symbol::fromEnumValueIndex(it->second);
}

// std::string CommandRegistry::describe(const CommandParameterData &param) const
// {
//     std::string (CommandRegistry::*fp)(const CommandParameterData &param) const = &CommandRegistry::describe;
//     return BEDROCK_CALL(fp, this, param);
// }

// std::string CommandRegistry::describe(const Signature & /*command*/, const std::string &alias, const Overload
// &overload,
//                                       unsigned int highlight, unsigned int *start, unsigned int *length) const
// {
//     std::stringstream ss;
//     ss << "/" << alias;
//     for (auto i = 0; i < overload.params.size(); i++) {
//         if (i == highlight && start != nullptr) {
//             *start = ss.tellp();
//         }
//         auto param_syntax = describe(overload.params[i]);
//         if (i == highlight && length != nullptr) {
//             *length = param_syntax.size();
//         }
//         ss << " " << param_syntax;
//     }
//     return ss.str();
// }

std::string CommandRegistry::symbolToString(Symbol symbol) const
{
    return BEDROCK_CALL(&CommandRegistry::symbolToString, this, symbol);
}

void CommandRegistry::registerOverloadInternal(Signature &signature, Overload &overload)
{
    BEDROCK_CALL(&CommandRegistry::registerOverloadInternal, this, signature, overload);
}

AvailableCommandsPacket CommandRegistry::serializeAvailableCommands() const
{
    return BEDROCK_CALL(&CommandRegistry::serializeAvailableCommands, this);
}

int CommandRegistry::addSoftEnum(const std::string &name, std::vector<std::string> values)
{
    return BEDROCK_CALL(&CommandRegistry::addSoftEnum, this, name, std::move(values));
}

void CommandRegistry::setSoftEnumValues(const std::string &enum_name, std::vector<std::string> values)
{
    BEDROCK_CALL(&CommandRegistry::setSoftEnumValues, this, enum_name, std::move(values));
}

void CommandRegistry::addSoftEnumValues(const std::string &enum_name, std::vector<std::string> values)
{
    BEDROCK_CALL(&CommandRegistry::addSoftEnumValues, this, enum_name, std::move(values));
}

void CommandRegistry::removeSoftEnumValues(const std::string &enum_name, std::vector<std::string> values)
{
    BEDROCK_CALL(&CommandRegistry::removeSoftEnumValues, this, enum_name, std::move(values));
}

int CommandRegistry::addEnumValues(const std::string &name, const std::vector<std::string> &values)
{
    return BEDROCK_CALL(&CommandRegistry::addEnumValues, this, name, values);
}

CommandRegistry::Symbol CommandRegistry::_addEnumValuesInternal(
    const std::string &name, const std::vector<std::pair<std::string, std::uint64_t>> &values,
    Bedrock::typeid_t<CommandRegistry> type, const ParseFunction parse)
{
    return BEDROCK_CALL(&CommandRegistry::_addEnumValuesInternal, this, name, values, type, parse);
}

template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<std::string>()
{
    return BEDROCK_VAR(const ParamParseRule *, "CommandRegistry::ParseRuleFor<std::string>::instance");
}

template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<int>()
{
    return BEDROCK_VAR(const ParamParseRule *, "CommandRegistry::ParseRuleFor<int>::instance");
}

template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<float>()
{
    return BEDROCK_VAR(const ParamParseRule *, "CommandRegistry::ParseRuleFor<float>::instance");
}

template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandMessage>()
{
    return BEDROCK_VAR(const ParamParseRule *, "CommandRegistry::ParseRuleFor<CommandMessage>::instance");
}

template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandRawText>()
{
    return BEDROCK_VAR(const ParamParseRule *, "CommandRegistry::ParseRuleFor<CommandRawText>::instance");
}

template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandSelector<Actor>>()
{
    return BEDROCK_VAR(const ParamParseRule *, "CommandRegistry::ParseRuleFor<CommandSelector<Actor>>::instance");
}

template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandSelector<Player>>()
{
    return BEDROCK_VAR(const ParamParseRule *, "CommandRegistry::ParseRuleFor<CommandSelector<Player>>::instance");
}

template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandPosition>()
{
    return BEDROCK_VAR(const ParamParseRule *, "CommandRegistry::ParseRuleFor<CommandPosition>::instance");
}

template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandPositionFloat>()
{
    return BEDROCK_VAR(const ParamParseRule *, "CommandRegistry::ParseRuleFor<CommandPositionFloat>::instance");
}

template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<Json::Value>()
{
    return BEDROCK_VAR(const ParamParseRule *, "CommandRegistry::ParseRuleFor<Json::Value>::instance");
}

template <>
Bedrock::typeid_t<CommandRegistry> CommandRegistry::getTypeId<std::string>()
{
    return *BEDROCK_VAR(const Bedrock::typeid_t<CommandRegistry> *,
                        "Bedrock::type_id<CommandRegistry, std::string>::id");
}
