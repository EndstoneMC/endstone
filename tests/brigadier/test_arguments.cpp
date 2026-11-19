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

#include "endstone/brigadier/arguments.h"

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

}  // namespace
}  // namespace endstone::brigadier
