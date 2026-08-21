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
#include <cstdint>
#include <functional>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "endstone/command/argument_type.h"
#include "endstone/command/command_context.h"
#include "endstone/command/command_sender.h"
#include "endstone/util/pointers.h"

namespace endstone {

/** Invoked when a branch of a command tree is executed. */
using CommandHandler = std::function<void(const CommandContext &)>;

/** Decides whether a sender may use a branch of a command tree. */
using CommandRequirement = std::function<bool(const NotNull<CommandSender> &)>;

/** Distinguishes the kinds of node a command tree is built from. */
enum class CommandNodeType : std::uint8_t {
    Literal,
    Argument,
};

/**
 * Represents a node in a command tree.
 */
class CommandNode {
public:
    virtual ~CommandNode() = default;

    /**
     * Returns which kind of node this is.
     *
     * @return Kind of this node
     */
    [[nodiscard]] virtual CommandNodeType getType() const = 0;

    /**
     * Returns the name of this node.
     *
     * For a literal this is the word the player types; for an argument it is the name the value is
     * retrieved by.
     *
     * @return Name of this node
     */
    [[nodiscard]] const std::string &getName() const { return name_; }

    /**
     * Returns the children of this node.
     *
     * @return Child nodes, in the order they were added
     */
    [[nodiscard]] const std::vector<NotNull<CommandNode>> &getChildren() const { return children_; }

    /**
     * Returns the handler run when a command ends at this node.
     *
     * @return Handler, or an empty function if this node is not executable
     */
    [[nodiscard]] const CommandHandler &getHandler() const { return handler_; }

    /**
     * Returns whether a command may end at this node.
     *
     * @return true if this node has a handler, otherwise false
     */
    [[nodiscard]] bool isExecutable() const { return static_cast<bool>(handler_); }

    /**
     * Returns the permissions that allow this branch to be used.
     *
     * A sender holding any one of them passes. An empty list is open to everyone.
     *
     * @return List of permission names
     */
    [[nodiscard]] const std::vector<std::string> &getPermissions() const { return permissions_; }

    /**
     * Returns the predicates that must all pass for this branch to be used.
     *
     * @return List of requirements
     */
    [[nodiscard]] const std::vector<CommandRequirement> &getRequirements() const { return requirements_; }

    /**
     * Tests whether a sender may use this node.
     *
     * This checks only this node, not its ancestors.
     *
     * @param sender Sender to test
     * @return true if the sender passes this node's permissions and requirements
     */
    [[nodiscard]] bool testSilently(const NotNull<CommandSender> &sender) const
    {
        if (!permissions_.empty() &&
            std::none_of(permissions_.begin(), permissions_.end(),
                         [&sender](const auto &permission) { return sender->hasPermission(permission); })) {
            return false;
        }
        return std::all_of(requirements_.begin(), requirements_.end(),
                           [&sender](const auto &requirement) { return requirement(sender); });
    }

    /**
     * Adds a child to this node.
     *
     * If a child with the same name is already present, the two are merged: the incoming node's
     * children are grafted on, and its handler replaces the existing one if it has one.
     *
     * @param child Node to add
     */
    void addChild(NotNull<CommandNode> child);

    /** Sets the handler run when a command ends at this node. */
    void setHandler(CommandHandler handler) { handler_ = std::move(handler); }

    /** Adds a permission that allows this branch to be used. */
    void addPermission(std::string permission) { permissions_.push_back(std::move(permission)); }

    /** Adds a predicate that must pass for this branch to be used. */
    void addRequirement(CommandRequirement requirement) { requirements_.push_back(std::move(requirement)); }

protected:
    explicit CommandNode(std::string name) : name_(std::move(name)) {}

    std::string name_;
    std::vector<NotNull<CommandNode>> children_;
    CommandHandler handler_;
    std::vector<std::string> permissions_;
    std::vector<CommandRequirement> requirements_;
};

/**
 * A node matched by typing a fixed word.
 */
class LiteralCommandNode : public CommandNode {
public:
    explicit LiteralCommandNode(std::string name) : CommandNode(std::move(name)) {}

    [[nodiscard]] CommandNodeType getType() const override { return CommandNodeType::Literal; }

    /**
     * Returns a brief description of this command.
     *
     * Only meaningful on the root of a command tree.
     *
     * @return Description of this command
     */
    [[nodiscard]] const std::string &getDescription() const { return description_; }

    /**
     * Returns the alternative names this command is registered under.
     *
     * Only meaningful on the root of a command tree.
     *
     * @return List of aliases
     */
    [[nodiscard]] const std::vector<std::string> &getAliases() const { return aliases_; }

    /** Sets a brief description of this command. */
    void setDescription(std::string description) { description_ = std::move(description); }

    /** Adds an alternative name this command is registered under. */
    void addAlias(std::string alias) { aliases_.push_back(std::move(alias)); }

private:
    std::string description_;
    std::vector<std::string> aliases_;
};

/**
 * A node matched by parsing a value.
 */
class ArgumentCommandNode : public CommandNode {
public:
    ArgumentCommandNode(std::string name, NotNull<ArgumentType> type)
        : CommandNode(std::move(name)), type_(std::move(type))
    {
    }

    [[nodiscard]] CommandNodeType getType() const override { return CommandNodeType::Argument; }

    /**
     * Returns how this argument is parsed.
     *
     * @return Type of this argument
     */
    [[nodiscard]] const NotNull<ArgumentType> &getArgumentType() const { return type_; }

private:
    NotNull<ArgumentType> type_;
};

inline void CommandNode::addChild(NotNull<CommandNode> child)
{
    const auto it = std::ranges::find_if(
        children_, [&child](const auto &existing) { return existing->getName() == child->getName(); });
    if (it == children_.end()) {
        children_.push_back(std::move(child));
        return;
    }

    auto &existing = *it;
    if (child->isExecutable()) {
        existing->setHandler(child->getHandler());
    }
    for (const auto &permission : child->getPermissions()) {
        existing->addPermission(permission);
    }
    for (const auto &requirement : child->getRequirements()) {
        existing->addRequirement(requirement);
    }
    for (const auto &grandchild : child->getChildren()) {
        existing->addChild(grandchild);
    }
}

}  // namespace endstone
