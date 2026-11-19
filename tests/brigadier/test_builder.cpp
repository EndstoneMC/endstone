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

#include "endstone/brigadier/dispatcher.h"

namespace endstone::brigadier {
namespace {

using Literal = LiteralArgumentBuilder<int>;
using Required = RequiredArgumentBuilder<int>;

class TestableArgumentBuilder : public ArgumentBuilder<int, TestableArgumentBuilder> {};

TEST(ArgumentBuilderTest, testArguments)
{
    TestableArgumentBuilder builder;
    const Required argument = Required::argument("bar", StringArgumentType::word());
    builder.then(argument);
    ASSERT_EQ(builder.getArguments().size(), 1);
    EXPECT_EQ(builder.getArguments().front()->getName(), "bar");
}

class LiteralArgumentBuilderTest : public ::testing::Test {
protected:
    Literal builder_ = Literal::literal("foo");
    Command<int> command_ = [](const CommandContext<int> &) {
        return 1;
    };
};

TEST_F(LiteralArgumentBuilderTest, testBuild)
{
    EXPECT_EQ(builder_.build()->getLiteral(), "foo");
}

TEST_F(LiteralArgumentBuilderTest, testBuildWithExecutor)
{
    const auto node = builder_.executes(command_).build();
    EXPECT_EQ(node->getLiteral(), "foo");
    EXPECT_TRUE(node->getCommand());
}

TEST_F(LiteralArgumentBuilderTest, testBuildWithChildren)
{
    builder_.then(Required::argument("bar", StringArgumentType::word()));
    builder_.then(Required::argument("baz", StringArgumentType::word()));
    EXPECT_EQ(builder_.build()->getChildren().size(), 2);
}

class RequiredArgumentBuilderTest : public ::testing::Test {
protected:
    std::shared_ptr<const ArgumentType> type_ = StringArgumentType::word();
    Required builder_ = Required::argument("foo", type_);
    Command<int> command_ = [](const CommandContext<int> &) {
        return 1;
    };
};

TEST_F(RequiredArgumentBuilderTest, testBuild)
{
    const auto node = builder_.build();
    EXPECT_EQ(node->getName(), "foo");
    EXPECT_EQ(node->getType(), type_);
}

TEST_F(RequiredArgumentBuilderTest, testBuildWithExecutor)
{
    const auto node = builder_.executes(command_).build();
    EXPECT_EQ(node->getName(), "foo");
    EXPECT_EQ(node->getType(), type_);
    EXPECT_TRUE(node->getCommand());
}

TEST_F(RequiredArgumentBuilderTest, testBuildWithChildren)
{
    builder_.then(Required::argument("bar", StringArgumentType::word()));
    builder_.then(Required::argument("baz", StringArgumentType::word()));
    EXPECT_EQ(builder_.build()->getChildren().size(), 2);
}

}  // namespace
}  // namespace endstone::brigadier
