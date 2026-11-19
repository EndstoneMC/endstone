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
using UsageMap = std::vector<std::pair<std::shared_ptr<CommandNode<int>>, std::string>>;

class CommandDispatcherUsagesTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        const auto never = [](const int &) {
            return false;
        };
        subject_.registerCommand(
            literal("a")
                .then(literal("1").then(literal("i").executes(command_)).then(literal("ii").executes(command_)))
                .then(literal("2").then(literal("i").executes(command_)).then(literal("ii").executes(command_))));
        subject_.registerCommand(literal("b").then(literal("1").executes(command_)));
        subject_.registerCommand(literal("c").executes(command_));
        subject_.registerCommand(literal("d").requires_(never).executes(command_));
        subject_.registerCommand(literal("e").executes(command_).then(literal("1")
                                                                          .executes(command_)
                                                                          .then(literal("i").executes(command_))
                                                                          .then(literal("ii").executes(command_))));
        subject_.registerCommand(literal("f")
                                     .then(literal("1")
                                               .then(literal("i").executes(command_))
                                               .then(literal("ii").executes(command_).requires_(never)))
                                     .then(literal("2")
                                               .then(literal("i").executes(command_).requires_(never))
                                               .then(literal("ii").executes(command_))));
        subject_.registerCommand(
            literal("g").executes(command_).then(literal("1").then(literal("i").executes(command_))));
        subject_.registerCommand(literal("h")
                                     .executes(command_)
                                     .then(literal("1").then(literal("i").executes(command_)))
                                     .then(literal("2").then(literal("i").then(literal("ii").executes(command_))))
                                     .then(literal("3").executes(command_)));
        subject_.registerCommand(literal("i")
                                     .executes(command_)
                                     .then(literal("1").executes(command_))
                                     .then(literal("2").executes(command_)));
        subject_.registerCommand(literal("j").redirect(subject_.getRoot()));
        subject_.registerCommand(literal("k").redirect(get("h")));
    }

    static Literal literal(const std::string &name) { return Literal::literal(name); }

    std::shared_ptr<CommandNode<int>> get(const std::string &command)
    {
        return subject_.parse(command, source_).getContext().getNodes().back().getNode();
    }

    void expectUsage(const UsageMap &results, const std::vector<std::pair<std::string, std::string>> &expected)
    {
        ASSERT_EQ(results.size(), expected.size());
        for (std::size_t i = 0; i < expected.size(); i++) {
            EXPECT_EQ(results[i].first, get(expected[i].first));
            EXPECT_EQ(results[i].second, expected[i].second);
        }
    }

    CommandDispatcher<int> subject_;
    Command<int> command_ = [](const CommandContext<int> &) {
        return 1;
    };
    int source_ = 0;
};

TEST_F(CommandDispatcherUsagesTest, testAllUsageNoCommands)
{
    const CommandDispatcher<int> empty;
    EXPECT_TRUE(empty.getAllUsage(*empty.getRoot(), source_, true).empty());
}

TEST_F(CommandDispatcherUsagesTest, testSmartUsageNoCommands)
{
    const CommandDispatcher<int> empty;
    EXPECT_TRUE(empty.getSmartUsage(*empty.getRoot(), source_).empty());
}

TEST_F(CommandDispatcherUsagesTest, testAllUsageRoot)
{
    EXPECT_EQ(
        subject_.getAllUsage(*subject_.getRoot(), source_, true),
        (std::vector<std::string>{
            "a 1 i", "a 1 ii", "a 2 i", "a 2 ii", "b 1",      "c",   "e", "e 1", "e 1 i", "e 1 ii", "f 1 i",  "f 2 ii",
            "g",     "g 1 i",  "h",     "h 1 i",  "h 2 i ii", "h 3", "i", "i 1", "i 2",   "j ...",  "k -> h",
        }));
}

TEST_F(CommandDispatcherUsagesTest, testSmartUsageRoot)
{
    expectUsage(subject_.getSmartUsage(*subject_.getRoot(), source_), {
                                                                          {"a", "a (1|2)"},
                                                                          {"b", "b 1"},
                                                                          {"c", "c"},
                                                                          {"e", "e [1]"},
                                                                          {"f", "f (1|2)"},
                                                                          {"g", "g [1]"},
                                                                          {"h", "h [1|2|3]"},
                                                                          {"i", "i [1|2]"},
                                                                          {"j", "j ..."},
                                                                          {"k", "k -> h"},
                                                                      });
}

TEST_F(CommandDispatcherUsagesTest, testSmartUsageH)
{
    expectUsage(subject_.getSmartUsage(*get("h"), source_), {
                                                                {"h 1", "[1] i"},
                                                                {"h 2", "[2] i ii"},
                                                                {"h 3", "[3]"},
                                                            });
}

TEST_F(CommandDispatcherUsagesTest, testSmartUsageOffsetH)
{
    StringReader offset_h("/|/|/h");
    offset_h.setCursor(5);
    const auto h = subject_.parse(offset_h, source_).getContext().getNodes().back().getNode();
    expectUsage(subject_.getSmartUsage(*h, source_), {
                                                         {"h 1", "[1] i"},
                                                         {"h 2", "[2] i ii"},
                                                         {"h 3", "[3]"},
                                                     });
}

}  // namespace
}  // namespace endstone::brigadier
