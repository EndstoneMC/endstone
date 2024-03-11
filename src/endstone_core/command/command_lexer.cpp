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

#include "endstone/detail/command/command_lexer.h"

namespace endstone::detail {

CommandLexer::Token CommandLexer::next()
{
    while (isSpace(peekChar())) {
        getChar();
    }

    char c = peekChar();
    if (isIdentifierCharacter(c) && !isDigit(c)) {
        return nextIdentifier();
    }
    if (isDigit(c)) {
        return nextNumber();
    }

    switch (c) {
    case '\0':
        return {TokenType::End, "\0"};
    default:
        return next(TokenType::Unexpected);
    case '(':
        return next(TokenType::LeftParen);
    case ')':
        return next(TokenType::RightParen);
    case '[':
        return next(TokenType::LeftSquare);
    case ']':
        return next(TokenType::RightSquare);
    case '{':
        return next(TokenType::LeftCurly);
    case '}':
        return next(TokenType::RightCurly);
    case '<':
        return next(TokenType::LessThan);
    case '>':
        return next(TokenType::GreaterThan);
    case '=':
        return next(TokenType::Equal);
    case '+':
        return next(TokenType::Plus);
    case '-':
        return next(TokenType::Minus);
    case '*':
        return next(TokenType::Asterisk);
    case '/':
        return next(TokenType::Slash);
    case '#':
        return next(TokenType::Hash);
    case '.':
        return next(TokenType::Dot);
    case ',':
        return next(TokenType::Comma);
    case ':':
        return next(TokenType::Colon);
    case ';':
        return next(TokenType::Semicolon);
    case '\'':
        return next(TokenType::SingleQuote);
    case '"':
        return next(TokenType::DoubleQuote);
    case '|':
        return next(TokenType::Pipe);
    }
}

CommandLexer::Token CommandLexer::peek()
{
    auto position = position_;
    auto result = next();
    position_ = position;
    return result;
}

CommandLexer::Token CommandLexer::next(CommandLexer::TokenType type)
{
    return CommandLexer::Token{type, value_.substr(position_++, 1)};
}

CommandLexer::Token CommandLexer::nextIdentifier()
{
    auto start = position_;
    do {
        getChar();
    } while (isIdentifierCharacter(peekChar()));
    return CommandLexer::Token{CommandLexer::TokenType::Identifier, value_.substr(start, position_ - start)};
}

CommandLexer::Token CommandLexer::nextNumber()
{
    auto start = position_;
    do {
        getChar();
    } while (isDigit(peekChar()));
    return CommandLexer::Token{CommandLexer::TokenType::Number, value_.substr(start, position_ - start)};
}

}  // namespace endstone::detail
