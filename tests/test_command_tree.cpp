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

#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "endstone/command/arguments.h"
#include "endstone/command/command_builder.h"
#include "endstone/core/command/tree/command_tree.h"

using endstone::Arguments;
using endstone::CommandContext;
using endstone::Commands;
using endstone::core::CommandTreeOverload;
using endstone::core::flattenCommandTree;
using endstone::core::MAX_OVERLOADS_PER_COMMAND;

namespace {

void noop(const CommandContext &) {}

std::vector<std::string> shapeOf(const CommandTreeOverload &overload)
{
    std::vector<std::string> shape;
    for (const auto &slot : overload.slots) {
        shape.push_back(slot.node->getName());
    }
    return shape;
}

std::vector<std::string> pathOf(const CommandTreeOverload &overload)
{
    std::vector<std::string> path;
    for (const auto &node : overload.path) {
        path.push_back(node->getName());
    }
    return path;
}

}  // namespace

TEST(CommandTreeTest, RootOnlyYieldsOneEmptyOverload)
{
    const auto root = Commands::literal("ping").executes(noop).build();

    const auto result = flattenCommandTree(root);
    ASSERT_TRUE(result.has_value()) << result.error();
    ASSERT_EQ(result->size(), 1);
    EXPECT_THAT(shapeOf(result->front()), testing::IsEmpty());
    EXPECT_EQ(result->front().leaf->getName(), "ping");
}

TEST(CommandTreeTest, EachExecutableNodeBecomesAnOverload)
{
    const auto root =
        Commands::literal("warp")
            .then(Commands::literal("add").then(Commands::argument("name", Arguments::string()).executes(noop)))
            .then(Commands::literal("list").executes(noop))
            .build();

    const auto result = flattenCommandTree(root);
    ASSERT_TRUE(result.has_value()) << result.error();
    ASSERT_EQ(result->size(), 2);
    EXPECT_THAT(shapeOf(result->at(0)), testing::ElementsAre("add", "name"));
    EXPECT_THAT(shapeOf(result->at(1)), testing::ElementsAre("list"));
}

TEST(CommandTreeTest, ExecutableParentAndChildBothYieldOverloads)
{
    const auto root = Commands::literal("home")
                          .executes(noop)
                          .then(Commands::argument("name", Arguments::string()).executes(noop))
                          .build();

    const auto result = flattenCommandTree(root);
    ASSERT_TRUE(result.has_value()) << result.error();
    ASSERT_EQ(result->size(), 2);
    EXPECT_THAT(shapeOf(result->at(0)), testing::IsEmpty());
    EXPECT_THAT(shapeOf(result->at(1)), testing::ElementsAre("name"));
}

TEST(CommandTreeTest, PathRunsFromRootToLeaf)
{
    const auto root =
        Commands::literal("warp")
            .then(Commands::literal("add").then(Commands::argument("name", Arguments::string()).executes(noop)))
            .build();

    const auto result = flattenCommandTree(root);
    ASSERT_TRUE(result.has_value()) << result.error();
    ASSERT_EQ(result->size(), 1);
    EXPECT_THAT(pathOf(result->front()), testing::ElementsAre("warp", "add", "name"));
}

TEST(CommandTreeTest, RejectsTreeWithNoExecutableNode)
{
    const auto root = Commands::literal("warp").then(Commands::literal("add")).build();

    const auto result = flattenCommandTree(root);
    ASSERT_FALSE(result.has_value());
    EXPECT_THAT(result.error(), testing::HasSubstr("no branch that does anything"));
}

TEST(CommandTreeTest, RejectsArgumentAfterGreedyMessage)
{
    const auto root = Commands::literal("say")
                          .then(Commands::argument("text", Arguments::message())
                                    .then(Commands::argument("extra", Arguments::string()).executes(noop)))
                          .build();

    const auto result = flattenCommandTree(root);
    ASSERT_FALSE(result.has_value());
    EXPECT_THAT(result.error(), testing::HasSubstr("must be the last one"));
}

TEST(CommandTreeTest, RejectsArgumentAfterGreedyRawText)
{
    const auto root = Commands::literal("say")
                          .then(Commands::argument("text", Arguments::rawText())
                                    .then(Commands::argument("extra", Arguments::string()).executes(noop)))
                          .build();

    const auto result = flattenCommandTree(root);
    ASSERT_FALSE(result.has_value());
    EXPECT_THAT(result.error(), testing::HasSubstr("must be the last one"));
}

TEST(CommandTreeTest, AllowsGreedyMessageAsTheLastArgument)
{
    const auto root =
        Commands::literal("say").then(Commands::argument("text", Arguments::message()).executes(noop)).build();

    const auto result = flattenCommandTree(root);
    ASSERT_TRUE(result.has_value()) << result.error();
    EXPECT_THAT(shapeOf(result->front()), testing::ElementsAre("text"));
}

TEST(CommandTreeTest, RejectsCycle)
{
    const auto root = Commands::literal("loop").build();
    const auto child = Commands::literal("again").executes(noop).build();
    child->addChild(root);
    root->addChild(child);

    const auto result = flattenCommandTree(root);
    ASSERT_FALSE(result.has_value());
    EXPECT_THAT(result.error(), testing::HasSubstr("reachable from itself"));
}

TEST(CommandTreeTest, RejectsMoreOverloadsThanTheBudget)
{
    auto root = Commands::literal("wide");
    for (std::size_t i = 0; i <= MAX_OVERLOADS_PER_COMMAND; ++i) {
        root = root.then(Commands::literal("branch" + std::to_string(i)).executes(noop));
    }

    const auto result = flattenCommandTree(root.build());
    ASSERT_FALSE(result.has_value());
    EXPECT_THAT(result.error(), testing::HasSubstr("Too many branches"));
}

TEST(CommandTreeTest, MergesChildrenWithTheSameName)
{
    const auto root =
        Commands::literal("warp")
            .then(Commands::literal("add").then(Commands::argument("name", Arguments::string()).executes(noop)))
            .then(Commands::literal("add").then(Commands::argument("other", Arguments::string()).executes(noop)))
            .build();

    ASSERT_EQ(root->getChildren().size(), 1);
    const auto result = flattenCommandTree(root);
    ASSERT_TRUE(result.has_value()) << result.error();
    ASSERT_EQ(result->size(), 2);
    EXPECT_THAT(shapeOf(result->at(0)), testing::ElementsAre("add", "name"));
    EXPECT_THAT(shapeOf(result->at(1)), testing::ElementsAre("add", "other"));
}

TEST(CommandTreeTest, MergeCombinesRequirementsRatherThanKeepingTheFirst)
{
    const auto first = Commands::literal("run").permission("a").executes(noop).build();
    const auto second = Commands::literal("run").permission("b").executes(noop).build();
    const auto root = Commands::literal("warp").then(first).then(second).build();

    ASSERT_EQ(root->getChildren().size(), 1);
    EXPECT_THAT(root->getChildren().front()->getPermissions(), testing::ElementsAre("a", "b"));
}
