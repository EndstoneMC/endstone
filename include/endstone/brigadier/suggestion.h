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

#pragma once

#include <algorithm>
#include <functional>
#include <limits>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "endstone/brigadier/context.h"

namespace endstone::brigadier {

/**
 * A completion for part of the input, with the range it replaces.
 *
 * @see com.mojang.brigadier.suggestion.Suggestion
 */
class Suggestion {
public:
    Suggestion(const StringRange &range, std::string text) : range_(range), text_(std::move(text)) {}

    Suggestion(const StringRange &range, std::string text, std::optional<Message> tooltip)
        : range_(range), text_(std::move(text)), tooltip_(std::move(tooltip))
    {
    }

    [[nodiscard]] const StringRange &getRange() const { return range_; }
    [[nodiscard]] const std::string &getText() const { return text_; }
    [[nodiscard]] const std::optional<Message> &getTooltip() const { return tooltip_; }

    [[nodiscard]] std::string apply(const std::string &input) const
    {
        if (range_.getStart() == 0 && range_.getEnd() == static_cast<int>(input.length())) {
            return text_;
        }
        std::string result;
        if (range_.getStart() > 0) {
            result += input.substr(0, range_.getStart());
        }
        result += text_;
        if (range_.getEnd() < static_cast<int>(input.length())) {
            result += input.substr(range_.getEnd());
        }
        return result;
    }

    bool operator==(const Suggestion &) const = default;

    [[nodiscard]] int compareTo(const Suggestion &o) const { return text_.compare(o.text_); }

    [[nodiscard]] int compareToIgnoreCase(const Suggestion &b) const
    {
        const std::size_t length = std::min(text_.length(), b.text_.length());
        for (std::size_t i = 0; i < length; i++) {
            const int x = lower(text_[i]);
            const int y = lower(b.text_[i]);
            if (x != y) {
                return x - y;
            }
        }
        return static_cast<int>(text_.length()) - static_cast<int>(b.text_.length());
    }

    [[nodiscard]] Suggestion expand(const std::string &command, const StringRange &range) const
    {
        if (range == range_) {
            return *this;
        }
        std::string result;
        if (range.getStart() < range_.getStart()) {
            result += command.substr(range.getStart(), range_.getStart() - range.getStart());
        }
        result += text_;
        if (range.getEnd() > range_.getEnd()) {
            result += command.substr(range_.getEnd(), range.getEnd() - range_.getEnd());
        }
        return {range, result, tooltip_};
    }

private:
    static int lower(char c) { return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }

    StringRange range_;
    std::string text_;
    std::optional<Message> tooltip_;
};

/**
 * @see com.mojang.brigadier.suggestion.Suggestions
 */
class Suggestions {
public:
    Suggestions(const StringRange &range, std::vector<Suggestion> suggestions)
        : range_(range), suggestions_(std::move(suggestions))
    {
    }

    [[nodiscard]] const StringRange &getRange() const { return range_; }
    [[nodiscard]] const std::vector<Suggestion> &getList() const { return suggestions_; }
    [[nodiscard]] bool isEmpty() const { return suggestions_.empty(); }

    bool operator==(const Suggestions &) const = default;

    static Suggestions empty() { return {StringRange::at(0), {}}; }

    static Suggestions merge(const std::string &command, const std::vector<Suggestions> &input)
    {
        if (input.empty()) {
            return empty();
        }
        if (input.size() == 1) {
            return input.front();
        }
        std::vector<Suggestion> texts;
        for (const Suggestions &suggestions : input) {
            texts.insert(texts.end(), suggestions.getList().begin(), suggestions.getList().end());
        }
        return create(command, texts);
    }

    static Suggestions create(const std::string &command, const std::vector<Suggestion> &suggestions)
    {
        if (suggestions.empty()) {
            return empty();
        }
        int start = std::numeric_limits<int>::max();
        int end = std::numeric_limits<int>::min();
        for (const Suggestion &suggestion : suggestions) {
            start = std::min(suggestion.getRange().getStart(), start);
            end = std::max(suggestion.getRange().getEnd(), end);
        }
        const StringRange range(start, end);
        std::vector<Suggestion> sorted;
        sorted.reserve(suggestions.size());
        for (const Suggestion &suggestion : suggestions) {
            sorted.push_back(suggestion.expand(command, range));
        }
        std::ranges::sort(sorted, [](const Suggestion &a, const Suggestion &b) {
            if (const int order = a.compareToIgnoreCase(b); order != 0) {
                return order < 0;
            }
            if (a.getText() != b.getText()) {
                return a.getText() > b.getText();
            }
            return a.getTooltip() < b.getTooltip();
        });
        sorted.erase(std::ranges::unique(sorted).begin(), sorted.end());
        return {range, std::move(sorted)};
    }

private:
    StringRange range_;
    std::vector<Suggestion> suggestions_;
};

/**
 * Collects the completions for the input from a start position on.
 *
 * @see com.mojang.brigadier.suggestion.SuggestionsBuilder
 */
class SuggestionsBuilder {
public:
    SuggestionsBuilder(std::string input, std::string input_lower_case, int start)
        : input_(std::move(input)), input_lower_case_(std::move(input_lower_case)), start_(start),
          remaining_(input_.substr(start)), remaining_lower_case_(input_lower_case_.substr(start))
    {
    }

    SuggestionsBuilder(const std::string &input, int start) : SuggestionsBuilder(input, toLowerCase(input), start) {}

    static std::string toLowerCase(std::string text)
    {
        std::ranges::transform(text, text.begin(), [](char c) { return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; });
        return text;
    }

    [[nodiscard]] const std::string &getInput() const { return input_; }
    [[nodiscard]] int getStart() const { return start_; }
    [[nodiscard]] const std::string &getRemaining() const { return remaining_; }
    [[nodiscard]] const std::string &getRemainingLowerCase() const { return remaining_lower_case_; }

    [[nodiscard]] Suggestions build() const { return Suggestions::create(input_, result_); }

    SuggestionsBuilder &suggest(const std::string &text)
    {
        if (text == remaining_) {
            return *this;
        }
        result_.emplace_back(StringRange::between(start_, static_cast<int>(input_.length())), text);
        return *this;
    }

    SuggestionsBuilder &suggest(const std::string &text, const Message &tooltip)
    {
        if (text == remaining_) {
            return *this;
        }
        result_.emplace_back(StringRange::between(start_, static_cast<int>(input_.length())), text, tooltip);
        return *this;
    }

    SuggestionsBuilder &add(const SuggestionsBuilder &other)
    {
        result_.insert(result_.end(), other.result_.begin(), other.result_.end());
        return *this;
    }

    [[nodiscard]] SuggestionsBuilder createOffset(int start) const { return {input_, input_lower_case_, start}; }
    [[nodiscard]] SuggestionsBuilder restart() const { return createOffset(start_); }

private:
    std::string input_;
    std::string input_lower_case_;
    int start_;
    std::string remaining_;
    std::string remaining_lower_case_;
    std::vector<Suggestion> result_;
};

/**
 * @see com.mojang.brigadier.suggestion.SuggestionProvider
 */
template <typename S>
using SuggestionProvider = std::function<Suggestions(const CommandContext<S> &, SuggestionsBuilder &)>;

}  // namespace endstone::brigadier
