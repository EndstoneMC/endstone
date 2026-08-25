// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/core/spdlog/text_formatter.h"

#include <string>
#include <string_view>

#include <gtest/gtest.h>

namespace endstone::core::test {

namespace {

std::string format(std::string_view payload, bool should_do_colors)
{
    TextFormatter formatter{should_do_colors};
    const spdlog::details::log_msg msg{"test", spdlog::level::info, {payload.data(), payload.size()}};
    spdlog::memory_buf_t dest;
    formatter.format(msg, std::tm{}, dest);
    return {dest.data(), dest.size()};
}

}  // namespace

class TextFormatterTest : public ::testing::Test {};

TEST_F(TextFormatterTest, PlainTextIsUntouched)
{
    ASSERT_EQ("hello world", format("hello world", true));
    ASSERT_EQ("hello world", format("hello world", false));
}

TEST_F(TextFormatterTest, ColorCodeBecomesAnsi)
{
    ASSERT_EQ("\x1b[92mgreen", format("\u00a7agreen", true));
    ASSERT_EQ("green", format("\u00a7agreen", false));
}

TEST_F(TextFormatterTest, AnsiColorIsKeptWithColors)
{
    ASSERT_EQ("\x1b[32mPASS\x1b[0m ok", format("\x1b[32mPASS\x1b[0m ok", true));
    ASSERT_EQ("\x1b[38;2;221;214;5mgold", format("\x1b[38;2;221;214;5mgold", true));
}

TEST_F(TextFormatterTest, AnsiColorIsStrippedWithoutColors)
{
    ASSERT_EQ("PASS ok", format("\x1b[32mPASS\x1b[0m ok", false));
    ASSERT_EQ("gold", format("\x1b[38;2;221;214;5mgold", false));
}

TEST_F(TextFormatterTest, NonColorEscapeIsAlwaysDropped)
{
    ASSERT_EQ("foo", format("\x1b[2Jfoo", true));
    ASSERT_EQ("foo", format("\x1b[2Jfoo", false));
    ASSERT_EQ("foo", format("\x1b[1;2Hfoo", true));
}

TEST_F(TextFormatterTest, IncompleteEscapeIsDropped)
{
    ASSERT_EQ("abc", format("abc\x1b", true));
    ASSERT_EQ("abc", format("abc\x1b[", true));
    ASSERT_EQ("", format("\x1b[32", true));
    ASSERT_EQ("Z", format("\x1b"
                          "Z",
                          true));
}

}  // namespace endstone::core::test
