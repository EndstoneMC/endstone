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
