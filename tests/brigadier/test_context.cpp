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

class CommandContextTest : public ::testing::Test {
protected:
    CommandDispatcher<int> dispatcher_;
    std::shared_ptr<CommandNode<int>> root_node_ = std::make_shared<RootCommandNode<int>>();
    CommandContextBuilder<int> builder_{dispatcher_, 7, root_node_, 0};
};

TEST_F(CommandContextTest, testGetArgumentNonexistent)
{
    EXPECT_THROW((void)builder_.build("").getArgument<std::string>("foo"), std::invalid_argument);
}

TEST_F(CommandContextTest, testGetArgumentWrongType)
{
    const CommandContext<int> context = builder_.withArgument("foo", ParsedArgument(0, 1, 123)).build("123");
    EXPECT_THROW((void)context.getArgument<std::string>("foo"), std::invalid_argument);
}

TEST_F(CommandContextTest, testGetArgument)
{
    const CommandContext<int> context = builder_.withArgument("foo", ParsedArgument(0, 1, 123)).build("123");
    EXPECT_EQ(context.getArgument<int>("foo"), 123);
}

TEST_F(CommandContextTest, testSource)
{
    EXPECT_EQ(builder_.build("").getSource(), 7);
}

TEST_F(CommandContextTest, testRootNode)
{
    EXPECT_EQ(builder_.build("").getRootNode(), root_node_);
}

TEST(ParsedArgumentTest, testEquals)
{
    EXPECT_EQ(ParsedArgument(0, 3, std::string("bar")), ParsedArgument(0, 3, std::string("bar")));
    EXPECT_NE(ParsedArgument(0, 3, std::string("bar")), ParsedArgument(3, 6, std::string("baz")));
    EXPECT_NE(ParsedArgument(3, 6, std::string("baz")), ParsedArgument(6, 9, std::string("baz")));
}

TEST(ParsedArgumentTest, getRaw)
{
    const StringReader reader("0123456789");
    const ParsedArgument argument(2, 5, std::string());
    EXPECT_EQ(argument.getRange().get(reader), "234");
}

}  // namespace
}  // namespace endstone::brigadier
