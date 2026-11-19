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

class CommandSuggestionsTest : public ::testing::Test {
protected:
    static Literal literal(const std::string &name) { return Literal::literal(name); }

    void testSuggestions(const std::string &contents, int cursor, const StringRange &range,
                         const std::vector<std::string> &suggestions)
    {
        const Suggestions result = subject_.getCompletionSuggestions(subject_.parse(contents, source_), cursor);
        EXPECT_EQ(result.getRange(), range) << contents << " at " << cursor;
        std::vector<Suggestion> expected;
        expected.reserve(suggestions.size());
        for (const std::string &suggestion : suggestions) {
            expected.emplace_back(range, suggestion);
        }
        EXPECT_EQ(result.getList(), expected) << contents << " at " << cursor;
    }

    static StringReader inputWithOffset(const std::string &input, int offset)
    {
        StringReader result(input);
        result.setCursor(offset);
        return result;
    }

    static std::vector<Suggestion> at(const StringRange &range, const std::vector<std::string> &texts)
    {
        std::vector<Suggestion> result;
        result.reserve(texts.size());
        for (const std::string &text : texts) {
            result.emplace_back(range, text);
        }
        return result;
    }

    CommandDispatcher<int> subject_;
    int source_ = 0;
};

TEST_F(CommandSuggestionsTest, getCompletionSuggestionsRootCommands)
{
    subject_.registerCommand(literal("foo"));
    subject_.registerCommand(literal("bar"));
    subject_.registerCommand(literal("baz"));
    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse("", source_));
    EXPECT_EQ(result.getRange(), StringRange::at(0));
    EXPECT_EQ(result.getList(), at(StringRange::at(0), {"bar", "baz", "foo"}));
}

TEST_F(CommandSuggestionsTest, getCompletionSuggestionsRootCommandsWithInputOffset)
{
    subject_.registerCommand(literal("foo"));
    subject_.registerCommand(literal("bar"));
    subject_.registerCommand(literal("baz"));
    StringReader input = inputWithOffset("OOO", 3);
    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse(input, source_));
    EXPECT_EQ(result.getRange(), StringRange::at(3));
    EXPECT_EQ(result.getList(), at(StringRange::at(3), {"bar", "baz", "foo"}));
}

TEST_F(CommandSuggestionsTest, getCompletionSuggestionsRootCommandsPartial)
{
    subject_.registerCommand(literal("foo"));
    subject_.registerCommand(literal("bar"));
    subject_.registerCommand(literal("baz"));
    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse("b", source_));
    EXPECT_EQ(result.getRange(), StringRange::between(0, 1));
    EXPECT_EQ(result.getList(), at(StringRange::between(0, 1), {"bar", "baz"}));
}

TEST_F(CommandSuggestionsTest, getCompletionSuggestionsRootCommandsPartialWithInputOffset)
{
    subject_.registerCommand(literal("foo"));
    subject_.registerCommand(literal("bar"));
    subject_.registerCommand(literal("baz"));
    StringReader input = inputWithOffset("Zb", 1);
    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse(input, source_));
    EXPECT_EQ(result.getRange(), StringRange::between(1, 2));
    EXPECT_EQ(result.getList(), at(StringRange::between(1, 2), {"bar", "baz"}));
}

TEST_F(CommandSuggestionsTest, getCompletionSuggestionsSubCommands)
{
    subject_.registerCommand(literal("parent").then(literal("foo")).then(literal("bar")).then(literal("baz")));
    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse("parent ", source_));
    EXPECT_EQ(result.getRange(), StringRange::at(7));
    EXPECT_EQ(result.getList(), at(StringRange::at(7), {"bar", "baz", "foo"}));
}

TEST_F(CommandSuggestionsTest, getCompletionSuggestionsMovingCursorSubCommands)
{
    subject_.registerCommand(literal("parent_one").then(literal("faz")).then(literal("fbz")).then(literal("gaz")));
    subject_.registerCommand(literal("parent_two"));
    testSuggestions("parent_one faz ", 0, StringRange::at(0), {"parent_one", "parent_two"});
    testSuggestions("parent_one faz ", 1, StringRange::between(0, 1), {"parent_one", "parent_two"});
    testSuggestions("parent_one faz ", 7, StringRange::between(0, 7), {"parent_one", "parent_two"});
    testSuggestions("parent_one faz ", 8, StringRange::between(0, 8), {"parent_one"});
    testSuggestions("parent_one faz ", 10, StringRange::at(0), {});
    testSuggestions("parent_one faz ", 11, StringRange::at(11), {"faz", "fbz", "gaz"});
    testSuggestions("parent_one faz ", 12, StringRange::between(11, 12), {"faz", "fbz"});
    testSuggestions("parent_one faz ", 13, StringRange::between(11, 13), {"faz"});
    testSuggestions("parent_one faz ", 14, StringRange::at(0), {});
    testSuggestions("parent_one faz ", 15, StringRange::at(0), {});
}

TEST_F(CommandSuggestionsTest, getCompletionSuggestionsSubCommandsPartial)
{
    subject_.registerCommand(literal("parent").then(literal("foo")).then(literal("bar")).then(literal("baz")));
    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse("parent b", source_));
    EXPECT_EQ(result.getRange(), StringRange::between(7, 8));
    EXPECT_EQ(result.getList(), at(StringRange::between(7, 8), {"bar", "baz"}));
}

TEST_F(CommandSuggestionsTest, getCompletionSuggestionsSubCommandsPartialWithInputOffset)
{
    subject_.registerCommand(literal("parent").then(literal("foo")).then(literal("bar")).then(literal("baz")));
    StringReader input = inputWithOffset("junk parent b", 5);
    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse(input, source_));
    EXPECT_EQ(result.getRange(), StringRange::between(12, 13));
    EXPECT_EQ(result.getList(), at(StringRange::between(12, 13), {"bar", "baz"}));
}

TEST_F(CommandSuggestionsTest, getCompletionSuggestionsCustomProvider)
{
    subject_.registerCommand(literal("actual").then(
        Required::argument("arg_one", StringArgumentType::word())
            .then(Required::argument("arg_two", StringArgumentType::word())
                      .suggests([](const CommandContext<int> &context, SuggestionsBuilder &builder) {
                          builder.suggest("contextual_" + StringArgumentType::getString(context, "arg_one"));
                          return builder.build();
                      }))));
    const Suggestions result = subject_.getCompletionSuggestions(subject_.parse("actual first ", source_));
    EXPECT_EQ(result.getRange(), StringRange::at(13));
    EXPECT_EQ(result.getList(), at(StringRange::at(13), {"contextual_first"}));
}

}  // namespace
}  // namespace endstone::brigadier
