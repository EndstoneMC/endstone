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

#include "endstone/detail/command/command_usage_parser.h"

#include <utility>

#include <fmt/format.h>

namespace endstone::detail {

bool CommandUsageParser::parse(std::string &command_name, std::vector<Parameter> &parameters,
                               std::string &error_message) noexcept
{
    try {
        lexer_.reset();
        parseToken(CommandLexer::TokenType::Slash, "/");
        command_name = parseIdentifier("command name");
        do {
            auto token = lexer_.peek();
            switch (token.type) {
            case CommandLexer::TokenType::LeftParen:
                parameters.push_back(parseEnum());
                break;
            case CommandLexer::TokenType::LeftSquare:
            case CommandLexer::TokenType::LessThan:
                parameters.push_back(parseParameter());
                break;
            case CommandLexer::TokenType::End:
                return true;
            default:
                throw std::runtime_error(fmt::format("Syntax Error: expect '(', '<' or '[', got '{}' at position {}.",
                                                     token.value, lexer_.getPosition()));
            }
        } while (true);
    }
    catch (std::exception &e) {
        error_message = e.what();
        return false;
    }
}

std::string_view CommandUsageParser::parseToken(CommandLexer::TokenType type, std::string what)
{
    auto token = lexer_.next();
    if (token.type != type) {
        throw std::runtime_error(fmt::format("Syntax Error: expect '{}', got '{}' at position {}.", what, token.value,
                                             lexer_.getPosition()));
    }
    return token.value;
}

std::string_view CommandUsageParser::parseIdentifier(std::string what)
{
    return parseToken(CommandLexer::TokenType::Identifier, std::move(what));
}

CommandUsageParser::Parameter CommandUsageParser::parseParameter()
{
    auto param = CommandUsageParser::Parameter();
    auto token = lexer_.next();
    switch (token.type) {
    case CommandLexer::TokenType::LeftSquare:
        param.optional = true;
        break;
    case CommandLexer::TokenType::LessThan:
        param.optional = false;
        break;
    default:
        throw std::runtime_error(fmt::format("Syntax Error: expect '<' or '[', got '{}' at position {}.", token.value,
                                             lexer_.getPosition()));
    };

    param.name = parseIdentifier("parameter name");
    parseToken(CommandLexer::TokenType::Colon, ":");
    param.type = parseIdentifier("parameter type");

    if (param.optional) {
        parseToken(CommandLexer::TokenType::RightSquare, "]");
    }
    else {
        parseToken(CommandLexer::TokenType::GreaterThan, ">");
    }

    return param;
}

CommandUsageParser::Parameter CommandUsageParser::parseEnum()
{
    parseToken(CommandLexer::TokenType::LeftParen, "(");
    std::vector<std::string> values;

    auto token = lexer_.peek();
    switch (token.type) {
    case CommandLexer::TokenType::RightParen:
        parseToken(CommandLexer::TokenType::RightParen, ")");
        break;
    case CommandLexer::TokenType::Identifier: {
        bool finished = false;
        do {
            values.emplace_back(parseIdentifier("enum value"));
            token = lexer_.next();
            switch (token.type) {
            case CommandLexer::TokenType::RightParen:
                finished = true;
                break;
            case CommandLexer::TokenType::Pipe:
                break;
            default:
                throw std::runtime_error(fmt::format("Syntax Error: expect ')' or '|', got '{}' at position {}.",
                                                     token.value, lexer_.getPosition()));
            }
        } while (!finished);
        break;
    }
    default:
        throw std::runtime_error(fmt::format("Syntax Error: expect ')' or 'enum values', got '{}' at position {}.",
                                             token.value, lexer_.getPosition()));
    };

    auto param = parseParameter();
    param.is_enum = true;
    param.values = values;
    return param;
}

}  // namespace endstone::detail
