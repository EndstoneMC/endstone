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

#include <gtest/gtest.h>

#include "endstone/detail/command/command_lexer.h"

using endstone::detail::CommandLexer;

class CommandLexerTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        // Prepare context if needed
    }

    void TearDown() override
    {
        // Cleanup context if needed
    }
};

CommandLexer::Token makeToken(CommandLexer::TokenType type, std::string_view value = "")
{
    return {type, value};
}

TEST_F(CommandLexerTest, TestCommandWithoutParameters)
{
    CommandLexer lexer("/command");
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Slash, "/"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "command"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::End, "\0"));
}

TEST_F(CommandLexerTest, TestCommandWithRequiredParameter)
{
    CommandLexer lexer("/command <text: string>");
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Slash, "/"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "command"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::LessThan, "<"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "text"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Colon, ":"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "string"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::GreaterThan, ">"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::End, "\0"));
}

TEST_F(CommandLexerTest, TestCommandWithOptionalParameter)
{
    CommandLexer lexer("/command [text: string]");
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Slash, "/"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "command"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::LeftSquare, "["));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "text"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Colon, ":"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "string"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::RightSquare, "]"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::End, "\0"));
}

TEST_F(CommandLexerTest, TestCommandWithEnumParameter)
{
    CommandLexer lexer("/command <mode: (add |set|query)>");
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Slash, "/"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "command"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::LessThan, "<"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "mode"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Colon, ":"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::LeftParen, "("));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "add"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Pipe, "|"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "set"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Pipe, "|"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::Identifier, "query"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::RightParen, ")"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::GreaterThan, ">"));
    EXPECT_EQ(lexer.next(), makeToken(CommandLexer::TokenType::End, "\0"));
}
