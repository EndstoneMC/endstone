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

Literal literal(const std::string &name)
{
    return Literal::literal(name);
}

int integerValue(const CommandContext<int> &context, const std::string &name)
{
    return std::stoi(context.getArgument<std::string>(name));
}

Required integerArgument(const std::string &name)
{
    return Required::argument(name, StringArgumentType::word());
}

struct Completion {
    Completion(int source, bool success, int result) : source(source), success(success), result(result) {}

    int source;
    bool success;
    int result;
    bool operator==(const Completion &) const = default;
};

class RedirectTest : public ::testing::Test {
protected:
    void recordCompletions()
    {
        subject_.setConsumer([this](const CommandContext<int> &context, bool success, int result) {
            completions_.emplace_back(context.getSource(), success, result);
        });
    }

    CommandDispatcher<int> subject_;
    std::vector<int> run_sources_;
    std::vector<Completion> completions_;
    int calls_ = 0;
    Command<int> command_ = [this](const CommandContext<int> &context) {
        calls_++;
        run_sources_.push_back(context.getSource());
        return 42;
    };
    int source_ = 0;
};

TEST_F(RedirectTest, testRedirect)
{
    const auto target = literal("target").build();
    auto builder = literal("foo");
    builder.redirect(target);
    EXPECT_EQ(builder.getRedirect(), target.get());
}

TEST_F(RedirectTest, testRedirectWithChild)
{
    const auto target = literal("target").build();
    auto builder = literal("foo");
    builder.then(literal("bar"));
    EXPECT_THROW(builder.redirect(target), std::logic_error);
}

TEST_F(RedirectTest, testThenWithRedirect)
{
    const auto target = literal("target").build();
    auto builder = literal("foo");
    builder.redirect(target);
    EXPECT_THROW(builder.then(literal("bar")), std::logic_error);
}

TEST_F(RedirectTest, testExecuteAmbiguousParentSubcommandViaRedirect)
{
    Command<int> sub_command = [](const CommandContext<int> &) {
        return 100;
    };
    const auto real = subject_.registerCommand(
        literal("test")
            .then(integerArgument("incorrect").executes(command_))
            .then(integerArgument("right").then(integerArgument("sub").executes(sub_command))));
    subject_.registerCommand(literal("redirect").redirect(real));

    EXPECT_EQ(subject_.execute("redirect 1 2", source_), 100);
    EXPECT_EQ(calls_, 0);
}

TEST_F(RedirectTest, testExecuteRedirectedMultipleTimes)
{
    const auto concrete_node = subject_.registerCommand(literal("actual").executes(command_));
    const auto redirect_node = subject_.registerCommand(literal("redirected").redirect(subject_.getRoot()));

    const std::string input = "redirected redirected actual";

    const ParseResults<int> parse = subject_.parse(input, source_);
    EXPECT_EQ(parse.getContext().getRange().get(input), "redirected");
    ASSERT_EQ(parse.getContext().getNodes().size(), 1U);
    EXPECT_EQ(parse.getContext().getRootNode(), subject_.getRoot());
    EXPECT_EQ(parse.getContext().getNodes()[0].getRange(), parse.getContext().getRange());
    EXPECT_EQ(parse.getContext().getNodes()[0].getNode(), redirect_node);

    const auto &child1 = parse.getContext().getChild();
    ASSERT_NE(child1, nullptr);
    EXPECT_EQ(child1->getRange().get(input), "redirected");
    ASSERT_EQ(child1->getNodes().size(), 1U);
    EXPECT_EQ(child1->getRootNode(), subject_.getRoot());
    EXPECT_EQ(child1->getNodes()[0].getRange(), child1->getRange());
    EXPECT_EQ(child1->getNodes()[0].getNode(), redirect_node);

    const auto &child2 = child1->getChild();
    ASSERT_NE(child2, nullptr);
    EXPECT_EQ(child2->getRange().get(input), "actual");
    ASSERT_EQ(child2->getNodes().size(), 1U);
    EXPECT_EQ(child2->getRootNode(), subject_.getRoot());
    EXPECT_EQ(child2->getNodes()[0].getRange(), child2->getRange());
    EXPECT_EQ(child2->getNodes()[0].getNode(), concrete_node);

    EXPECT_EQ(subject_.execute(parse), 42);
    EXPECT_EQ(calls_, 1);
}

TEST_F(RedirectTest, testCorrectExecuteContextAfterRedirect)
{
    CommandDispatcher<int> subject;
    const auto root = subject.getRoot();
    auto add = literal("add");
    auto blank = literal("blank");
    auto add_arg = integerArgument("value");
    auto run = literal("run");

    subject.registerCommand(add.then(add_arg.redirect(root, [](const CommandContext<int> &context) {
        return context.getSource() + integerValue(context, "value");
    })));
    subject.registerCommand(blank.redirect(root));
    subject.registerCommand(run.executes([](const CommandContext<int> &context) { return context.getSource(); }));

    EXPECT_EQ(subject.execute("run", 0), 0);
    EXPECT_EQ(subject.execute("run", 1), 1);

    EXPECT_EQ(subject.execute("add 5 run", 1), 1 + 5);
    EXPECT_EQ(subject.execute("add 5 add 6 run", 2), 2 + 5 + 6);
    EXPECT_EQ(subject.execute("add 5 blank run", 1), 1 + 5);
    EXPECT_EQ(subject.execute("blank add 5 run", 1), 1 + 5);
    EXPECT_EQ(subject.execute("add 5 blank add 6 run", 2), 2 + 5 + 6);
    EXPECT_EQ(subject.execute("add 5 blank blank add 6 run", 2), 2 + 5 + 6);
}

TEST_F(RedirectTest, testSharedRedirectAndExecuteNodes)
{
    CommandDispatcher<int> subject;
    const auto root = subject.getRoot();
    auto add = literal("add");
    auto add_arg = integerArgument("value");

    subject.registerCommand(
        add.then(add_arg
                     .redirect(root,
                               [](const CommandContext<int> &context) {
                                   return context.getSource() + integerValue(context, "value");
                               })
                     .executes([](const CommandContext<int> &context) { return context.getSource(); })));

    EXPECT_EQ(subject.execute("add 5", 1), 1);
    EXPECT_EQ(subject.execute("add 5 add 6", 1), 1 + 5);
}

TEST_F(RedirectTest, testExecuteRedirected)
{
    const RedirectModifier<int> modifier = [](const CommandContext<int> &) {
        return std::vector<int>{1, 2};
    };

    const auto concrete_node = subject_.registerCommand(literal("actual").executes(command_));
    const auto redirect_node = subject_.registerCommand(literal("redirected").fork(subject_.getRoot(), modifier));

    const std::string input = "redirected actual";
    const ParseResults<int> parse = subject_.parse(input, source_);
    EXPECT_EQ(parse.getContext().getRange().get(input), "redirected");
    ASSERT_EQ(parse.getContext().getNodes().size(), 1U);
    EXPECT_EQ(parse.getContext().getRootNode(), subject_.getRoot());
    EXPECT_EQ(parse.getContext().getNodes()[0].getRange(), parse.getContext().getRange());
    EXPECT_EQ(parse.getContext().getNodes()[0].getNode(), redirect_node);
    EXPECT_EQ(parse.getContext().getSource(), source_);

    const auto &parent = parse.getContext().getChild();
    ASSERT_NE(parent, nullptr);
    EXPECT_EQ(parent->getRange().get(input), "actual");
    ASSERT_EQ(parent->getNodes().size(), 1U);
    EXPECT_EQ(parent->getNodes()[0].getRange(), parent->getRange());
    EXPECT_EQ(parent->getNodes()[0].getNode(), concrete_node);
    EXPECT_EQ(parent->getSource(), source_);

    EXPECT_EQ(subject_.execute(parse), 2);
    EXPECT_EQ(run_sources_, (std::vector<int>{1, 2}));
}

TEST_F(RedirectTest, testIncompleteRedirectShouldThrow)
{
    const auto foo = subject_.registerCommand(
        literal("foo")
            .then(literal("bar").then(integerArgument("value").executes(
                [](const CommandContext<int> &context) { return integerValue(context, "value"); })))
            .then(literal("awa").executes([](const CommandContext<int> &) { return 2; })));
    subject_.registerCommand(literal("baz").redirect(foo));
    try {
        subject_.execute("baz bar", source_);
        FAIL() << "Should have thrown an exception";
    }
    catch (const CommandSyntaxException &ex) {
        EXPECT_EQ(&ex.getType(), &CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownCommand());
    }
}

TEST_F(RedirectTest, testRedirectModifierEmptyResult)
{
    const auto foo = subject_.registerCommand(
        literal("foo")
            .then(literal("bar").then(integerArgument("value").executes(
                [](const CommandContext<int> &context) { return integerValue(context, "value"); })))
            .then(literal("awa").executes([](const CommandContext<int> &) { return 2; })));
    const RedirectModifier<int> empty_modifier = [](const CommandContext<int> &) {
        return std::vector<int>{};
    };
    subject_.registerCommand(literal("baz").fork(foo, empty_modifier));
    EXPECT_EQ(subject_.execute("baz bar 100", source_), 0);
}

TEST_F(RedirectTest, testResultConsumerInForkedNonErrorRun)
{
    recordCompletions();
    subject_.registerCommand(
        literal("foo").executes([](const CommandContext<int> &context) { return context.getSource(); }));
    subject_.registerCommand(literal("repeat").fork(
        subject_.getRoot(), [](const CommandContext<int> &) { return std::vector<int>{9, 10, 11}; }));

    EXPECT_EQ(subject_.execute("repeat foo", source_), 3);
    EXPECT_EQ(completions_, (std::vector<Completion>{{9, true, 9}, {10, true, 10}, {11, true, 11}}));
}

TEST_F(RedirectTest, testExceptionInNonForkedRedirectedCommand)
{
    recordCompletions();
    subject_.registerCommand(literal("crash").executes([](const CommandContext<int> &) -> int {
        throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedBool().create();
    }));
    subject_.registerCommand(literal("redirect").redirect(subject_.getRoot()));

    try {
        subject_.execute("redirect crash", source_);
        FAIL();
    }
    catch (const CommandSyntaxException &ex) {
        EXPECT_EQ(&ex.getType(), &CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedBool());
    }
    EXPECT_EQ(completions_, (std::vector<Completion>{{0, false, 0}}));
}

TEST_F(RedirectTest, testExceptionInForkedRedirectedCommand)
{
    recordCompletions();
    subject_.registerCommand(literal("crash").executes([](const CommandContext<int> &) -> int {
        throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedBool().create();
    }));
    subject_.registerCommand(literal("redirect").fork(subject_.getRoot(), [](const CommandContext<int> &context) {
        return std::vector<int>{context.getSource()};
    }));

    EXPECT_EQ(subject_.execute("redirect crash", source_), 0);
    EXPECT_EQ(completions_, (std::vector<Completion>{{0, false, 0}}));
}

TEST_F(RedirectTest, testExceptionInNonForkedRedirect)
{
    recordCompletions();
    subject_.registerCommand(literal("noop").executes(command_));
    subject_.registerCommand(literal("redirect").redirect(subject_.getRoot(), [](const CommandContext<int> &) -> int {
        throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedBool().create();
    }));

    try {
        subject_.execute("redirect noop", source_);
        FAIL();
    }
    catch (const CommandSyntaxException &ex) {
        EXPECT_EQ(&ex.getType(), &CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedBool());
    }
    EXPECT_EQ(calls_, 0);
    EXPECT_EQ(completions_, (std::vector<Completion>{{0, false, 0}}));
}

TEST_F(RedirectTest, testExceptionInForkedRedirect)
{
    recordCompletions();
    subject_.registerCommand(literal("noop").executes(command_));
    subject_.registerCommand(
        literal("redirect").fork(subject_.getRoot(), [](const CommandContext<int> &) -> std::vector<int> {
            throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedBool().create();
        }));

    EXPECT_EQ(subject_.execute("redirect noop", source_), 0);
    EXPECT_EQ(calls_, 0);
    EXPECT_EQ(completions_, (std::vector<Completion>{{0, false, 0}}));
}

TEST_F(RedirectTest, testPartialExceptionInForkedRedirect)
{
    recordCompletions();
    constexpr int other_source = 7;
    constexpr int rejected_source = 8;
    subject_.registerCommand(literal("run").executes([this](const CommandContext<int> &context) {
        run_sources_.push_back(context.getSource());
        return 3;
    }));
    subject_.registerCommand(literal("split").fork(subject_.getRoot(), [](const CommandContext<int> &context) {
        return std::vector<int>{context.getSource(), rejected_source, other_source};
    }));
    subject_.registerCommand(literal("filter").fork(subject_.getRoot(), [](const CommandContext<int> &context) {
        if (context.getSource() == rejected_source) {
            throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedBool().create();
        }
        return std::vector<int>{context.getSource()};
    }));

    EXPECT_EQ(subject_.execute("split filter run", source_), 2);
    EXPECT_EQ(run_sources_, (std::vector<int>{source_, other_source}));
    EXPECT_EQ(completions_,
              (std::vector<Completion>{{rejected_source, false, 0}, {source_, true, 3}, {other_source, true, 3}}));
}

TEST_F(RedirectTest, testChainExecuteAllForRedirectedCommand)
{
    recordCompletions();
    constexpr int redirected_source = 5;
    constexpr int runtime_source = 6;
    subject_.registerCommand(literal("foo").executes([](const CommandContext<int> &) { return 4; }));
    subject_.registerCommand(
        literal("bar").redirect(subject_.getRoot(), [](const CommandContext<int> &) { return redirected_source; }));

    const ParseResults<int> result = subject_.parse("bar foo", source_);
    const CommandContext<int> top_context = result.getContext().build("bar foo");
    const auto chain = ContextChain<int>::tryFlatten(top_context);
    ASSERT_TRUE(chain.has_value());

    EXPECT_EQ(chain->executeAll(runtime_source,
                                [this](const CommandContext<int> &context, bool success, int value) {
                                    completions_.emplace_back(context.getSource(), success, value);
                                }),
              4);
    EXPECT_EQ(completions_, (std::vector<Completion>{{redirected_source, true, 4}}));
}

TEST_F(RedirectTest, testSingleStageExecution)
{
    subject_.registerCommand(literal("foo").executes(command_));
    const ParseResults<int> result = subject_.parse("foo", source_);
    const CommandContext<int> top_context = result.getContext().build("foo");
    const auto chain = ContextChain<int>::tryFlatten(top_context);
    ASSERT_TRUE(chain.has_value());

    EXPECT_EQ(chain->getStage(), ContextChain<int>::Stage::Execute);
    EXPECT_EQ(&chain->getTopContext().getInput(), &chain->getTopContext().getInput());
    EXPECT_EQ(chain->nextStage(), nullptr);
}

TEST_F(RedirectTest, testMultiStageExecution)
{
    subject_.registerCommand(literal("foo").executes(command_));
    subject_.registerCommand(literal("bar").redirect(subject_.getRoot()));

    const ParseResults<int> result = subject_.parse("bar bar foo", source_);
    const CommandContext<int> top_context = result.getContext().build("bar bar foo");
    const auto stage0 = ContextChain<int>::tryFlatten(top_context);
    ASSERT_TRUE(stage0.has_value());

    EXPECT_EQ(stage0->getStage(), ContextChain<int>::Stage::Modify);
    EXPECT_EQ(stage0->getTopContext().getNodes(), top_context.getNodes());

    const auto stage1 = stage0->nextStage();
    ASSERT_NE(stage1, nullptr);
    EXPECT_EQ(stage1->getStage(), ContextChain<int>::Stage::Modify);
    EXPECT_EQ(stage1->getTopContext().getNodes(), top_context.getChild()->getNodes());

    const auto stage2 = stage1->nextStage();
    ASSERT_NE(stage2, nullptr);
    EXPECT_EQ(stage2->getStage(), ContextChain<int>::Stage::Execute);
    EXPECT_EQ(stage2->getTopContext().getNodes(), top_context.getChild()->getChild()->getNodes());

    EXPECT_EQ(stage2->nextStage(), nullptr);
}

TEST_F(RedirectTest, testMissingExecute)
{
    subject_.registerCommand(literal("foo").executes(command_));
    subject_.registerCommand(literal("bar").redirect(subject_.getRoot()));

    const ParseResults<int> result = subject_.parse("bar bar", source_);
    const CommandContext<int> top_context = result.getContext().build("bar bar");
    EXPECT_FALSE(ContextChain<int>::tryFlatten(top_context).has_value());
}

TEST_F(RedirectTest, getCompletionSuggestionsRedirect)
{
    const auto actual = subject_.registerCommand(literal("actual").then(literal("sub")));
    subject_.registerCommand(literal("redirect").redirect(actual));

    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse("redirect ", source_));
    EXPECT_EQ(result.getRange(), StringRange::at(9));
    EXPECT_EQ(result.getList(), (std::vector<Suggestion>{Suggestion(StringRange::at(9), "sub")}));
}

TEST_F(RedirectTest, getCompletionSuggestionsRedirectPartial)
{
    const auto actual = subject_.registerCommand(literal("actual").then(literal("sub")));
    subject_.registerCommand(literal("redirect").redirect(actual));

    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse("redirect s", source_));
    EXPECT_EQ(result.getRange(), StringRange::between(9, 10));
    EXPECT_EQ(result.getList(), (std::vector<Suggestion>{Suggestion(StringRange::between(9, 10), "sub")}));
}

TEST_F(RedirectTest, getCompletionSuggestionsRedirectLots)
{
    const auto loop = subject_.registerCommand(literal("redirect"));
    subject_.registerCommand(literal("redirect").then(literal("loop").then(integerArgument("loop").redirect(loop))));

    const Suggestions result =
        subject_.getCompletionSuggestions(subject_.parse("redirect loop 1 loop 02 loop 003 ", source_));
    EXPECT_EQ(result.getRange(), StringRange::at(33));
    EXPECT_EQ(result.getList(), (std::vector<Suggestion>{Suggestion(StringRange::at(33), "loop")}));
}

TEST_F(RedirectTest, getCompletionSuggestionsExecuteSimulation)
{
    const auto execute = subject_.registerCommand(literal("execute"));
    subject_.registerCommand(
        literal("execute")
            .then(literal("as").then(Required::argument("name", StringArgumentType::word()).redirect(execute)))
            .then(literal("store").then(Required::argument("name", StringArgumentType::word()).redirect(execute)))
            .then(literal("run").executes([](const CommandContext<int> &) { return 0; })));

    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse("execute as Dinnerbone as", source_));
    EXPECT_TRUE(result.isEmpty());
}

TEST_F(RedirectTest, getCompletionSuggestionsMovingCursorRedirect)
{
    const auto actual_one =
        subject_.registerCommand(literal("actual_one").then(literal("faz")).then(literal("fbz")).then(literal("gaz")));
    subject_.registerCommand(literal("actual_two"));
    subject_.registerCommand(literal("redirect_one").redirect(actual_one));
    subject_.registerCommand(literal("redirect_two").redirect(actual_one));

    const auto check = [this](int cursor, const StringRange &range, const std::vector<std::string> &texts) {
        const Suggestions result =
            subject_.getCompletionSuggestions(subject_.parse("redirect_one faz ", source_), cursor);
        EXPECT_EQ(result.getRange(), range) << cursor;
        std::vector<Suggestion> expected;
        expected.reserve(texts.size());
        for (const std::string &text : texts) {
            expected.emplace_back(range, text);
        }
        EXPECT_EQ(result.getList(), expected) << cursor;
    };
    check(0, StringRange::at(0), {"actual_one", "actual_two", "redirect_one", "redirect_two"});
    check(9, StringRange::between(0, 9), {"redirect_one", "redirect_two"});
    check(10, StringRange::between(0, 10), {"redirect_one"});
    check(12, StringRange::at(0), {});
    check(13, StringRange::at(13), {"faz", "fbz", "gaz"});
    check(14, StringRange::between(13, 14), {"faz", "fbz"});
    check(15, StringRange::between(13, 15), {"faz"});
    check(16, StringRange::at(0), {});
    check(17, StringRange::at(0), {});
}

TEST_F(RedirectTest, removeChildByName)
{
    subject_.registerCommand(literal("foo"));
    subject_.registerCommand(literal("bar"));
    subject_.getRoot()->removeChildByName("foo");
    EXPECT_EQ(subject_.getRoot()->getChild("foo"), nullptr);
    EXPECT_NE(subject_.getRoot()->getChild("bar"), nullptr);
    EXPECT_EQ(subject_.getRoot()->getChildren().size(), 1U);
    EXPECT_THROW(subject_.execute("foo", source_), CommandSyntaxException);
}

}  // namespace
}  // namespace endstone::brigadier
