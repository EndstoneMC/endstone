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

class CommandDispatcherTest : public ::testing::Test {
protected:
    static Literal literal(const std::string &name) { return Literal::literal(name); }

    static Required argument(const std::string &name) { return Required::argument(name, StringArgumentType::word()); }

    CommandDispatcher<int> subject_;
    int calls_ = 0;
    Command<int> command_ = [this](const CommandContext<int> &) {
        calls_++;
        return 42;
    };
    int source_ = 0;
};

TEST_F(CommandDispatcherTest, testCreateAndExecuteCommand)
{
    subject_.registerCommand(literal("foo").executes(command_));
    EXPECT_EQ(subject_.execute("foo", source_), 42);
    EXPECT_EQ(calls_, 1);
}

TEST_F(CommandDispatcherTest, testCreateAndExecuteOffsetCommand)
{
    subject_.registerCommand(literal("foo").executes(command_));
    StringReader reader("/foo");
    reader.setCursor(1);
    EXPECT_EQ(subject_.execute(reader, source_), 42);
    EXPECT_EQ(calls_, 1);
}

TEST_F(CommandDispatcherTest, testCreateAndMergeCommands)
{
    subject_.registerCommand(literal("base").then(literal("foo").executes(command_)));
    subject_.registerCommand(literal("base").then(literal("bar").executes(command_)));
    EXPECT_EQ(subject_.execute("base foo", source_), 42);
    EXPECT_EQ(subject_.execute("base bar", source_), 42);
    EXPECT_EQ(calls_, 2);
}

TEST_F(CommandDispatcherTest, testExecuteUnknownCommand)
{
    subject_.registerCommand(literal("bar"));
    subject_.registerCommand(literal("baz"));
    expectSyntaxError([&] { subject_.execute("foo", source_); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownCommand(), 0);
}

TEST_F(CommandDispatcherTest, testExecuteImpermissibleCommand)
{
    subject_.registerCommand(literal("foo").requires_([](const int &) { return false; }));
    expectSyntaxError([&] { subject_.execute("foo", source_); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownCommand(), 0);
}

TEST_F(CommandDispatcherTest, testExecuteEmptyCommand)
{
    subject_.registerCommand(literal(""));
    expectSyntaxError([&] { subject_.execute("", source_); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownCommand(), 0);
}

TEST_F(CommandDispatcherTest, testExecuteUnknownSubcommand)
{
    subject_.registerCommand(literal("foo").executes(command_));
    expectSyntaxError([&] { subject_.execute("foo bar", source_); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownArgument(), 4);
}

TEST_F(CommandDispatcherTest, testExecuteIncorrectLiteral)
{
    subject_.registerCommand(literal("foo").executes(command_).then(literal("bar")));
    expectSyntaxError([&] { subject_.execute("foo baz", source_); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownArgument(), 4);
}

TEST_F(CommandDispatcherTest, testExecuteAmbiguousIncorrectArgument)
{
    subject_.registerCommand(literal("foo").executes(command_).then(literal("bar")).then(literal("baz")));
    expectSyntaxError([&] { subject_.execute("foo unknown", source_); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownArgument(), 4);
}

TEST_F(CommandDispatcherTest, testExecuteSubcommand)
{
    int sub_calls = 0;
    const Command<int> sub_command = [&](const CommandContext<int> &) {
        sub_calls++;
        return 100;
    };
    subject_.registerCommand(literal("foo")
                                 .then(literal("a"))
                                 .then(literal("=").executes(sub_command))
                                 .then(literal("c"))
                                 .executes(command_));
    EXPECT_EQ(subject_.execute("foo =", source_), 100);
    EXPECT_EQ(sub_calls, 1);
}

TEST_F(CommandDispatcherTest, testParseIncompleteLiteral)
{
    subject_.registerCommand(literal("foo").then(literal("bar").executes(command_)));
    const ParseResults<int> parse = subject_.parse("foo ", source_);
    EXPECT_EQ(parse.getReader().getRemaining(), " ");
    EXPECT_EQ(parse.getContext().getNodes().size(), 1);
}

TEST_F(CommandDispatcherTest, testParseIncompleteArgument)
{
    subject_.registerCommand(literal("foo").then(argument("bar").executes(command_)));
    const ParseResults<int> parse = subject_.parse("foo ", source_);
    EXPECT_EQ(parse.getReader().getRemaining(), " ");
    EXPECT_EQ(parse.getContext().getNodes().size(), 1);
}

TEST_F(CommandDispatcherTest, testExecuteAmbiguiousParentSubcommand)
{
    int sub_calls = 0;
    const Command<int> sub_command = [&](const CommandContext<int> &) {
        sub_calls++;
        return 100;
    };
    subject_.registerCommand(literal("test")
                                 .then(argument("incorrect").executes(command_))
                                 .then(argument("right").then(argument("sub").executes(sub_command))));
    EXPECT_EQ(subject_.execute("test 1 2", source_), 100);
    EXPECT_EQ(sub_calls, 1);
    EXPECT_EQ(calls_, 0);
}

TEST_F(CommandDispatcherTest, testExecuteOrphanedSubcommand)
{
    subject_.registerCommand(literal("foo").then(argument("bar")).executes(command_));
    expectSyntaxError([&] { subject_.execute("foo 5", source_); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownCommand(), 5);
}

TEST_F(CommandDispatcherTest, testExecuteInvalidOther)
{
    int wrong_calls = 0;
    const Command<int> wrong_command = [&](const CommandContext<int> &) {
        wrong_calls++;
        return 0;
    };
    subject_.registerCommand(literal("w").executes(wrong_command));
    subject_.registerCommand(literal("world").executes(command_));
    EXPECT_EQ(subject_.execute("world", source_), 42);
    EXPECT_EQ(wrong_calls, 0);
    EXPECT_EQ(calls_, 1);
}

TEST_F(CommandDispatcherTest, parseNoSpaceSeparator)
{
    subject_.registerCommand(literal("foo").then(argument("bar").executes(command_)));
    expectSyntaxError([&] { subject_.execute("foo$", source_); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownCommand(), 0);
}

TEST_F(CommandDispatcherTest, testArgumentsReachTheCommand)
{
    std::string seen;
    subject_.registerCommand(literal("say").then(
        Required::argument("text", StringArgumentType::greedyString()).executes([&](const CommandContext<int> &c) {
            seen = StringArgumentType::getString(c, "text");
            return SINGLE_SUCCESS;
        })));
    EXPECT_EQ(subject_.execute("say hello big world", source_), SINGLE_SUCCESS);
    EXPECT_EQ(seen, "hello big world");
}

TEST_F(CommandDispatcherTest, testGetPath)
{
    const auto bar = literal("bar").build();
    subject_.registerCommand(literal("foo").then(bar));
    EXPECT_EQ(subject_.getPath(*bar), (std::vector<std::string>{"foo", "bar"}));
}

TEST_F(CommandDispatcherTest, testFindNodeExists)
{
    const auto bar = literal("bar").build();
    subject_.registerCommand(literal("foo").then(bar));
    EXPECT_EQ(subject_.findNode({"foo", "bar"}), bar);
}

TEST_F(CommandDispatcherTest, testFindNodeDoesntExist)
{
    EXPECT_EQ(subject_.findNode({"foo", "bar"}), nullptr);
}

}  // namespace
}  // namespace endstone::brigadier
