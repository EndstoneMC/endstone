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
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "endstone/brigadier/arguments.h"
#include "endstone/brigadier/exceptions.h"
#include "endstone/brigadier/reader.h"

namespace endstone::brigadier {

template <typename S>
class CommandNode;

template <typename S>
class CommandDispatcher;

template <typename S>
class CommandContext;

/**
 * The code that runs when a command's node is reached.
 *
 * @see com.mojang.brigadier.Command
 */
template <typename S>
using Command = std::function<int(const CommandContext<S> &)>;

/**
 * Turns the source of a command into the sources of the command it redirects to.
 *
 * @see com.mojang.brigadier.RedirectModifier
 */
template <typename S>
using RedirectModifier = std::function<std::vector<S>(const CommandContext<S> &)>;

/**
 * Turns the source of a command into the one source of the command it redirects to.
 *
 * @see com.mojang.brigadier.SingleRedirectModifier
 */
template <typename S>
using SingleRedirectModifier = std::function<S(const CommandContext<S> &)>;

/**
 * Notified after each command that ran.
 *
 * @see com.mojang.brigadier.ResultConsumer
 */
template <typename S>
using ResultConsumer = std::function<void(const CommandContext<S> &, bool, int)>;

/**
 * The span of the input that a node or an argument consumed.
 *
 * @see com.mojang.brigadier.context.StringRange
 */
class StringRange {
public:
    StringRange(int start, int end) : start_(start), end_(end) {}

    static StringRange at(int pos) { return {pos, pos}; }
    static StringRange between(int start, int end) { return {start, end}; }

    static StringRange encompassing(const StringRange &a, const StringRange &b)
    {
        return {std::min(a.getStart(), b.getStart()), std::max(a.getEnd(), b.getEnd())};
    }

    [[nodiscard]] int getStart() const { return start_; }
    [[nodiscard]] int getEnd() const { return end_; }

    [[nodiscard]] std::string get(const StringReader &reader) const { return get(reader.getString()); }
    [[nodiscard]] std::string get(const std::string &string) const { return string.substr(start_, end_ - start_); }

    [[nodiscard]] bool isEmpty() const { return start_ == end_; }
    [[nodiscard]] int getLength() const { return end_ - start_; }

    bool operator==(const StringRange &) const = default;

private:
    int start_;
    int end_;
};

/**
 * @see com.mojang.brigadier.context.ParsedArgument
 */
class ParsedArgument {
public:
    ParsedArgument(int start, int end, ArgumentValue result)
        : range_(StringRange::between(start, end)), result_(std::move(result))
    {
    }

    [[nodiscard]] const StringRange &getRange() const { return range_; }
    [[nodiscard]] const ArgumentValue &getResult() const { return result_; }

    bool operator==(const ParsedArgument &) const = default;

private:
    StringRange range_;
    ArgumentValue result_;
};

/**
 * @see com.mojang.brigadier.context.ParsedCommandNode
 */
template <typename S>
class ParsedCommandNode {
public:
    ParsedCommandNode(std::shared_ptr<CommandNode<S>> node, StringRange range) : node_(std::move(node)), range_(range)
    {
    }

    [[nodiscard]] const std::shared_ptr<CommandNode<S>> &getNode() const { return node_; }
    [[nodiscard]] const StringRange &getRange() const { return range_; }

    bool operator==(const ParsedCommandNode &) const = default;

private:
    std::shared_ptr<CommandNode<S>> node_;
    StringRange range_;
};

/**
 * What a command receives: the source that ran it and the arguments parsed from its input.
 *
 * @see com.mojang.brigadier.context.CommandContext
 */
template <typename S>
class CommandContext {
public:
    CommandContext(S source, std::string input, std::unordered_map<std::string, ParsedArgument> arguments,
                   Command<S> command, std::shared_ptr<CommandNode<S>> root_node,
                   std::vector<ParsedCommandNode<S>> nodes, StringRange range, std::shared_ptr<CommandContext> child,
                   RedirectModifier<S> modifier, bool forks)
        : source_(std::move(source)), input_(std::move(input)), arguments_(std::move(arguments)),
          command_(std::move(command)), root_node_(std::move(root_node)), nodes_(std::move(nodes)), range_(range),
          child_(std::move(child)), modifier_(std::move(modifier)), forks_(forks)
    {
    }

    [[nodiscard]] CommandContext copyFor(S source) const
    {
        return {std::move(source), input_, arguments_, command_, root_node_, nodes_, range_, child_, modifier_, forks_};
    }

    [[nodiscard]] const std::shared_ptr<CommandContext> &getChild() const { return child_; }

    [[nodiscard]] const CommandContext &getLastChild() const
    {
        const CommandContext *result = this;
        while (result->getChild()) {
            result = result->getChild().get();
        }
        return *result;
    }

    [[nodiscard]] const Command<S> &getCommand() const { return command_; }
    [[nodiscard]] const S &getSource() const { return source_; }

    template <typename V>
    [[nodiscard]] const V &getArgument(const std::string &name) const
    {
        const auto argument = arguments_.find(name);
        if (argument == arguments_.end()) {
            throw std::invalid_argument("No such argument '" + name + "' exists on this command");
        }
        if (const auto *result = std::get_if<V>(&argument->second.getResult())) {
            return *result;
        }
        std::string message = "Argument '" + name + "' is defined as ";
        message += ARGUMENT_TYPE_NAMES[argument->second.getResult().index()];
        message += ", not ";
        constexpr auto expected = []<std::size_t... I>(std::index_sequence<I...>) {
            return ((std::is_same_v<V, std::variant_alternative_t<I, ArgumentValue>> ? I : 0) + ...);
        }(std::make_index_sequence<std::variant_size_v<ArgumentValue>>{});
        message += ARGUMENT_TYPE_NAMES[expected];
        throw std::invalid_argument(message);
    }

    [[nodiscard]] const std::unordered_map<std::string, ParsedArgument> &getArguments() const { return arguments_; }
    [[nodiscard]] const RedirectModifier<S> &getRedirectModifier() const { return modifier_; }
    [[nodiscard]] const StringRange &getRange() const { return range_; }
    [[nodiscard]] const std::string &getInput() const { return input_; }
    [[nodiscard]] const std::shared_ptr<CommandNode<S>> &getRootNode() const { return root_node_; }
    [[nodiscard]] const std::vector<ParsedCommandNode<S>> &getNodes() const { return nodes_; }
    [[nodiscard]] bool hasNodes() const { return !nodes_.empty(); }
    [[nodiscard]] bool isForked() const { return forks_; }

private:
    static constexpr std::string_view ARGUMENT_TYPE_NAMES[] = {"bool",
                                                               "int",
                                                               "long",
                                                               "float",
                                                               "double",
                                                               "string",
                                                               "PlayerSelectorArgumentResolver",
                                                               "ActorSelectorArgumentResolver",
                                                               "BlockPositionResolver",
                                                               "FinePositionResolver",
                                                               "Dimension",
                                                               "BlockType",
                                                               "ItemType",
                                                               "ActorType"};

    S source_;
    std::string input_;
    std::unordered_map<std::string, ParsedArgument> arguments_;
    Command<S> command_;
    std::shared_ptr<CommandNode<S>> root_node_;
    std::vector<ParsedCommandNode<S>> nodes_;
    StringRange range_;
    std::shared_ptr<CommandContext> child_;
    RedirectModifier<S> modifier_;
    bool forks_;
};

/**
 * The commands of a redirected command, flattened into the modifiers that run first and the command that runs last.
 *
 * @see com.mojang.brigadier.context.ContextChain
 */
template <typename S>
class ContextChain {
public:
    enum class Stage {
        Modify,
        Execute,
    };

    ContextChain(std::vector<CommandContext<S>> modifiers, CommandContext<S> executable)
        : modifiers_(std::move(modifiers)), executable_(std::move(executable))
    {
        if (!executable_.getCommand()) {
            throw std::invalid_argument("Last command in chain must be executable");
        }
    }

    static std::optional<ContextChain> tryFlatten(const CommandContext<S> &root_context)
    {
        std::vector<CommandContext<S>> modifiers;
        const CommandContext<S> *current = &root_context;
        while (true) {
            const CommandContext<S> *child = current->getChild().get();
            if (child == nullptr) {
                if (!current->getCommand()) {
                    return std::nullopt;
                }
                return ContextChain(std::move(modifiers), *current);
            }
            modifiers.push_back(*current);
            current = child;
        }
    }

    static std::vector<S> runModifier(const CommandContext<S> &modifier, const S &source,
                                      const ResultConsumer<S> &result_consumer, bool forked_mode)
    {
        const RedirectModifier<S> &source_modifier = modifier.getRedirectModifier();
        if (!source_modifier) {
            return {source};
        }
        const CommandContext<S> context_to_use = modifier.copyFor(source);
        try {
            return source_modifier(context_to_use);
        }
        catch (const CommandSyntaxException &) {
            result_consumer(context_to_use, false, 0);
            if (forked_mode) {
                return {};
            }
            throw;
        }
    }

    static int runExecutable(const CommandContext<S> &executable, const S &source,
                             const ResultConsumer<S> &result_consumer, bool forked_mode)
    {
        const CommandContext<S> context_to_use = executable.copyFor(source);
        try {
            const int result = executable.getCommand()(context_to_use);
            result_consumer(context_to_use, true, result);
            return forked_mode ? 1 : result;
        }
        catch (const CommandSyntaxException &) {
            result_consumer(context_to_use, false, 0);
            if (forked_mode) {
                return 0;
            }
            throw;
        }
    }

    int executeAll(const S &source, const ResultConsumer<S> &result_consumer) const
    {
        if (modifiers_.empty()) {
            return runExecutable(executable_, source, result_consumer, false);
        }

        bool forked_mode = false;
        std::vector<S> current_sources{source};
        for (const CommandContext<S> &modifier : modifiers_) {
            forked_mode |= modifier.isForked();
            std::vector<S> next_sources;
            for (const S &source_to_run : current_sources) {
                std::vector<S> sources = runModifier(modifier, source_to_run, result_consumer, forked_mode);
                next_sources.insert(next_sources.end(), std::make_move_iterator(sources.begin()),
                                    std::make_move_iterator(sources.end()));
            }
            if (next_sources.empty()) {
                return 0;
            }
            current_sources = std::move(next_sources);
        }

        int result = 0;
        for (const S &execution_source : current_sources) {
            result += runExecutable(executable_, execution_source, result_consumer, forked_mode);
        }
        return result;
    }

    [[nodiscard]] Stage getStage() const { return modifiers_.empty() ? Stage::Execute : Stage::Modify; }

    [[nodiscard]] const CommandContext<S> &getTopContext() const
    {
        if (modifiers_.empty()) {
            return executable_;
        }
        return modifiers_.front();
    }

    [[nodiscard]] std::shared_ptr<ContextChain> nextStage() const
    {
        if (modifiers_.empty()) {
            return nullptr;
        }
        if (!next_stage_cache_) {
            next_stage_cache_ = std::make_shared<ContextChain>(
                std::vector<CommandContext<S>>(modifiers_.begin() + 1, modifiers_.end()), executable_);
        }
        return next_stage_cache_;
    }

private:
    std::vector<CommandContext<S>> modifiers_;
    CommandContext<S> executable_;
    mutable std::shared_ptr<ContextChain> next_stage_cache_;
};

template <typename S>
class CommandContextBuilder;

/**
 * @see com.mojang.brigadier.context.SuggestionContext
 */
template <typename S>
struct SuggestionContext {
    const CommandContextBuilder<S> &context;
    std::shared_ptr<CommandNode<S>> parent;
    int start_pos;
};

/**
 * The context a parse builds up node by node, which becomes the CommandContext a command runs with.
 *
 * @see com.mojang.brigadier.context.CommandContextBuilder
 */
template <typename S>
class CommandContextBuilder {
public:
    CommandContextBuilder(CommandDispatcher<S> &dispatcher, S source, std::shared_ptr<CommandNode<S>> root_node,
                          int start)
        : root_node_(std::move(root_node)), dispatcher_(&dispatcher), source_(std::move(source)),
          range_(StringRange::at(start))
    {
    }

    CommandContextBuilder &withSource(S source)
    {
        source_ = std::move(source);
        return *this;
    }

    [[nodiscard]] const S &getSource() const { return source_; }
    [[nodiscard]] const std::shared_ptr<CommandNode<S>> &getRootNode() const { return root_node_; }

    CommandContextBuilder &withArgument(const std::string &name, ParsedArgument argument)
    {
        arguments_.insert_or_assign(name, std::move(argument));
        return *this;
    }

    [[nodiscard]] const std::unordered_map<std::string, ParsedArgument> &getArguments() const { return arguments_; }

    CommandContextBuilder &withCommand(Command<S> command)
    {
        command_ = std::move(command);
        return *this;
    }

    CommandContextBuilder &withNode(const std::shared_ptr<CommandNode<S>> &node, const StringRange &range);

    [[nodiscard]] CommandContextBuilder copy() const
    {
        CommandContextBuilder copy(*dispatcher_, source_, root_node_, range_.getStart());
        copy.command_ = command_;
        copy.arguments_ = arguments_;
        copy.nodes_ = nodes_;
        copy.child_ = child_;
        copy.range_ = range_;
        copy.forks_ = forks_;
        return copy;
    }

    CommandContextBuilder &withChild(CommandContextBuilder child)
    {
        child_ = std::make_shared<CommandContextBuilder>(std::move(child));
        return *this;
    }

    [[nodiscard]] const std::shared_ptr<CommandContextBuilder> &getChild() const { return child_; }

    [[nodiscard]] const CommandContextBuilder &getLastChild() const
    {
        const CommandContextBuilder *result = this;
        while (result->getChild()) {
            result = result->getChild().get();
        }
        return *result;
    }

    [[nodiscard]] const Command<S> &getCommand() const { return command_; }
    [[nodiscard]] const std::vector<ParsedCommandNode<S>> &getNodes() const { return nodes_; }

    [[nodiscard]] CommandContext<S> build(const std::string &input) const
    {
        return {source_,    input,
                arguments_, command_,
                root_node_, nodes_,
                range_,     child_ ? std::make_shared<CommandContext<S>>(child_->build(input)) : nullptr,
                modifier_,  forks_};
    }

    [[nodiscard]] CommandDispatcher<S> &getDispatcher() const { return *dispatcher_; }
    [[nodiscard]] const StringRange &getRange() const { return range_; }

    [[nodiscard]] SuggestionContext<S> findSuggestionContext(int cursor) const
    {
        if (range_.getStart() <= cursor) {
            if (range_.getEnd() < cursor) {
                if (child_) {
                    return child_->findSuggestionContext(cursor);
                }
                if (!nodes_.empty()) {
                    const ParsedCommandNode<S> &last = nodes_.back();
                    return {*this, last.getNode(), last.getRange().getEnd() + 1};
                }
                return {*this, root_node_, range_.getStart()};
            }
            std::shared_ptr<CommandNode<S>> prev = root_node_;
            for (const ParsedCommandNode<S> &node : nodes_) {
                const StringRange &node_range = node.getRange();
                if (node_range.getStart() <= cursor && cursor <= node_range.getEnd()) {
                    return {*this, prev, node_range.getStart()};
                }
                prev = node.getNode();
            }
            if (!prev) {
                throw std::logic_error("Can't find node before cursor");
            }
            return {*this, prev, range_.getStart()};
        }
        throw std::logic_error("Can't find node before cursor");
    }

private:
    std::unordered_map<std::string, ParsedArgument> arguments_;
    std::shared_ptr<CommandNode<S>> root_node_;
    std::vector<ParsedCommandNode<S>> nodes_;
    CommandDispatcher<S> *dispatcher_;
    S source_;
    Command<S> command_;
    std::shared_ptr<CommandContextBuilder> child_;
    StringRange range_;
    RedirectModifier<S> modifier_;
    bool forks_ = false;
};

}  // namespace endstone::brigadier
