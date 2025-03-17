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

#include "endstone/core/command/command_usage_parser.h"

#include <utility>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <fmt/format.h>

namespace {
void generateNameAndType(std::string_view command_name, endstone::core::CommandUsageParser::Parameter &parameter)
{
    if (!parameter.is_enum) {
        return;
    }
    parameter.name = boost::algorithm::join(parameter.values, "_");
    parameter.type = command_name;
    parameter.type[0] = std::toupper(static_cast<unsigned char>(parameter.type[0]));
    for (auto value : parameter.values) {
        value[0] = std::toupper(static_cast<unsigned char>(value[0]));
        parameter.type.append(value);
    }
}
}  // namespace

namespace endstone::core {

nonstd::expected<CommandUsageParser::Result, std::string> CommandUsageParser::parse() noexcept
{
    Result result;
    lexer_.reset();

    // Expect a slash at the start.
    if (auto slash = parseToken(CommandLexer::TokenType::Slash, "/"); !slash) {
        return nonstd::make_unexpected(slash.error());
    }

    // Parse command name.
    auto command_name_result = parseIdentifier("command name");
    if (!command_name_result) {
        return nonstd::make_unexpected(command_name_result.error());
    }
    result.command_name = std::string(command_name_result.value());
    command_name_ = result.command_name;

    // Parse parameters (or enums).
    while (true) {
        auto token = lexer_.peek();
        switch (token.type) {
        case CommandLexer::TokenType::Identifier: {
            auto enum_result = parseSingleUnnamedEnum();
            if (!enum_result) {
                return nonstd::make_unexpected(enum_result.error());
            }
            result.parameters.push_back(enum_result.value());
            break;
        }
        case CommandLexer::TokenType::LeftParen: {
            auto enum_result = parseEnum();
            if (!enum_result) {
                return nonstd::make_unexpected(enum_result.error());
            }
            result.parameters.push_back(enum_result.value());
            break;
        }
        case CommandLexer::TokenType::LeftSquare:
        case CommandLexer::TokenType::LessThan: {
            auto param_result = parseParameterOrEnum();
            if (!param_result) {
                return nonstd::make_unexpected(param_result.error());
            }
            result.parameters.push_back(param_result.value());
            break;
        }
        case CommandLexer::TokenType::End:
            return result;
        default:
            return nonstd::make_unexpected(fmt::format("Syntax Error: expect '(', '<' or '[', got '{}' at position {}.",
                                                       token.value, lexer_.getPosition()));
        }
    }
}

nonstd::expected<std::string_view, std::string> CommandUsageParser::parseToken(CommandLexer::TokenType type,
                                                                               std::string what)
{
    auto token = lexer_.next();
    if (token.type != type) {
        return nonstd::make_unexpected(fmt::format("Syntax Error: expect '{}', got '{}' at position {}.", what,
                                                   token.value, lexer_.getPosition()));
    }
    return token.value;
}

nonstd::expected<std::string_view, std::string> CommandUsageParser::parseIdentifier(std::string what)
{
    return parseToken(CommandLexer::TokenType::Identifier, std::move(what));
}

nonstd::expected<CommandUsageParser::Parameter, std::string> CommandUsageParser::parseParameterOrEnum()
{
    Parameter param;
    auto token = lexer_.next();
    switch (token.type) {
    case CommandLexer::TokenType::LeftSquare:
        param.optional = true;
        break;
    case CommandLexer::TokenType::LessThan:
        param.optional = false;
        break;
    default:
        return nonstd::make_unexpected(fmt::format("Syntax Error: expect '<' or '[', got '{}' at position {}.",
                                                   token.value, lexer_.getPosition()));
    }

    auto name_result = parseIdentifier("parameter name' or 'enum value");
    if (!name_result) {
        return nonstd::make_unexpected(name_result.error());
    }

    token = lexer_.peek();
    switch (token.type) {
    case CommandLexer::TokenType::Colon: {  // parameter
        param.name = std::string(name_result.value());
        if (auto colon = parseToken(CommandLexer::TokenType::Colon, ":"); !colon) {
            return nonstd::make_unexpected(colon.error());
        }

        auto type_result = parseIdentifier("parameter type");
        if (!type_result) {
            return nonstd::make_unexpected(type_result.error());
        }
        param.type = std::string(type_result.value());
        break;
    }
    case CommandLexer::TokenType::Pipe: {  // unnamed enum (multiple)
        param.is_enum = true;
        param.values.emplace_back(name_result.value());
        if (auto pipe = parseToken(CommandLexer::TokenType::Pipe, "|"); !pipe) {
            return nonstd::make_unexpected(pipe.error());
        }
        bool finished = false;
        while (!finished) {
            auto enum_result = parseIdentifier("enum value");
            if (!enum_result) {
                return nonstd::make_unexpected(enum_result.error());
            }
            param.values.emplace_back(enum_result.value());
            token = lexer_.peek();
            switch (token.type) {
            case CommandLexer::TokenType::Pipe:
                if (auto pipe = parseToken(CommandLexer::TokenType::Pipe, "|"); !pipe) {
                    return nonstd::make_unexpected(pipe.error());
                }
                break;
            default:
                finished = true;
                break;
            }
        }
        generateNameAndType(command_name_, param);
        break;
    }
    case CommandLexer::TokenType::RightSquare:
    case CommandLexer::TokenType::GreaterThan: {  // unnamed enum (single)
        param.is_enum = true;
        param.values.emplace_back(name_result.value());
        generateNameAndType(command_name_, param);
        break;
    }
    default:
        return nonstd::make_unexpected(fmt::format(
            "Syntax Error: expect ':', '|', '>' or ']', got '{}' at position {}.", token.value, lexer_.getPosition()));
    }

    if (param.optional) {
        if (auto rsq = parseToken(CommandLexer::TokenType::RightSquare, "]"); !rsq) {
            return nonstd::make_unexpected(rsq.error());
        }
    }
    else {
        if (auto gt = parseToken(CommandLexer::TokenType::GreaterThan, ">"); !gt) {
            return nonstd::make_unexpected(gt.error());
        }
    }
    return param;
}

nonstd::expected<CommandUsageParser::Parameter, std::string> CommandUsageParser::parseEnum()
{
    // Parse the enum's opening parenthesis.
    if (auto lp = parseToken(CommandLexer::TokenType::LeftParen, "("); !lp) {
        return nonstd::make_unexpected(lp.error());
    }

    std::vector<std::string> values;
    auto token = lexer_.peek();
    switch (token.type) {
    case CommandLexer::TokenType::RightParen: {
        if (auto rp = parseToken(CommandLexer::TokenType::RightParen, ")"); !rp) {
            return nonstd::make_unexpected(rp.error());
        }
        break;
    }
    case CommandLexer::TokenType::Identifier: {
        bool finished = false;
        while (!finished) {
            auto enum_result = parseIdentifier("enum value");
            if (!enum_result) {
                return nonstd::make_unexpected(enum_result.error());
            }
            values.emplace_back(enum_result.value());

            token = lexer_.next();
            switch (token.type) {
            case CommandLexer::TokenType::RightParen:
                finished = true;
                break;
            case CommandLexer::TokenType::Pipe:
                break;
            default:
                return nonstd::make_unexpected(fmt::format("Syntax Error: expect ')' or '|', got '{}' at position {}.",
                                                           token.value, lexer_.getPosition()));
            }
        }
        break;
    }
    default:
        return nonstd::make_unexpected(fmt::format(
            "Syntax Error: expect ')' or 'enum values', got '{}' at position {}.", token.value, lexer_.getPosition()));
    }

    // Parse the parameter associated with this enum.
    auto param_result = parseParameterOrEnum();
    if (!param_result) {
        return nonstd::make_unexpected(param_result.error());
    }
    Parameter param = param_result.value();
    param.is_enum = true;
    param.values = std::move(values);
    return param;
}

nonstd::expected<CommandUsageParser::Parameter, std::string> CommandUsageParser::parseSingleUnnamedEnum()
{
    auto enum_result = parseIdentifier("enum value");
    if (!enum_result) {
        return nonstd::make_unexpected(enum_result.error());
    }
    Parameter param;
    param.is_enum = true;
    param.optional = false;
    param.values.emplace_back(enum_result.value());
    generateNameAndType(command_name_, param);
    return param;
}

}  // namespace endstone::core
