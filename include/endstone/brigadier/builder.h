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

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "endstone/brigadier/arguments.h"
#include "endstone/brigadier/context.h"
#include "endstone/brigadier/suggestion.h"
#include "endstone/brigadier/tree.h"

namespace endstone::brigadier {

/**
 * Builds a node and the nodes below it, the way a command is declared.
 *
 * @see com.mojang.brigadier.builder.ArgumentBuilder
 */
template <typename S, typename T>
class ArgumentBuilder {
public:
    template <typename U>
    T &then(const ArgumentBuilder<S, U> &argument)
    {
        if (target_ != nullptr) {
            throw std::logic_error("Cannot add children to a redirected node");
        }
        arguments_->addChild(static_cast<const U &>(argument).build());
        return getThis();
    }

    T &then(const std::shared_ptr<CommandNode<S>> &argument)
    {
        if (target_ != nullptr) {
            throw std::logic_error("Cannot add children to a redirected node");
        }
        arguments_->addChild(argument);
        return getThis();
    }

    [[nodiscard]] const std::vector<std::shared_ptr<CommandNode<S>>> &getArguments() const
    {
        return arguments_->getChildren();
    }

    T &executes(Command<S> command)
    {
        command_ = std::move(command);
        return getThis();
    }

    [[nodiscard]] const Command<S> &getCommand() const { return command_; }

    T &requires_(Predicate<S> requirement)
    {
        requirement_ = std::move(requirement);
        return getThis();
    }

    [[nodiscard]] const Predicate<S> &getRequirement() const { return requirement_; }

    T &redirect(const std::shared_ptr<CommandNode<S>> &target) { return forward(target, {}, false); }

    T &redirect(const std::shared_ptr<CommandNode<S>> &target, SingleRedirectModifier<S> modifier)
    {
        if (!modifier) {
            return forward(target, {}, false);
        }
        return forward(
            target,
            [modifier = std::move(modifier)](const CommandContext<S> &context) {
                return std::vector<S>{modifier(context)};
            },
            false);
    }

    T &fork(const std::shared_ptr<CommandNode<S>> &target, RedirectModifier<S> modifier)
    {
        return forward(target, std::move(modifier), true);
    }

    T &forward(const std::shared_ptr<CommandNode<S>> &target, RedirectModifier<S> modifier, bool fork)
    {
        if (!arguments_->getChildren().empty()) {
            throw std::logic_error("Cannot forward a node with children");
        }
        target_ = target;
        modifier_ = std::move(modifier);
        forks_ = fork;
        return getThis();
    }

    [[nodiscard]] CommandNode<S> *getRedirect() const { return target_.get(); }
    [[nodiscard]] const std::shared_ptr<CommandNode<S>> &getRedirectTarget() const { return target_; }
    [[nodiscard]] const RedirectModifier<S> &getRedirectModifier() const { return modifier_; }
    [[nodiscard]] bool isFork() const { return forks_; }

protected:
    T &getThis() { return static_cast<T &>(*this); }

private:
    std::shared_ptr<RootCommandNode<S>> arguments_ = std::make_shared<RootCommandNode<S>>();
    Command<S> command_;
    Predicate<S> requirement_ = [](const S &) {
        return true;
    };
    std::shared_ptr<CommandNode<S>> target_;
    RedirectModifier<S> modifier_;
    bool forks_ = false;
};

/**
 * @see com.mojang.brigadier.builder.LiteralArgumentBuilder
 */
template <typename S>
class LiteralArgumentBuilder : public ArgumentBuilder<S, LiteralArgumentBuilder<S>> {
public:
    static LiteralArgumentBuilder literal(const std::string &name) { return LiteralArgumentBuilder(name); }

    [[nodiscard]] const std::string &getLiteral() const { return literal_; }

    [[nodiscard]] std::shared_ptr<LiteralCommandNode<S>> build() const
    {
        auto result = std::make_shared<LiteralCommandNode<S>>(getLiteral(), this->getCommand(), this->getRequirement(),
                                                              this->getRedirectTarget(), this->getRedirectModifier(),
                                                              this->isFork());
        for (const std::shared_ptr<CommandNode<S>> &argument : this->getArguments()) {
            result->addChild(argument);
        }
        return result;
    }

protected:
    explicit LiteralArgumentBuilder(std::string literal) : literal_(std::move(literal)) {}

private:
    std::string literal_;
};

/**
 * @see com.mojang.brigadier.builder.RequiredArgumentBuilder
 */
template <typename S>
class RequiredArgumentBuilder : public ArgumentBuilder<S, RequiredArgumentBuilder<S>> {
public:
    static RequiredArgumentBuilder argument(const std::string &name, std::shared_ptr<const ArgumentType> type)
    {
        return RequiredArgumentBuilder(name, std::move(type));
    }

    RequiredArgumentBuilder &suggests(SuggestionProvider<S> provider)
    {
        suggestions_provider_ = std::move(provider);
        return *this;
    }

    [[nodiscard]] const SuggestionProvider<S> &getSuggestionsProvider() const { return suggestions_provider_; }
    [[nodiscard]] const std::shared_ptr<const ArgumentType> &getType() const { return type_; }
    [[nodiscard]] const std::string &getName() const { return name_; }

    [[nodiscard]] std::shared_ptr<ArgumentCommandNode<S>> build() const
    {
        auto result = std::make_shared<ArgumentCommandNode<S>>(
            getName(), getType(), this->getCommand(), this->getRequirement(), this->getRedirectTarget(),
            this->getRedirectModifier(), this->isFork(), getSuggestionsProvider());
        for (const std::shared_ptr<CommandNode<S>> &argument : this->getArguments()) {
            result->addChild(argument);
        }
        return result;
    }

private:
    RequiredArgumentBuilder(std::string name, std::shared_ptr<const ArgumentType> type)
        : name_(std::move(name)), type_(std::move(type))
    {
    }

    std::string name_;
    std::shared_ptr<const ArgumentType> type_;
    SuggestionProvider<S> suggestions_provider_;
};

}  // namespace endstone::brigadier
