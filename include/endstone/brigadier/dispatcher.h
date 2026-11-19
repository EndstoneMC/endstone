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
#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "endstone/brigadier/arguments.h"
#include "endstone/brigadier/builder.h"
#include "endstone/brigadier/context.h"
#include "endstone/brigadier/exceptions.h"
#include "endstone/brigadier/reader.h"
#include "endstone/brigadier/suggestion.h"
#include "endstone/brigadier/tree.h"

namespace endstone::brigadier {

inline constexpr int SINGLE_SUCCESS = 1;

/**
 * @see com.mojang.brigadier.ParseResults
 */
template <typename S>
class ParseResults {
public:
    using ExceptionMap = std::vector<std::pair<std::shared_ptr<CommandNode<S>>, CommandSyntaxException>>;

    ParseResults(CommandContextBuilder<S> context, StringReader reader, ExceptionMap exceptions)
        : context_(std::move(context)), exceptions_(std::move(exceptions)), reader_(std::move(reader))
    {
    }

    explicit ParseResults(CommandContextBuilder<S> context) : ParseResults(std::move(context), StringReader(""), {}) {}

    [[nodiscard]] const CommandContextBuilder<S> &getContext() const { return context_; }
    [[nodiscard]] const StringReader &getReader() const { return reader_; }
    [[nodiscard]] const ExceptionMap &getExceptions() const { return exceptions_; }

private:
    CommandContextBuilder<S> context_;
    ExceptionMap exceptions_;
    StringReader reader_;
};

/**
 * The core command dispatcher, for registering, parsing, and executing commands.
 *
 * @see com.mojang.brigadier.CommandDispatcher
 */
template <typename S>
class CommandDispatcher {
public:
    static constexpr std::string_view ARGUMENT_SEPARATOR = " ";
    static constexpr char ARGUMENT_SEPARATOR_CHAR = ' ';

    explicit CommandDispatcher(std::shared_ptr<RootCommandNode<S>> root) : root_(std::move(root)) {}
    CommandDispatcher() : CommandDispatcher(std::make_shared<RootCommandNode<S>>()) {}

    void setConsumer(ResultConsumer<S> consumer) { consumer_ = std::move(consumer); }

    std::shared_ptr<LiteralCommandNode<S>> registerCommand(const LiteralArgumentBuilder<S> &command)
    {
        auto build = command.build();
        root_->addChild(build);
        return build;
    }

    int execute(const std::string &input, const S &source)
    {
        StringReader reader(input);
        return execute(reader, source);
    }

    int execute(StringReader &input, const S &source)
    {
        const ParseResults<S> parse = this->parse(input, source);
        return execute(parse);
    }

    int execute(const ParseResults<S> &parse)
    {
        if (parse.getReader().canRead()) {
            if (parse.getExceptions().size() == 1) {
                throw parse.getExceptions().front().second;
            }
            if (parse.getContext().getRange().isEmpty()) {
                throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownCommand().createWithContext(
                    parse.getReader());
            }
            throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownArgument().createWithContext(
                parse.getReader());
        }
        const std::string &command = parse.getReader().getString();
        const CommandContext<S> original = parse.getContext().build(command);

        const std::optional<ContextChain<S>> flat_context = ContextChain<S>::tryFlatten(original);
        if (!flat_context) {
            consumer_(original, false, 0);
            throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherUnknownCommand().createWithContext(
                parse.getReader());
        }

        return flat_context->executeAll(original.getSource(), consumer_);
    }

    ParseResults<S> parse(const std::string &command, const S &source)
    {
        StringReader reader(command);
        return parse(reader, source);
    }

    ParseResults<S> parse(StringReader &command, const S &source)
    {
        CommandContextBuilder<S> context(*this, source, root_, command.getCursor());
        return parseNodes(root_, command, context);
    }

    [[nodiscard]] std::vector<std::string> getAllUsage(const CommandNode<S> &node, const S &source,
                                                       bool restricted) const
    {
        std::vector<std::string> result;
        getAllUsage(node, source, result, "", restricted);
        return result;
    }

    [[nodiscard]] std::vector<std::pair<std::shared_ptr<CommandNode<S>>, std::string>> getSmartUsage(
        const CommandNode<S> &node, const S &source) const
    {
        std::vector<std::pair<std::shared_ptr<CommandNode<S>>, std::string>> result;
        const bool optional = static_cast<bool>(node.getCommand());
        for (const std::shared_ptr<CommandNode<S>> &child : node.getChildren()) {
            if (std::optional<std::string> usage = getSmartUsage(*child, source, optional, false)) {
                result.emplace_back(child, std::move(*usage));
            }
        }
        return result;
    }

    Suggestions getCompletionSuggestions(const ParseResults<S> &parse) const
    {
        return getCompletionSuggestions(parse, parse.getReader().getTotalLength());
    }

    Suggestions getCompletionSuggestions(const ParseResults<S> &parse, int cursor) const
    {
        const CommandContextBuilder<S> &context = parse.getContext();
        const SuggestionContext<S> node_before_cursor = context.findSuggestionContext(cursor);
        const std::shared_ptr<CommandNode<S>> &parent = node_before_cursor.parent;
        const int start = std::min(node_before_cursor.start_pos, cursor);
        const std::string &full_input = parse.getReader().getString();
        const std::string truncated_input = full_input.substr(0, cursor);
        const std::string truncated_input_lower_case = SuggestionsBuilder::toLowerCase(truncated_input);
        std::vector<Suggestions> suggestions;
        suggestions.reserve(parent->getChildren().size());
        for (const std::shared_ptr<CommandNode<S>> &node : parent->getChildren()) {
            Suggestions future = Suggestions::empty();
            try {
                SuggestionsBuilder builder(truncated_input, truncated_input_lower_case, start);
                future = node->listSuggestions(node_before_cursor.context.build(truncated_input), builder);
            }
            catch (const CommandSyntaxException &) {
            }
            suggestions.push_back(std::move(future));
        }
        return Suggestions::merge(full_input, suggestions);
    }

    [[nodiscard]] const std::shared_ptr<RootCommandNode<S>> &getRoot() const { return root_; }

    [[nodiscard]] std::vector<std::string> getPath(const CommandNode<S> &target) const
    {
        std::vector<std::vector<const CommandNode<S> *>> nodes;
        addPaths(*root_, nodes, {});
        for (const std::vector<const CommandNode<S> *> &list : nodes) {
            if (list.back() == &target) {
                std::vector<std::string> result;
                result.reserve(list.size());
                for (const CommandNode<S> *node : list) {
                    if (node != root_.get()) {
                        result.push_back(node->getName());
                    }
                }
                return result;
            }
        }
        return {};
    }

    [[nodiscard]] std::shared_ptr<CommandNode<S>> findNode(const std::vector<std::string> &path) const
    {
        std::shared_ptr<CommandNode<S>> node = root_;
        for (const std::string &name : path) {
            node = node->getChild(name);
            if (!node) {
                return nullptr;
            }
        }
        return node;
    }

private:
    ParseResults<S> parseNodes(const std::shared_ptr<CommandNode<S>> &node, StringReader &original_reader,
                               const CommandContextBuilder<S> &context_so_far)
    {
        const S &source = context_so_far.getSource();
        typename ParseResults<S>::ExceptionMap errors;
        std::vector<ParseResults<S>> potentials;
        const int cursor = original_reader.getCursor();

        for (const std::shared_ptr<CommandNode<S>> &child : node->getRelevantNodes(original_reader)) {
            if (!child->canUse(source)) {
                continue;
            }
            CommandContextBuilder<S> context = context_so_far.copy();
            StringReader reader(original_reader);
            try {
                try {
                    child->parse(reader, context);
                    context.withNode(child, StringRange::between(cursor, reader.getCursor()));
                }
                catch (const CommandSyntaxException &) {
                    throw;
                }
                catch (const std::exception &ex) {
                    throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherParseException().createWithContext(
                        reader, ex.what());
                }
                if (reader.canRead()) {
                    if (reader.peek() != ARGUMENT_SEPARATOR_CHAR) {
                        throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.dispatcherExpectedArgumentSeparator()
                            .createWithContext(reader);
                    }
                }
            }
            catch (const CommandSyntaxException &ex) {
                errors.emplace_back(child, ex);
                reader.setCursor(cursor);
                continue;
            }
            context.withCommand(child->getCommand());
            if (reader.canRead(child->getRedirect() == nullptr ? 2 : 1)) {
                reader.skip();
                if (child->getRedirect() != nullptr) {
                    CommandContextBuilder<S> child_context(*this, source, child->getRedirectTarget(),
                                                           reader.getCursor());
                    ParseResults<S> parse = parseNodes(child->getRedirectTarget(), reader, child_context);
                    context.withChild(parse.getContext());
                    return {std::move(context), parse.getReader(), parse.getExceptions()};
                }
                potentials.push_back(parseNodes(child, reader, context));
            }
            else {
                potentials.emplace_back(std::move(context), std::move(reader),
                                        typename ParseResults<S>::ExceptionMap{});
            }
        }

        if (!potentials.empty()) {
            if (potentials.size() > 1) {
                std::ranges::stable_sort(potentials, [](const ParseResults<S> &a, const ParseResults<S> &b) {
                    if (!a.getReader().canRead() && b.getReader().canRead()) {
                        return true;
                    }
                    if (a.getReader().canRead() && !b.getReader().canRead()) {
                        return false;
                    }
                    return a.getExceptions().empty() && !b.getExceptions().empty();
                });
            }
            return std::move(potentials.front());
        }

        return {context_so_far, original_reader, std::move(errors)};
    }

    void getAllUsage(const CommandNode<S> &node, const S &source, std::vector<std::string> &result,
                     const std::string &prefix, bool restricted) const
    {
        if (restricted && !node.canUse(source)) {
            return;
        }
        if (node.getCommand()) {
            result.push_back(prefix);
        }

        if (node.getRedirect() != nullptr) {
            const std::string redirect =
                node.getRedirect() == root_.get() ? "..." : "-> " + node.getRedirect()->getUsageText();
            result.push_back(prefix.empty() ? node.getUsageText() + std::string(ARGUMENT_SEPARATOR) + redirect
                                            : prefix + std::string(ARGUMENT_SEPARATOR) + redirect);
        }
        else if (!node.getChildren().empty()) {
            for (const std::shared_ptr<CommandNode<S>> &child : node.getChildren()) {
                getAllUsage(*child, source, result,
                            prefix.empty() ? child->getUsageText()
                                           : prefix + std::string(ARGUMENT_SEPARATOR) + child->getUsageText(),
                            restricted);
            }
        }
    }

    [[nodiscard]] std::optional<std::string> getSmartUsage(const CommandNode<S> &node, const S &source, bool optional,
                                                           bool deep) const
    {
        if (!node.canUse(source)) {
            return std::nullopt;
        }

        const std::string self =
            optional ? USAGE_OPTIONAL_OPEN + node.getUsageText() + USAGE_OPTIONAL_CLOSE : node.getUsageText();
        const bool child_optional = static_cast<bool>(node.getCommand());
        const std::string open = child_optional ? USAGE_OPTIONAL_OPEN : USAGE_REQUIRED_OPEN;
        const std::string close = child_optional ? USAGE_OPTIONAL_CLOSE : USAGE_REQUIRED_CLOSE;

        if (!deep) {
            if (node.getRedirect() != nullptr) {
                const std::string redirect =
                    node.getRedirect() == root_.get() ? "..." : "-> " + node.getRedirect()->getUsageText();
                return self + std::string(ARGUMENT_SEPARATOR) + redirect;
            }
            std::vector<std::shared_ptr<CommandNode<S>>> children;
            for (const std::shared_ptr<CommandNode<S>> &child : node.getChildren()) {
                if (child->canUse(source)) {
                    children.push_back(child);
                }
            }

            if (children.size() == 1) {
                if (std::optional<std::string> usage =
                        getSmartUsage(*children.front(), source, child_optional, child_optional)) {
                    return self + std::string(ARGUMENT_SEPARATOR) + *usage;
                }
            }
            else if (children.size() > 1) {
                std::vector<std::string> child_usage;
                for (const std::shared_ptr<CommandNode<S>> &child : children) {
                    if (std::optional<std::string> usage = getSmartUsage(*child, source, child_optional, true)) {
                        if (std::ranges::find(child_usage, *usage) == child_usage.end()) {
                            child_usage.push_back(std::move(*usage));
                        }
                    }
                }
                if (child_usage.size() == 1) {
                    const std::string &usage = child_usage.front();
                    return self + std::string(ARGUMENT_SEPARATOR) +
                           (child_optional ? USAGE_OPTIONAL_OPEN + usage + USAGE_OPTIONAL_CLOSE : usage);
                }
                if (child_usage.size() > 1) {
                    std::string builder = open;
                    int count = 0;
                    for (const std::shared_ptr<CommandNode<S>> &child : children) {
                        if (count > 0) {
                            builder += USAGE_OR;
                        }
                        builder += child->getUsageText();
                        count++;
                    }
                    if (count > 0) {
                        builder += close;
                        return self + std::string(ARGUMENT_SEPARATOR) + builder;
                    }
                }
            }
        }

        return self;
    }

    void addPaths(const CommandNode<S> &node, std::vector<std::vector<const CommandNode<S> *>> &result,
                  const std::vector<const CommandNode<S> *> &parents) const
    {
        std::vector<const CommandNode<S> *> current = parents;
        current.push_back(&node);
        result.push_back(current);

        for (const std::shared_ptr<CommandNode<S>> &child : node.getChildren()) {
            addPaths(*child, result, current);
        }
    }

    static constexpr const char *USAGE_OPTIONAL_OPEN = "[";
    static constexpr const char *USAGE_OPTIONAL_CLOSE = "]";
    static constexpr const char *USAGE_REQUIRED_OPEN = "(";
    static constexpr const char *USAGE_REQUIRED_CLOSE = ")";
    static constexpr const char *USAGE_OR = "|";

    std::shared_ptr<RootCommandNode<S>> root_;
    ResultConsumer<S> consumer_ = [](const CommandContext<S> &, bool, int) {
    };
};

}  // namespace endstone::brigadier
