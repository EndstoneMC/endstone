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
#include <concepts>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include "endstone/brigadier/arguments.h"
#include "endstone/brigadier/context.h"
#include "endstone/brigadier/exceptions.h"
#include "endstone/brigadier/reader.h"
#include "endstone/brigadier/suggestion.h"

namespace endstone::brigadier {

template <typename S>
class LiteralArgumentBuilder;

template <typename S>
class RequiredArgumentBuilder;

template <typename S>
class RootCommandNode;

template <typename S>
class LiteralCommandNode;

template <typename S>
class ArgumentCommandNode;

template <typename S>
using Predicate = std::function<bool(const S &)>;

/**
 * A node of the command tree, which parses a part of the input and holds the command that runs at it.
 *
 * @see com.mojang.brigadier.tree.CommandNode
 */
template <typename S>
class CommandNode {
public:
    CommandNode(const CommandNode &) = delete;
    CommandNode &operator=(const CommandNode &) = delete;
    virtual ~CommandNode() = default;

    [[nodiscard]] const Command<S> &getCommand() const { return command_; }
    [[nodiscard]] const std::vector<std::shared_ptr<CommandNode>> &getChildren() const { return children_; }
    [[nodiscard]] CommandNode *getRedirect() const { return redirect_.lock().get(); }
    [[nodiscard]] std::shared_ptr<CommandNode> getRedirectTarget() const { return redirect_.lock(); }
    [[nodiscard]] const RedirectModifier<S> &getRedirectModifier() const { return modifier_; }
    [[nodiscard]] bool isFork() const { return forks_; }

    [[nodiscard]] std::shared_ptr<CommandNode> getChild(const std::string &name) const
    {
        const auto child = child_by_name_.find(name);
        return child == child_by_name_.end() ? nullptr : child->second;
    }

    [[nodiscard]] bool canUse(const S &source) const { return requirement_(source); }

    void addChild(const std::shared_ptr<CommandNode> &node)
    {
        if (node->template is<RootCommandNode<S>>()) {
            throw std::logic_error("Cannot add a RootCommandNode as a child to any other CommandNode");
        }
        if (const std::shared_ptr<CommandNode> child = getChild(node->getName())) {
            if (node->getCommand()) {
                child->command_ = node->getCommand();
            }
            for (const std::shared_ptr<CommandNode> &grandchild : node->getChildren()) {
                child->addChild(grandchild);
            }
        }
        else {
            children_.push_back(node);
            child_by_name_.emplace(node->getName(), node);
            if (auto *literal = node->template as<LiteralCommandNode<S>>()) {
                literals_.emplace(node->getName(), std::shared_ptr<LiteralCommandNode<S>>(node, literal));
            }
            else if (node->template is<ArgumentCommandNode<S>>()) {
                arguments_.push_back(node);
            }
        }
    }

    void removeChildByName(const std::string &name)
    {
        std::erase_if(children_, [&](const std::shared_ptr<CommandNode> &child) { return child->getName() == name; });
        child_by_name_.erase(name);
        literals_.erase(name);
        std::erase_if(arguments_, [&](const std::shared_ptr<CommandNode> &child) { return child->getName() == name; });
    }

    [[nodiscard]] virtual bool isValidInput(const std::string &input) const = 0;

    [[nodiscard]] const Predicate<S> &getRequirement() const { return requirement_; }
    [[nodiscard]] virtual std::string getName() const = 0;
    [[nodiscard]] virtual std::string getUsageText() const = 0;
    virtual void parse(StringReader &reader, CommandContextBuilder<S> &context_builder) = 0;
    [[nodiscard]] virtual Suggestions listSuggestions(const CommandContext<S> &context,
                                                      SuggestionsBuilder &builder) const = 0;
    [[nodiscard]] virtual std::vector<std::string> getExamples() const = 0;

    [[nodiscard]] std::vector<std::shared_ptr<CommandNode>> getRelevantNodes(StringReader &input) const
    {
        if (!literals_.empty()) {
            const int cursor = input.getCursor();
            while (input.canRead() && input.peek() != ' ') {
                input.skip();
            }
            const std::string text = input.getString().substr(cursor, input.getCursor() - cursor);
            input.setCursor(cursor);
            const auto literal = literals_.find(text);
            if (literal != literals_.end()) {
                return {literal->second};
            }
        }
        return arguments_;
    }

    [[nodiscard]] virtual bool isInstanceOf(const std::type_info &target) const
    {
        return std::string_view(target.name()) == typeid(CommandNode).name();
    }

    template <std::derived_from<CommandNode> T>
    [[nodiscard]] T *as()
    {
        return is<T>() ? static_cast<T *>(this) : nullptr;
    }

    template <std::derived_from<CommandNode> T>
    [[nodiscard]] const T *as() const
    {
        return is<T>() ? static_cast<const T *>(this) : nullptr;
    }

    template <std::derived_from<CommandNode> T>
    [[nodiscard]] bool is() const
    {
        return isInstanceOf(typeid(T));
    }

    [[nodiscard]] int compareTo(const CommandNode &o) const
    {
        const bool literal = is<LiteralCommandNode<S>>();
        const bool other_literal = o.template is<LiteralCommandNode<S>>();
        if (literal == other_literal) {
            return getSortedKey().compare(o.getSortedKey());
        }
        return other_literal ? 1 : -1;
    }

protected:
    CommandNode(Command<S> command, Predicate<S> requirement, const std::shared_ptr<CommandNode> &redirect,
                RedirectModifier<S> modifier, bool forks)
        : requirement_(std::move(requirement)), redirect_(redirect), modifier_(std::move(modifier)), forks_(forks),
          command_(std::move(command))
    {
    }

    [[nodiscard]] virtual std::string getSortedKey() const = 0;

private:
    std::vector<std::shared_ptr<CommandNode>> children_;
    std::unordered_map<std::string, std::shared_ptr<CommandNode>> child_by_name_;
    std::unordered_map<std::string, std::shared_ptr<LiteralCommandNode<S>>> literals_;
    std::vector<std::shared_ptr<CommandNode>> arguments_;
    Predicate<S> requirement_;
    std::weak_ptr<CommandNode> redirect_;
    RedirectModifier<S> modifier_;
    bool forks_;
    Command<S> command_;
};

/**
 * @see com.mojang.brigadier.tree.RootCommandNode
 */
template <typename S>
class RootCommandNode : public CommandNode<S> {
public:
    RootCommandNode() : CommandNode<S>({}, [](const S &) { return true; }, nullptr, {}, false) {}

    [[nodiscard]] std::string getName() const override { return ""; }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(RootCommandNode).name() ||
               CommandNode<S>::isInstanceOf(target);
    }

    [[nodiscard]] std::string getUsageText() const override { return ""; }
    void parse(StringReader &, CommandContextBuilder<S> &) override {}
    [[nodiscard]] Suggestions listSuggestions(const CommandContext<S> &, SuggestionsBuilder &) const override
    {
        return Suggestions::empty();
    }
    [[nodiscard]] bool isValidInput(const std::string &) const override { return false; }
    [[nodiscard]] std::vector<std::string> getExamples() const override { return {}; }
    [[nodiscard]] std::string toString() const { return "<root>"; }

protected:
    [[nodiscard]] std::string getSortedKey() const override { return ""; }
};

/**
 * @see com.mojang.brigadier.tree.LiteralCommandNode
 */
template <typename S>
class LiteralCommandNode : public CommandNode<S> {
public:
    LiteralCommandNode(std::string literal, Command<S> command, Predicate<S> requirement,
                       const std::shared_ptr<CommandNode<S>> &redirect, RedirectModifier<S> modifier, bool forks)
        : CommandNode<S>(std::move(command), std::move(requirement), redirect, std::move(modifier), forks),
          literal_(std::move(literal)), literal_lower_case_(SuggestionsBuilder::toLowerCase(literal_))
    {
    }

    [[nodiscard]] const std::string &getLiteral() const { return literal_; }
    [[nodiscard]] std::string getName() const override { return literal_; }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(LiteralCommandNode).name() ||
               CommandNode<S>::isInstanceOf(target);
    }

    void parse(StringReader &reader, CommandContextBuilder<S> &) override
    {
        if (parse(reader) > -1) {
            return;
        }
        throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.literalIncorrect().createWithContext(reader, literal_);
    }

    [[nodiscard]] Suggestions listSuggestions(const CommandContext<S> &, SuggestionsBuilder &builder) const override
    {
        if (literal_lower_case_.starts_with(builder.getRemainingLowerCase())) {
            return builder.suggest(literal_).build();
        }
        return Suggestions::empty();
    }

    [[nodiscard]] bool isValidInput(const std::string &input) const override
    {
        StringReader reader(input);
        return parse(reader) > -1;
    }

    [[nodiscard]] std::string getUsageText() const override { return literal_; }

    [[nodiscard]] LiteralArgumentBuilder<S> createBuilder() const
    {
        LiteralArgumentBuilder<S> builder = LiteralArgumentBuilder<S>::literal(literal_);
        builder.requires_(this->getRequirement());
        builder.forward(this->getRedirectTarget(), this->getRedirectModifier(), this->isFork());
        if (this->getCommand()) {
            builder.executes(this->getCommand());
        }
        return builder;
    }

    [[nodiscard]] std::vector<std::string> getExamples() const override { return {literal_}; }
    [[nodiscard]] std::string toString() const { return "<literal " + literal_ + ">"; }

protected:
    [[nodiscard]] std::string getSortedKey() const override { return literal_; }

private:
    [[nodiscard]] int parse(StringReader &reader) const
    {
        const int start = reader.getCursor();
        if (reader.canRead(static_cast<int>(literal_.length()))) {
            const int end = start + static_cast<int>(literal_.length());
            if (reader.getString().compare(start, literal_.length(), literal_) == 0) {
                reader.setCursor(end);
                if (!reader.canRead() || reader.peek() == ' ') {
                    return end;
                }
                reader.setCursor(start);
            }
        }
        return -1;
    }

    std::string literal_;
    std::string literal_lower_case_;
};

/**
 * @see com.mojang.brigadier.tree.ArgumentCommandNode
 */
template <typename S>
class ArgumentCommandNode : public CommandNode<S> {
public:
    ArgumentCommandNode(std::string name, std::shared_ptr<const ArgumentType> type, Command<S> command,
                        Predicate<S> requirement, const std::shared_ptr<CommandNode<S>> &redirect,
                        RedirectModifier<S> modifier, bool forks, SuggestionProvider<S> custom_suggestions)
        : CommandNode<S>(std::move(command), std::move(requirement), redirect, std::move(modifier), forks),
          name_(std::move(name)), type_(std::move(type)), custom_suggestions_(std::move(custom_suggestions))
    {
    }

    [[nodiscard]] const std::shared_ptr<const ArgumentType> &getType() const { return type_; }
    [[nodiscard]] std::string getName() const override { return name_; }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(ArgumentCommandNode).name() ||
               CommandNode<S>::isInstanceOf(target);
    }

    [[nodiscard]] std::string getUsageText() const override
    {
        return USAGE_ARGUMENT_OPEN + name_ + USAGE_ARGUMENT_CLOSE;
    }
    [[nodiscard]] const SuggestionProvider<S> &getCustomSuggestions() const { return custom_suggestions_; }

    void parse(StringReader &reader, CommandContextBuilder<S> &context_builder) override
    {
        const int start = reader.getCursor();
        ArgumentValue result = type_->parse(reader);
        const ParsedArgument parsed(start, reader.getCursor(), std::move(result));
        context_builder.withArgument(name_, parsed);
    }

    [[nodiscard]] Suggestions listSuggestions(const CommandContext<S> &context,
                                              SuggestionsBuilder &builder) const override
    {
        if (!custom_suggestions_) {
            return Suggestions::empty();
        }
        return custom_suggestions_(context, builder);
    }

    [[nodiscard]] RequiredArgumentBuilder<S> createBuilder() const
    {
        RequiredArgumentBuilder<S> builder = RequiredArgumentBuilder<S>::argument(name_, type_);
        builder.requires_(this->getRequirement());
        builder.forward(this->getRedirectTarget(), this->getRedirectModifier(), this->isFork());
        builder.suggests(custom_suggestions_);
        if (this->getCommand()) {
            builder.executes(this->getCommand());
        }
        return builder;
    }

    [[nodiscard]] bool isValidInput(const std::string &input) const override
    {
        try {
            StringReader reader(input);
            std::ignore = type_->parse(reader);
            return !reader.canRead() || reader.peek() == ' ';
        }
        catch (const CommandSyntaxException &) {
            return false;
        }
    }

    [[nodiscard]] std::vector<std::string> getExamples() const override { return type_->getExamples(); }
    [[nodiscard]] std::string toString() const { return "<argument " + name_ + ":" + type_->toString() + ">"; }

protected:
    [[nodiscard]] std::string getSortedKey() const override { return name_; }

private:
    static constexpr const char *USAGE_ARGUMENT_OPEN = "<";
    static constexpr const char *USAGE_ARGUMENT_CLOSE = ">";

    std::string name_;
    std::shared_ptr<const ArgumentType> type_;
    SuggestionProvider<S> custom_suggestions_;
};

template <typename S>
CommandContextBuilder<S> &CommandContextBuilder<S>::withNode(const std::shared_ptr<CommandNode<S>> &node,
                                                             const StringRange &range)
{
    nodes_.emplace_back(node, range);
    range_ = StringRange::encompassing(range_, range);
    modifier_ = node->getRedirectModifier();
    forks_ = node->isFork();
    return *this;
}

}  // namespace endstone::brigadier
