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
#include "support.h"

namespace endstone::brigadier {
namespace {

using support::expectSyntaxError;

using Literal = LiteralArgumentBuilder<int>;
using Required = RequiredArgumentBuilder<int>;

class CommandNodeTest : public ::testing::TestWithParam<int> {
protected:
    [[nodiscard]] static std::shared_ptr<CommandNode<int>> makeNode()
    {
        switch (GetParam()) {
        case 0:
            return Required::argument("foo", StringArgumentType::word()).build();
        case 1:
            return Literal::literal("foo").build();
        default:
            return std::make_shared<RootCommandNode<int>>();
        }
    }

    Command<int> command_ = [](const CommandContext<int> &) {
        return 1;
    };
};

TEST_P(CommandNodeTest, testAddChild)
{
    const auto node = makeNode();
    node->addChild(Literal::literal("child1").build());
    node->addChild(Literal::literal("child2").build());
    node->addChild(Literal::literal("child1").build());
    EXPECT_EQ(node->getChildren().size(), 2);
}

TEST_P(CommandNodeTest, testAddChildMergesGrandchildren)
{
    const auto node = makeNode();
    node->addChild(Literal::literal("child").then(Literal::literal("grandchild1")).build());
    node->addChild(Literal::literal("child").then(Literal::literal("grandchild2")).build());
    ASSERT_EQ(node->getChildren().size(), 1);
    EXPECT_EQ(node->getChildren().front()->getChildren().size(), 2);
}

TEST_P(CommandNodeTest, testAddChildPreservesCommand)
{
    const auto node = makeNode();
    node->addChild(Literal::literal("child").executes(command_).build());
    node->addChild(Literal::literal("child").build());
    EXPECT_TRUE(node->getChildren().front()->getCommand());
}

TEST_P(CommandNodeTest, testAddChildOverwritesCommand)
{
    const auto node = makeNode();
    node->addChild(Literal::literal("child").build());
    node->addChild(Literal::literal("child").executes(command_).build());
    EXPECT_TRUE(node->getChildren().front()->getCommand());
}

INSTANTIATE_TEST_SUITE_P(Nodes, CommandNodeTest, ::testing::Values(0, 1, 2));

class NodeParseTest : public ::testing::Test {
protected:
    CommandDispatcher<int> dispatcher_;
    CommandContextBuilder<int> context_builder_{dispatcher_, 0, std::make_shared<RootCommandNode<int>>(), 0};
};

TEST_F(NodeParseTest, argumentParse)
{
    const auto node = Required::argument("foo", StringArgumentType::word()).build();
    StringReader reader("abc def");
    node->parse(reader, context_builder_);
    ASSERT_TRUE(context_builder_.getArguments().contains("foo"));
    EXPECT_EQ(context_builder_.getArguments().at("foo").getResult(), ArgumentValue(std::string("abc")));
}

TEST_F(NodeParseTest, argumentUsage)
{
    EXPECT_EQ(Required::argument("foo", StringArgumentType::word()).build()->getUsageText(), "<foo>");
}

TEST_F(NodeParseTest, argumentSuggestions)
{
    const auto node = Required::argument("foo", StringArgumentType::word()).build();
    SuggestionsBuilder builder("", 0);
    EXPECT_TRUE(node->listSuggestions(context_builder_.build(""), builder).isEmpty());
}

TEST_F(NodeParseTest, argumentCreateBuilder)
{
    const auto node = Required::argument("foo", StringArgumentType::word()).build();
    const Required builder = node->createBuilder();
    EXPECT_EQ(builder.getName(), node->getName());
    EXPECT_EQ(builder.getType(), node->getType());
}

TEST_F(NodeParseTest, literalParse)
{
    const auto node = Literal::literal("foo").build();
    StringReader reader("foo bar");
    node->parse(reader, context_builder_);
    EXPECT_EQ(reader.getRemaining(), " bar");
}

TEST_F(NodeParseTest, literalParseExact)
{
    const auto node = Literal::literal("foo").build();
    StringReader reader("foo");
    node->parse(reader, context_builder_);
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST_F(NodeParseTest, literalParseSimilar)
{
    const auto node = Literal::literal("foo").build();
    StringReader reader("foobar");
    expectSyntaxError([&] { node->parse(reader, context_builder_); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.literalIncorrect(), 0);
}

TEST_F(NodeParseTest, literalParseInvalid)
{
    const auto node = Literal::literal("foo").build();
    StringReader reader("bar");
    expectSyntaxError([&] { node->parse(reader, context_builder_); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.literalIncorrect(), 0);
}

TEST_F(NodeParseTest, literalUsage)
{
    EXPECT_EQ(Literal::literal("foo").build()->getUsageText(), "foo");
}

TEST_F(NodeParseTest, literalSuggestions)
{
    const auto node = Literal::literal("foo").build();
    SuggestionsBuilder empty_builder("", 0);
    EXPECT_EQ(node->listSuggestions(context_builder_.build(""), empty_builder).getList(),
              std::vector{Suggestion(StringRange::at(0), "foo")});
    SuggestionsBuilder foo_builder("foo", 0);
    EXPECT_TRUE(node->listSuggestions(context_builder_.build("foo"), foo_builder).isEmpty());
    SuggestionsBuilder food_builder("food", 0);
    EXPECT_TRUE(node->listSuggestions(context_builder_.build("food"), food_builder).isEmpty());
    SuggestionsBuilder b_builder("b", 0);
    EXPECT_TRUE(node->listSuggestions(context_builder_.build("b"), b_builder).isEmpty());
}

TEST_F(NodeParseTest, literalCreateBuilder)
{
    const auto node = Literal::literal("foo").build();
    const Literal builder = node->createBuilder();
    EXPECT_EQ(builder.getLiteral(), node->getLiteral());
}

TEST_F(NodeParseTest, rootParse)
{
    RootCommandNode<int> node;
    StringReader reader("hello world");
    node.parse(reader, context_builder_);
    EXPECT_EQ(reader.getCursor(), 0);
}

TEST_F(NodeParseTest, rootAddChildNoRoot)
{
    RootCommandNode<int> node;
    EXPECT_THROW(node.addChild(std::make_shared<RootCommandNode<int>>()), std::logic_error);
}

TEST_F(NodeParseTest, rootUsage)
{
    EXPECT_EQ(RootCommandNode<int>().getUsageText(), "");
}

TEST_F(NodeParseTest, rootSuggestions)
{
    RootCommandNode<int> node;
    SuggestionsBuilder builder("", 0);
    EXPECT_TRUE(node.listSuggestions(context_builder_.build(""), builder).isEmpty());
}

TEST(NarrowingTest, testNodeIsAndAs)
{
    const auto literal = Literal::literal("foo").build();
    const auto argument = Required::argument("bar", StringArgumentType::word()).build();
    const auto root = std::make_shared<RootCommandNode<int>>();

    EXPECT_TRUE(literal->is<LiteralCommandNode<int>>());
    EXPECT_FALSE(literal->is<ArgumentCommandNode<int>>());
    EXPECT_FALSE(literal->is<RootCommandNode<int>>());
    EXPECT_TRUE(argument->is<ArgumentCommandNode<int>>());
    EXPECT_FALSE(argument->is<LiteralCommandNode<int>>());
    EXPECT_TRUE(root->is<RootCommandNode<int>>());
    EXPECT_TRUE(root->is<CommandNode<int>>());

    EXPECT_EQ(literal->as<LiteralCommandNode<int>>(), literal.get());
    EXPECT_EQ(literal->as<ArgumentCommandNode<int>>(), nullptr);
    EXPECT_EQ(argument->as<ArgumentCommandNode<int>>(), argument.get());
}

TEST(NarrowingTest, testArgumentTypeIsAndAs)
{
    const std::shared_ptr<const ArgumentType> type = StringArgumentType::greedyString();

    EXPECT_TRUE(type->is<StringArgumentType>());
    EXPECT_TRUE(type->is<ArgumentType>());
    ASSERT_NE(type->as<StringArgumentType>(), nullptr);
    EXPECT_EQ(type->as<StringArgumentType>()->getType(), StringArgumentType::StringType::GreedyPhrase);
}

}  // namespace
}  // namespace endstone::brigadier
