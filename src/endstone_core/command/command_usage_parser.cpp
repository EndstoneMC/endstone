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

#include <fmt/format.h>

namespace endstone::detail {

bool CommandUsageParser::parse(std::string &command_name, std::vector<Parameter> &parameters,
                               std::string &error_message)
{
    lexer_.reset();

    auto token = lexer_.next();
    if (!token.is(CommandLexer::TokenType::Slash)) {
        error_message = fmt::format("Syntax Error: expect '/', got '{}' at position {}.", token.value, lexer_.position);
        return false;
    }

    if (!parseIdentifier(command_name, error_message)) {
        return false;
    }

    do {
        token = lexer_.next();
        if (token.is(CommandLexer::TokenType::End)) {
            return true;
        }

        if (!token.is(CommandLexer::TokenType::LessThan) && !token.is(CommandLexer::TokenType::LeftSquare)) {
            error_message =
                fmt::format("Syntax Error: expect '<' or '[', got '{}' at position {}.", token.value, lexer_.position);
            return false;
        }

        bool optional = token.type == CommandLexer::TokenType::LeftSquare;
        if (!parseParameter(parameters, optional, error_message)) {
            return false;
        }
    } while (true);
}

bool CommandUsageParser::parseIdentifier(std::string &id, std::string &error_message)
{
    auto token = lexer_.next();
    if (token.type != CommandLexer::TokenType::Identifier) {
        error_message =
            fmt::format("Syntax Error: expect identifier, got '{}' at position {}.", token.value, lexer_.position);
        return false;
    }
    id = std::string(token.value);
    return true;
}

bool CommandUsageParser::parseParameter(std::vector<Parameter> &parameters, bool optional, std::string &error_message)
{
    std::string name;
    if (!parseIdentifier(name, error_message)) {
        return false;
    }

    auto token = lexer_.next();
    if (!token.is(CommandLexer::TokenType::Colon)) {
        error_message = fmt::format("Syntax Error: expect ':', got '{}' at position {}.", token.value, lexer_.position);
        return false;
    }

    token = lexer_.next();
    std::string type;
    std::vector<std::string> values;
    if (token.is(CommandLexer::TokenType::Identifier)) {
        type = token.value;
    }
    else if (token.is(CommandLexer::TokenType::LeftParen)) {
        type = "enum";
        do {
            token = lexer_.next();
            if (token.is(CommandLexer::TokenType::Identifier)) {
                values.emplace_back(token.value);
                token = lexer_.next();
                if (token.is(CommandLexer::TokenType::RightParen)) {
                    break;
                }
                if (!token.is(CommandLexer::TokenType::Pipe)) {
                    error_message = fmt::format("Syntax Error: expect ')' or '|', got '{}' at position {}.",
                                                token.value, lexer_.position);
                    return false;
                }
            }
            else {
                error_message = fmt::format("Syntax Error: expect identifier, got '{}' at position {}.", token.value,
                                            lexer_.position);
                return false;
            }
        } while (true);
    }
    else {
        error_message = fmt::format("Syntax Error: expect enums or identifier, got '{}' at position {}.", token.value,
                                    lexer_.position);
        return false;
    }

    token = lexer_.next();
    if (optional) {
        if (!token.is(CommandLexer::TokenType::RightSquare)) {
            error_message =
                fmt::format("Syntax Error: expect ']', got '{}' at position {}.", token.value, lexer_.position);
            return false;
        }
    }
    else {
        if (!token.is(CommandLexer::TokenType::GreaterThan)) {
            error_message =
                fmt::format("Syntax Error: expect '>', got '{}' at position {}.", token.value, lexer_.position);
            return false;
        }
    }

    parameters.push_back({name, type, optional, values});
    return true;
}

}  // namespace endstone::detail
