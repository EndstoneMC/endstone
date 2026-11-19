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

#include "endstone/brigadier/suggestion.h"

namespace endstone::brigadier {
namespace {

TEST(SuggestionTest, applyInsertationStart)
{
    EXPECT_EQ(Suggestion(StringRange::at(0), "And so I said: ").apply("Hello world!"), "And so I said: Hello world!");
}

TEST(SuggestionTest, applyInsertationMiddle)
{
    EXPECT_EQ(Suggestion(StringRange::at(6), "small ").apply("Hello world!"), "Hello small world!");
}

TEST(SuggestionTest, applyInsertationEnd)
{
    EXPECT_EQ(Suggestion(StringRange::at(5), " world!").apply("Hello"), "Hello world!");
}

TEST(SuggestionTest, applyReplacementStart)
{
    EXPECT_EQ(Suggestion(StringRange::between(0, 5), "Goodbye").apply("Hello world!"), "Goodbye world!");
}

TEST(SuggestionTest, applyReplacementMiddle)
{
    EXPECT_EQ(Suggestion(StringRange::between(6, 11), "Alex").apply("Hello world!"), "Hello Alex!");
}

TEST(SuggestionTest, applyReplacementEnd)
{
    EXPECT_EQ(Suggestion(StringRange::between(6, 12), "Creeper!").apply("Hello world!"), "Hello Creeper!");
}

TEST(SuggestionTest, applyReplacementEverything)
{
    EXPECT_EQ(Suggestion(StringRange::between(0, 12), "Oh dear.").apply("Hello world!"), "Oh dear.");
}

TEST(SuggestionTest, expandUnchanged)
{
    const Suggestion suggestion(StringRange::at(1), "oo");
    EXPECT_EQ(suggestion.expand("f", StringRange::at(1)), suggestion);
}

TEST(SuggestionTest, expandLeft)
{
    const Suggestion suggestion(StringRange::at(1), "oo");
    EXPECT_EQ(suggestion.expand("f", StringRange::between(0, 1)), Suggestion(StringRange::between(0, 1), "foo"));
}

TEST(SuggestionTest, expandRight)
{
    const Suggestion suggestion(StringRange::at(0), "minecraft:");
    EXPECT_EQ(suggestion.expand("fish", StringRange::between(0, 4)),
              Suggestion(StringRange::between(0, 4), "minecraft:fish"));
}

TEST(SuggestionTest, expandBoth)
{
    const Suggestion suggestion(StringRange::at(11), "minecraft:");
    EXPECT_EQ(suggestion.expand("give Steve fish_block", StringRange::between(5, 21)),
              Suggestion(StringRange::between(5, 21), "Steve minecraft:fish_block"));
}

TEST(SuggestionTest, expandReplacement)
{
    const Suggestion suggestion(StringRange::between(6, 11), "strangers");
    EXPECT_EQ(suggestion.expand("Hello world!", StringRange::between(0, 12)),
              Suggestion(StringRange::between(0, 12), "Hello strangers!"));
}

class SuggestionsBuilderTest : public ::testing::Test {
protected:
    SuggestionsBuilder builder_{"Hello w", 6};
};

TEST_F(SuggestionsBuilderTest, suggestAppends)
{
    const Suggestions result = builder_.suggest("world!").build();
    EXPECT_EQ(result.getList(), std::vector{Suggestion(StringRange::between(6, 7), "world!")});
    EXPECT_EQ(result.getRange(), StringRange::between(6, 7));
    EXPECT_FALSE(result.isEmpty());
}

TEST_F(SuggestionsBuilderTest, suggestReplaces)
{
    const Suggestions result = builder_.suggest("everybody").build();
    EXPECT_EQ(result.getList(), std::vector{Suggestion(StringRange::between(6, 7), "everybody")});
    EXPECT_EQ(result.getRange(), StringRange::between(6, 7));
    EXPECT_FALSE(result.isEmpty());
}

TEST_F(SuggestionsBuilderTest, suggestNoop)
{
    const Suggestions result = builder_.suggest("w").build();
    EXPECT_TRUE(result.getList().empty());
    EXPECT_TRUE(result.isEmpty());
}

TEST_F(SuggestionsBuilderTest, suggestMultiple)
{
    const Suggestions result = builder_.suggest("world!").suggest("everybody").suggest("weekend").build();
    EXPECT_EQ(result.getList(), (std::vector{Suggestion(StringRange::between(6, 7), "everybody"),
                                             Suggestion(StringRange::between(6, 7), "weekend"),
                                             Suggestion(StringRange::between(6, 7), "world!")}));
    EXPECT_EQ(result.getRange(), StringRange::between(6, 7));
    EXPECT_FALSE(result.isEmpty());
}

TEST_F(SuggestionsBuilderTest, restart)
{
    builder_.suggest("will not be included in restart");
    const SuggestionsBuilder other = builder_.restart();
    EXPECT_NE(&other, &builder_);
    EXPECT_EQ(other.getInput(), builder_.getInput());
    EXPECT_EQ(other.getStart(), builder_.getStart());
    EXPECT_EQ(other.getRemaining(), builder_.getRemaining());
    EXPECT_TRUE(other.build().isEmpty());
}

TEST_F(SuggestionsBuilderTest, sortAlphabetical)
{
    const Suggestions result =
        builder_.suggest("2").suggest("4").suggest("6").suggest("8").suggest("30").suggest("32").build();
    std::vector<std::string> actual;
    for (const Suggestion &suggestion : result.getList()) {
        actual.push_back(suggestion.getText());
    }
    EXPECT_EQ(actual, (std::vector<std::string>{"2", "30", "32", "4", "6", "8"}));
}

TEST(SuggestionsTest, mergeEmpty)
{
    EXPECT_TRUE(Suggestions::merge("foo b", {}).isEmpty());
}

TEST(SuggestionsTest, mergeSingle)
{
    const Suggestions suggestions(StringRange::at(5), {Suggestion(StringRange::at(5), "ar")});
    EXPECT_EQ(Suggestions::merge("foo b", {suggestions}), suggestions);
}

TEST(SuggestionsTest, mergeMultiple)
{
    const Suggestions a(StringRange::at(5), {Suggestion(StringRange::at(5), "ar"), Suggestion(StringRange::at(5), "az"),
                                             Suggestion(StringRange::at(5), "Az")});
    const Suggestions b(StringRange::between(4, 5),
                        {Suggestion(StringRange::between(4, 5), "foo"), Suggestion(StringRange::between(4, 5), "qux"),
                         Suggestion(StringRange::between(4, 5), "apple"),
                         Suggestion(StringRange::between(4, 5), "Bar")});
    const Suggestions merged = Suggestions::merge("foo b", {a, b});
    EXPECT_EQ(
        merged.getList(),
        (std::vector{Suggestion(StringRange::between(4, 5), "apple"), Suggestion(StringRange::between(4, 5), "bar"),
                     Suggestion(StringRange::between(4, 5), "Bar"), Suggestion(StringRange::between(4, 5), "baz"),
                     Suggestion(StringRange::between(4, 5), "bAz"), Suggestion(StringRange::between(4, 5), "foo"),
                     Suggestion(StringRange::between(4, 5), "qux")}));
}

}  // namespace
}  // namespace endstone::brigadier
