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

#include "endstone/brigadier/exceptions.h"

namespace endstone::brigadier {
namespace {

TEST(DynamicCommandExceptionTypeTest, createWithContext)
{
    const DynamicCommandExceptionType type([](const std::string &name) { return "Hello, " + name + "!"; });
    StringReader reader("Foo bar");
    reader.setCursor(5);
    const CommandSyntaxException exception = type.createWithContext(reader, "World");
    EXPECT_EQ(&exception.getType(), &type);
    EXPECT_EQ(exception.getInput(), "Foo bar");
    EXPECT_EQ(exception.getCursor(), 5);
    EXPECT_EQ(exception.getRawMessage(), "Hello, World!");
}

TEST(Dynamic2CommandExceptionTypeTest, createWithContext)
{
    const Dynamic2CommandExceptionType type(
        [](const std::string &a, const std::string &b) { return "Hello, " + a + " and " + b + "!"; });
    StringReader reader("Foo bar");
    reader.setCursor(5);
    const CommandSyntaxException exception = type.createWithContext(reader, "World", "Moon");
    EXPECT_EQ(&exception.getType(), &type);
    EXPECT_EQ(exception.getInput(), "Foo bar");
    EXPECT_EQ(exception.getCursor(), 5);
    EXPECT_EQ(exception.getRawMessage(), "Hello, World and Moon!");
}

TEST(BuiltInExceptionsTest, RangeMessagesMatchBrigadier)
{
    const auto &exceptions = CommandSyntaxException::BUILT_IN_EXCEPTIONS;
    EXPECT_EQ(exceptions.integerTooLow().create("-5", "0").getRawMessage(),
              "Integer must not be less than 0, found -5");
    EXPECT_EQ(exceptions.integerTooHigh().create("5", "0").getRawMessage(), "Integer must not be more than 0, found 5");
    EXPECT_EQ(exceptions.longTooLow().create("-5", "0").getRawMessage(), "Long must not be less than 0, found -5");
    EXPECT_EQ(exceptions.longTooHigh().create("5", "0").getRawMessage(), "Long must not be more than 0, found 5");
    EXPECT_EQ(exceptions.floatTooLow().create("-5.0", "0.0").getRawMessage(),
              "Float must not be less than 0.0, found -5.0");
    EXPECT_EQ(exceptions.floatTooHigh().create("5.0", "0.0").getRawMessage(),
              "Float must not be more than 0.0, found 5.0");
    EXPECT_EQ(exceptions.doubleTooLow().create("-5.0", "0.0").getRawMessage(),
              "Double must not be less than 0.0, found -5.0");
    EXPECT_EQ(exceptions.doubleTooHigh().create("5.0", "0.0").getRawMessage(),
              "Double must not be more than 0.0, found 5.0");
}

TEST(SimpleCommandExceptionTypeTest, createWithContext)
{
    const SimpleCommandExceptionType type("error");
    StringReader reader("Foo bar");
    reader.setCursor(5);
    const CommandSyntaxException exception = type.createWithContext(reader);
    EXPECT_EQ(&exception.getType(), &type);
    EXPECT_EQ(exception.getInput(), "Foo bar");
    EXPECT_EQ(exception.getCursor(), 5);
}

TEST(SimpleCommandExceptionTypeTest, getContextNone)
{
    const CommandExceptionType type;
    const CommandSyntaxException exception(type, "error");
    EXPECT_FALSE(exception.getContext().has_value());
    EXPECT_STREQ(exception.what(), "error");
}

TEST(SimpleCommandExceptionTypeTest, getContextShort)
{
    const CommandExceptionType type;
    const CommandSyntaxException exception(type, "error", "Hello world!", 5);
    EXPECT_EQ(exception.getContext(), "Hello<--[HERE]");
    EXPECT_STREQ(exception.what(), "error at position 5: Hello<--[HERE]");
}

TEST(SimpleCommandExceptionTypeTest, getContextLong)
{
    const CommandExceptionType type;
    const CommandSyntaxException exception(type, "error", "Hello world! This has an error in it. Oh dear!", 20);
    EXPECT_EQ(exception.getContext(), "...d! This ha<--[HERE]");
}

}  // namespace
}  // namespace endstone::brigadier
