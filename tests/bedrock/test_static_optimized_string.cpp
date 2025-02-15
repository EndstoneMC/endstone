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

#include "bedrock/core/string/static_optimized_string.h"

namespace Bedrock {

TEST(StaticOptimizedStringTest, DefaultConstructor)
{
    StaticOptimizedString s;
    EXPECT_EQ(s.get(), nullptr);
    EXPECT_EQ(s.length(), 0);
}

TEST(StaticOptimizedStringTest, SmallStringInitialization)
{
    StaticOptimizedString s("Hello");
    EXPECT_STREQ(s.get(), "Hello");
    EXPECT_EQ(s.length(), 5);
    EXPECT_FALSE(s.view().empty());
    EXPECT_EQ(s.view(), "Hello");
}

TEST(StaticOptimizedStringTest, SmallStringViewInitialization)
{
    std::string small_string(8, 'A');
    StaticOptimizedString s(small_string, StaticOptimizedString::StorageType::Static);
    EXPECT_STREQ(s.get(), small_string.c_str());
    EXPECT_EQ(s.length(), small_string.length());
}

TEST(StaticOptimizedStringTest, LargeStringViewInitialization)
{
    std::string large_string(StaticOptimizedString::MAX_STATIC_STRING_LENGTH + 1, 'A');
    StaticOptimizedString s(large_string, StaticOptimizedString::StorageType::Dynamic);
    EXPECT_STREQ(s.get(), large_string.c_str());
    EXPECT_EQ(s.length(), large_string.length());
}

TEST(StaticOptimizedStringTest, CopyConstructor)
{
    StaticOptimizedString original("Copy Test");
    StaticOptimizedString copy(original);
    EXPECT_STREQ(copy.get(), "Copy Test");
    EXPECT_EQ(copy.length(), original.length());
}

TEST(StaticOptimizedStringTest, MoveConstructor)
{
    StaticOptimizedString original("Move Test");
    StaticOptimizedString moved(std::move(original));
    EXPECT_STREQ(moved.get(), "Move Test");
    EXPECT_EQ(moved.length(), 9);
    EXPECT_EQ(original.get(), nullptr);
    EXPECT_EQ(original.length(), 0);
}

TEST(StaticOptimizedStringTest, CopyAssignment)
{
    StaticOptimizedString original("Assignment Test");
    StaticOptimizedString assigned;
    assigned = original;
    EXPECT_STREQ(assigned.get(), "Assignment Test");
    EXPECT_EQ(assigned.length(), original.length());
}

TEST(StaticOptimizedStringTest, MoveAssignment)
{
    StaticOptimizedString original("Move Assign Test");
    StaticOptimizedString assigned;
    assigned = std::move(original);
    EXPECT_STREQ(assigned.get(), "Move Assign Test");
    EXPECT_EQ(assigned.length(), 16);
    EXPECT_EQ(original.get(), nullptr);
    EXPECT_EQ(original.length(), 0);
}

TEST(StaticOptimizedStringTest, ResetFunction)
{
    StaticOptimizedString s("Reset Test");
    s.reset();
    EXPECT_EQ(s.get(), nullptr);
    EXPECT_EQ(s.length(), 0);
}

TEST(StaticOptimizedStringTest, StringViewFunction)
{
    StaticOptimizedString s("View Test");
    EXPECT_EQ(s.view(), "View Test");
}

TEST(StaticOptimizedStringTest, OperatorConversion)
{
    StaticOptimizedString s("Operator Test");
    const char *c_str = s;
    EXPECT_STREQ(c_str, "Operator Test");
}

}  // namespace Bedrock
