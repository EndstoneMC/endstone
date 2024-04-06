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

#include "endstone/detail/command/command_usage_parser.h"

using endstone::detail::CommandUsageParser;

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
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(result, true);
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 0);
    ASSERT_EQ(error_message, "");
}

TEST_F(ParserTest, ParseCommandWithMandatoryParameter)
{
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command <text: string>");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(error_message, "");
    ASSERT_EQ(result, true);
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "text");
    ASSERT_EQ(parameters[0].type, "string");
    ASSERT_EQ(parameters[0].optional, false);
    ASSERT_EQ(parameters[0].is_enum, false);
}

TEST_F(ParserTest, ParseCommandWithOptionalParameter)
{
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command [text: string]");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(error_message, "");
    ASSERT_EQ(result, true);
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "text");
    ASSERT_EQ(parameters[0].type, "string");
    ASSERT_EQ(parameters[0].optional, true);
    ASSERT_EQ(parameters[0].is_enum, false);
}

TEST_F(ParserTest, ParseCommandWithMandatoryEnumParameter)
{
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command (add | set | query)<enum: EnumType>");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(error_message, "");
    ASSERT_EQ(result, true);
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
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command (add | set | query)[enum: EnumType]");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(error_message, "");
    ASSERT_EQ(result, true);
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
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command ()[enum: EnumType]");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(error_message, "");
    ASSERT_EQ(result, true);
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
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command (set)[enum: EnumType]");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(error_message, "");
    ASSERT_EQ(result, true);
    ASSERT_EQ(command_name, "command");
    ASSERT_EQ(parameters.size(), 1);
    ASSERT_EQ(parameters[0].name, "enum");
    ASSERT_EQ(parameters[0].type, "EnumType");
    ASSERT_EQ(parameters[0].values, std::vector<std::string>({"set"}));
    ASSERT_EQ(parameters[0].optional, true);
    ASSERT_EQ(parameters[0].is_enum, true);
}

TEST_F(ParserTest, ParseCommandWithError)
{
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/some_command <param1 int>");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(result, false);
    ASSERT_EQ(error_message, "Syntax Error: expect ':', got 'int' at position 25.");
}

TEST_F(ParserTest, ParseCommandWithUnfinishedEnum)
{
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command (add|set|)<enum : EnumType>");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(result, false);
    ASSERT_EQ(error_message, "Syntax Error: expect 'enum value', got ')' at position 19.");
}

TEST_F(ParserTest, ParseCommandWithoutEndingToken)
{
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command <text:string");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(result, false);
    ASSERT_EQ(error_message, "Syntax Error: expect '>', got '' at position 21.");
}

TEST_F(ParserTest, ParseCommandWithoutSlash)
{
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("command <text:string>");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(result, false);
    ASSERT_EQ(error_message, "Syntax Error: expect '/', got 'command' at position 7.");
}

TEST_F(ParserTest, ParseCommandWithUnexpectedBracket)
{
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command <text:string]");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(result, false);
    ASSERT_EQ(error_message, "Syntax Error: expect '>', got ']' at position 22.");
}

TEST_F(ParserTest, ParseCommandWithoutParameterType)
{
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command <text>");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(result, false);
    ASSERT_EQ(error_message, "Syntax Error: expect ':', got '>' at position 15.");
}

TEST_F(ParserTest, ParseCommandWithUnexpectedToken)
{
    std::string command_name;
    std::vector<CommandUsageParser::Parameter> parameters;
    std::string error_message;

    CommandUsageParser parser("/command <<text:string>");
    bool result = parser.parse(command_name, parameters, error_message);

    ASSERT_EQ(result, false);
    ASSERT_EQ(error_message, "Syntax Error: expect 'parameter name', got '<' at position 11.");
}
