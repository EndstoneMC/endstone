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

#include <limits>

#include <gtest/gtest.h>

#include "endstone/brigadier/arguments.h"
#include "support.h"

namespace endstone::brigadier {
namespace {

TEST(StringArgumentTypeTest, testParseWord)
{
    StringReader reader("hello world");
    EXPECT_EQ(std::get<std::string>(StringArgumentType::word()->parse(reader)), "hello");
    EXPECT_EQ(reader.getRead(), "hello");
}

TEST(StringArgumentTypeTest, testParseString)
{
    StringReader reader("\"hello world\" rest");
    EXPECT_EQ(std::get<std::string>(StringArgumentType::string()->parse(reader)), "hello world");
    EXPECT_EQ(reader.getRemaining(), " rest");
}

TEST(StringArgumentTypeTest, testParseGreedyString)
{
    StringReader reader("Hello world! This is a test.");
    EXPECT_EQ(std::get<std::string>(StringArgumentType::greedyString()->parse(reader)), "Hello world! This is a test.");
    EXPECT_FALSE(reader.canRead());
}

TEST(StringArgumentTypeTest, testToString)
{
    EXPECT_EQ(StringArgumentType::string()->toString(), "string()");
}

TEST(StringArgumentTypeTest, testEscapeIfRequiredNotRequired)
{
    EXPECT_EQ(StringArgumentType::escapeIfRequired("hello"), "hello");
    EXPECT_EQ(StringArgumentType::escapeIfRequired(""), "");
}

TEST(StringArgumentTypeTest, testEscapeIfRequiredMultipleWords)
{
    EXPECT_EQ(StringArgumentType::escapeIfRequired("hello world"), "\"hello world\"");
}

TEST(StringArgumentTypeTest, testEscapeIfRequiredQuote)
{
    EXPECT_EQ(StringArgumentType::escapeIfRequired("hello \"world\"!"), "\"hello \\\"world\\\"!\"");
}

TEST(StringArgumentTypeTest, testEscapeIfRequiredEscapes)
{
    EXPECT_EQ(StringArgumentType::escapeIfRequired("\\"), "\"\\\\\"");
}

TEST(StringArgumentTypeTest, testEscapeIfRequiredSingleQuote)
{
    EXPECT_EQ(StringArgumentType::escapeIfRequired("\""), "\"\\\"\"");
}

TEST(BoolArgumentTypeTest, parse)
{
    StringReader reader("true");
    EXPECT_EQ(std::get<bool>(BoolArgumentType::bool_()->parse(reader)), true);
    EXPECT_FALSE(reader.canRead());
}

TEST(IntegerArgumentTypeTest, parse)
{
    StringReader reader("15");
    EXPECT_EQ(std::get<int>(IntegerArgumentType::integer()->parse(reader)), 15);
    EXPECT_FALSE(reader.canRead());
}

TEST(IntegerArgumentTypeTest, parse_tooSmall)
{
    StringReader reader("-5");
    support::expectSyntaxError([&] { static_cast<void>(IntegerArgumentType::integer(0, 100)->parse(reader)); },
                               CommandSyntaxException::BUILT_IN_EXCEPTIONS.integerTooLow(), 0);
}

TEST(IntegerArgumentTypeTest, parse_tooBig)
{
    StringReader reader("5");
    support::expectSyntaxError([&] { static_cast<void>(IntegerArgumentType::integer(-100, 0)->parse(reader)); },
                               CommandSyntaxException::BUILT_IN_EXCEPTIONS.integerTooHigh(), 0);
}

TEST(IntegerArgumentTypeTest, testEquals)
{
    EXPECT_EQ(*IntegerArgumentType::integer(), *IntegerArgumentType::integer());
    EXPECT_EQ(*IntegerArgumentType::integer(-100, 100), *IntegerArgumentType::integer(-100, 100));
    EXPECT_EQ(*IntegerArgumentType::integer(-100, 50), *IntegerArgumentType::integer(-100, 50));
    EXPECT_EQ(*IntegerArgumentType::integer(-50, 100), *IntegerArgumentType::integer(-50, 100));
    EXPECT_FALSE(*IntegerArgumentType::integer(-100, 100) == *IntegerArgumentType::integer(-100, 50));
    EXPECT_FALSE(*IntegerArgumentType::integer(-100, 100) == *IntegerArgumentType::integer(-50, 100));
}

TEST(IntegerArgumentTypeTest, testToString)
{
    EXPECT_EQ(IntegerArgumentType::integer()->toString(), "integer()");
    EXPECT_EQ(IntegerArgumentType::integer(-100)->toString(), "integer(-100)");
    EXPECT_EQ(IntegerArgumentType::integer(-100, 100)->toString(), "integer(-100, 100)");
    EXPECT_EQ(IntegerArgumentType::integer(std::numeric_limits<int>::min(), 100)->toString(),
              "integer(-2147483648, 100)");
}

TEST(FloatArgumentTypeTest, parse)
{
    StringReader reader("15");
    EXPECT_EQ(std::get<float>(FloatArgumentType::floatArg()->parse(reader)), 15.0F);
    EXPECT_FALSE(reader.canRead());
}

TEST(FloatArgumentTypeTest, parse_tooSmall)
{
    StringReader reader("-5");
    support::expectSyntaxError([&] { static_cast<void>(FloatArgumentType::floatArg(0, 100)->parse(reader)); },
                               CommandSyntaxException::BUILT_IN_EXCEPTIONS.floatTooLow(), 0);
}

TEST(FloatArgumentTypeTest, parse_tooBig)
{
    StringReader reader("5");
    support::expectSyntaxError([&] { static_cast<void>(FloatArgumentType::floatArg(-100, 0)->parse(reader)); },
                               CommandSyntaxException::BUILT_IN_EXCEPTIONS.floatTooHigh(), 0);
}

TEST(FloatArgumentTypeTest, testEquals)
{
    EXPECT_EQ(*FloatArgumentType::floatArg(), *FloatArgumentType::floatArg());
    EXPECT_EQ(*FloatArgumentType::floatArg(-100, 100), *FloatArgumentType::floatArg(-100, 100));
    EXPECT_EQ(*FloatArgumentType::floatArg(-100, 50), *FloatArgumentType::floatArg(-100, 50));
    EXPECT_EQ(*FloatArgumentType::floatArg(-50, 100), *FloatArgumentType::floatArg(-50, 100));
    EXPECT_FALSE(*FloatArgumentType::floatArg(-100, 100) == *FloatArgumentType::floatArg(-100, 50));
    EXPECT_FALSE(*FloatArgumentType::floatArg(-100, 100) == *FloatArgumentType::floatArg(-50, 100));
}

TEST(FloatArgumentTypeTest, testToString)
{
    EXPECT_EQ(FloatArgumentType::floatArg()->toString(), "float()");
    EXPECT_EQ(FloatArgumentType::floatArg(-100)->toString(), "float(-100.0)");
    EXPECT_EQ(FloatArgumentType::floatArg(-100, 100)->toString(), "float(-100.0, 100.0)");
    EXPECT_EQ(FloatArgumentType::floatArg(static_cast<float>(std::numeric_limits<int>::min()), 100)->toString(),
              "float(-2.1474836E9, 100.0)");
}

TEST(DoubleArgumentTypeTest, parse)
{
    StringReader reader("15");
    EXPECT_EQ(std::get<double>(DoubleArgumentType::doubleArg()->parse(reader)), 15.0);
    EXPECT_FALSE(reader.canRead());
}

TEST(DoubleArgumentTypeTest, parse_tooSmall)
{
    StringReader reader("-5");
    support::expectSyntaxError([&] { static_cast<void>(DoubleArgumentType::doubleArg(0, 100)->parse(reader)); },
                               CommandSyntaxException::BUILT_IN_EXCEPTIONS.doubleTooLow(), 0);
}

TEST(DoubleArgumentTypeTest, parse_tooBig)
{
    StringReader reader("5");
    support::expectSyntaxError([&] { static_cast<void>(DoubleArgumentType::doubleArg(-100, 0)->parse(reader)); },
                               CommandSyntaxException::BUILT_IN_EXCEPTIONS.doubleTooHigh(), 0);
}

TEST(DoubleArgumentTypeTest, testEquals)
{
    EXPECT_EQ(*DoubleArgumentType::doubleArg(), *DoubleArgumentType::doubleArg());
    EXPECT_EQ(*DoubleArgumentType::doubleArg(-100, 100), *DoubleArgumentType::doubleArg(-100, 100));
    EXPECT_EQ(*DoubleArgumentType::doubleArg(-100, 50), *DoubleArgumentType::doubleArg(-100, 50));
    EXPECT_EQ(*DoubleArgumentType::doubleArg(-50, 100), *DoubleArgumentType::doubleArg(-50, 100));
    EXPECT_FALSE(*DoubleArgumentType::doubleArg(-100, 100) == *DoubleArgumentType::doubleArg(-100, 50));
    EXPECT_FALSE(*DoubleArgumentType::doubleArg(-100, 100) == *DoubleArgumentType::doubleArg(-50, 100));
}

TEST(DoubleArgumentTypeTest, testToString)
{
    EXPECT_EQ(DoubleArgumentType::doubleArg()->toString(), "double()");
    EXPECT_EQ(DoubleArgumentType::doubleArg(-100)->toString(), "double(-100.0)");
    EXPECT_EQ(DoubleArgumentType::doubleArg(-100, 100)->toString(), "double(-100.0, 100.0)");
    EXPECT_EQ(DoubleArgumentType::doubleArg(std::numeric_limits<int>::min(), 100)->toString(),
              "double(-2.147483648E9, 100.0)");
}

TEST(DoubleArgumentTypeTest, ReportsTheRangeInJavaNotation)
{
    StringReader reader("0.0001");
    try {
        static_cast<void>(DoubleArgumentType::doubleArg(1.5)->parse(reader));
        ADD_FAILURE() << "expected a CommandSyntaxException";
    }
    catch (const CommandSyntaxException &e) {
        EXPECT_EQ(e.getRawMessage(), "Double must not be less than 1.5, found 1.0E-4");
    }
}

}  // namespace
}  // namespace endstone::brigadier
