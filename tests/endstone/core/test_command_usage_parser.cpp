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

#include "endstone/core/command/command_usage_parser.h"

using endstone::core::CommandUsageParser;

// Define the fixture class
class ParserTest : public ::testing::Test {
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

TEST_F(ParserTest, ParseCommandWithoutParameters)
{
    CommandUsageParser parser("/command");
    auto result = parser.parse();
    ASSERT_EQ(result.has_value(), true);

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 0);
}

TEST_F(ParserTest, ParseCommandWithMandatoryParameter)
{
    CommandUsageParser parser("/command <text: string>");
    auto result = parser.parse();
    ASSERT_EQ(result.has_value(), true);

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "text");
    ASSERT_EQ(parameters[0].type, "string");
    ASSERT_EQ(parameters[0].optional, false);
    ASSERT_EQ(parameters[0].is_enum, false);
}

TEST_F(ParserTest, ParseCommandWithOptionalParameter)
{
    CommandUsageParser parser("/command [text: string]");
    auto result = parser.parse();
    ASSERT_EQ(result.has_value(), true);

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "text");
    ASSERT_EQ(parameters[0].type, "string");
    ASSERT_EQ(parameters[0].optional, true);
    ASSERT_EQ(parameters[0].is_enum, false);
}

TEST_F(ParserTest, ParseCommandWithMandatoryEnumParameter)
{
    CommandUsageParser parser("/command (add | set | query)<enum: EnumType>");
    auto result = parser.parse();
    ASSERT_EQ(result.has_value(), true);

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "enum");
    ASSERT_EQ(parameters[0].type, "EnumType");
    ASSERT_EQ(parameters[0].values, std::vector<std::string>({"add", "set", "query"}));
    ASSERT_EQ(parameters[0].optional, false);
    ASSERT_EQ(parameters[0].is_enum, true);
}

TEST_F(ParserTest, ParseCommandWithOptionalEnumParameter)
{
    CommandUsageParser parser("/command (add | set | query)[enum: EnumType]");
    auto result = parser.parse();
    ASSERT_EQ(result.has_value(), true);

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "enum");
    ASSERT_EQ(parameters[0].type, "EnumType");
    ASSERT_EQ(parameters[0].values, std::vector<std::string>({"add", "set", "query"}));
    ASSERT_EQ(parameters[0].optional, true);
    ASSERT_EQ(parameters[0].is_enum, true);
}

TEST_F(ParserTest, ParseCommandWithEmptyEnumParameter)
{
    CommandUsageParser parser("/command ()[enum: EnumType]");
    auto result = parser.parse();
    ASSERT_EQ(result.has_value(), true);

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "enum");
    ASSERT_EQ(parameters[0].type, "EnumType");
    ASSERT_EQ(parameters[0].values.empty(), true);
    ASSERT_EQ(parameters[0].optional, true);
    ASSERT_EQ(parameters[0].is_enum, true);
}

TEST_F(ParserTest, ParseCommandWithOneEnumParameter)
{
    CommandUsageParser parser("/command (set)[enum: EnumType]");
    auto result = parser.parse();
    ASSERT_EQ(result.has_value(), true);

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "enum");
    ASSERT_EQ(parameters[0].type, "EnumType");
    ASSERT_EQ(parameters[0].values, std::vector<std::string>({"set"}));
    ASSERT_EQ(parameters[0].optional, true);
    ASSERT_EQ(parameters[0].is_enum, true);
}

TEST_F(ParserTest, ParseCommandWithOneEnumParameterShorthand)
{
    CommandUsageParser parser("/command set");
    auto result = parser.parse();
    ASSERT_EQ(result.has_value(), true);

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "set");
    ASSERT_EQ(parameters[0].type, "CommandSet");
    ASSERT_EQ(parameters[0].values, std::vector<std::string>({"set"}));
    ASSERT_EQ(parameters[0].optional, false);
    ASSERT_EQ(parameters[0].is_enum, true);
}

TEST_F(ParserTest, ParseCommandWithOneEnumParameterShorthandMandatory)
{
    CommandUsageParser parser("/command <set>");
    auto result = parser.parse();
    ASSERT_EQ(result.has_value(), true);

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "set");
    ASSERT_EQ(parameters[0].type, "CommandSet");
    ASSERT_EQ(parameters[0].values, std::vector<std::string>({"set"}));
    ASSERT_EQ(parameters[0].optional, false);
    ASSERT_EQ(parameters[0].is_enum, true);
}

TEST_F(ParserTest, ParseCommandWithOneEnumParameterShorthandOptional)
{
    CommandUsageParser parser("/command [set]");
    auto result = parser.parse();
    ASSERT_EQ(result.has_value(), true);

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "set");
    ASSERT_EQ(parameters[0].type, "CommandSet");
    ASSERT_EQ(parameters[0].values, std::vector<std::string>({"set"}));
    ASSERT_EQ(parameters[0].optional, true);
    ASSERT_EQ(parameters[0].is_enum, true);
}

TEST_F(ParserTest, ParseCommandWithUnnamedEnumParameterMandatory)
{
    CommandUsageParser parser("/command <add|set>");
    auto result = parser.parse();
    ASSERT_TRUE(result.has_value());

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);

    // Expect auto-generated parameter: name "add_set", type "CommandAddSet"
    ASSERT_EQ(parameters[0].name, "add_set");
    ASSERT_EQ(parameters[0].type, "CommandAddSet");
    ASSERT_EQ(parameters[0].values, std::vector<std::string>({"add", "set"}));
    ASSERT_FALSE(parameters[0].optional);
    ASSERT_TRUE(parameters[0].is_enum);
}

TEST_F(ParserTest, ParseCommandWithUnnamedEnumParameterOptional)
{
    CommandUsageParser parser("/command [add|set]");
    auto result = parser.parse();
    ASSERT_TRUE(result.has_value());

    const auto &[command_name, parameters] = result.value();
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);

    // Expect auto-generated parameter: name "add_set", type "CommandAddSet"
    ASSERT_EQ(parameters[0].name, "add_set");
    ASSERT_EQ(parameters[0].type, "CommandAddSet");
    ASSERT_EQ(parameters[0].values, std::vector<std::string>({"add", "set"}));
    ASSERT_TRUE(parameters[0].optional);
    ASSERT_TRUE(parameters[0].is_enum);
}

TEST_F(ParserTest, ParseCommandWithError)
{
    CommandUsageParser parser("/some_command <param1 int>");
    auto result = parser.parse();

    ASSERT_EQ(result.has_value(), false);
    ASSERT_EQ(result.error(), "Syntax Error: expect ':', '|', '>' or ']', got 'int' at position 21.");
}

TEST_F(ParserTest, ParseCommandWithUnfinishedEnum)
{
    CommandUsageParser parser("/command (add|set|)<enum : EnumType>");
    auto result = parser.parse();

    ASSERT_EQ(result.has_value(), false);
    ASSERT_EQ(result.error(), "Syntax Error: expect 'enum value', got ')' at position 19.");
}

TEST_F(ParserTest, ParseCommandWithUnfinishedEnumUnamed)
{
    CommandUsageParser parser("/command <add|set|>");
    auto result = parser.parse();

    ASSERT_EQ(result.has_value(), false);
    ASSERT_EQ(result.error(), "Syntax Error: expect 'enum value', got '>' at position 19.");
}

TEST_F(ParserTest, ParseCommandWithoutEndingToken)
{
    CommandUsageParser parser("/command <text:string");
    auto result = parser.parse();

    ASSERT_EQ(result.has_value(), false);
    ASSERT_EQ(result.error(), "Syntax Error: expect '>', got '' at position 21.");
}

TEST_F(ParserTest, ParseCommandWithoutSlash)
{
    CommandUsageParser parser("command <text:string>");
    auto result = parser.parse();

    ASSERT_EQ(result.has_value(), false);
    ASSERT_EQ(result.error(), "Syntax Error: expect '/', got 'command' at position 7.");
}

TEST_F(ParserTest, ParseCommandWithUnexpectedBracket)
{
    CommandUsageParser parser("/command <text:string]");
    auto result = parser.parse();

    ASSERT_EQ(result.has_value(), false);
    ASSERT_EQ(result.error(), "Syntax Error: expect '>', got ']' at position 22.");
}

TEST_F(ParserTest, ParseCommandWithUnexpectedToken)
{
    CommandUsageParser parser("/command <<text:string>");
    auto result = parser.parse();

    ASSERT_EQ(result.has_value(), false);
    ASSERT_EQ(result.error(), "Syntax Error: expect 'parameter name' or 'enum value', got '<' at position 11.");
}
