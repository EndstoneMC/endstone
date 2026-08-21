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
#include <string>
#include <utility>

#include "endstone/command/argument_type.h"
#include "endstone/command/command_map.h"
#include "endstone/command/command_node.h"
#include "endstone/util/pointers.h"

namespace endstone {

class Plugin;

/**
 * Shared surface for building a node of a command tree.
 *
 * A builder is a handle onto the node it is building, so copying one and continuing to chain on
 * either copy shapes the same node.
 */
template <typename Derived, typename NodeType>
class ArgumentBuilder {
public:
    /**
     * Returns the node this builder has shaped.
     *
     * @return The built node
     */
    [[nodiscard]] const NotNull<NodeType> &build() const { return node_; }

    // NOLINTNEXTLINE(*-explicit-constructor)
    operator NotNull<CommandNode>() const { return node_; }

    /**
     * Adds a child branch.
     *
     * @param child Branch to add
     * @return This builder
     */
    Derived then(const NotNull<CommandNode> &child)
    {
        node_->addChild(child);
        return self();
    }

    /**
     * Sets the handler run when a command ends at this node.
     *
     * Returning normally reports the command as successful; throw a CommandError to report a
     * failure to the sender.
     *
     * @param handler Handler to run
     * @return This builder
     */
    Derived executes(CommandHandler handler)
    {
        node_->setHandler(std::move(handler));
        return self();
    }

    /**
     * Requires a permission to use this branch.
     *
     * A sender holding any one of the permissions added here passes. A sender without one is told
     * they lack permission, and the branch is hidden from their client.
     *
     * @param permission Permission name
     * @return This builder
     */
    Derived permission(std::string permission)
    {
        node_->addPermission(std::move(permission));
        return self();
    }

    /**
     * Requires a predicate to pass to use this branch.
     *
     * Unlike permission(), a sender that fails is not told why, and the branch is simply absent.
     * Keep the predicate cheap: it runs once per node per parse, and again for every node each
     * time the command tree is sent to a player.
     *
     * @param requirement Predicate to test
     * @return This builder
     */
    Derived requires_(CommandRequirement requirement)  // NOLINT(*-identifier-naming)
    {
        node_->addRequirement(std::move(requirement));
        return self();
    }

protected:
    explicit ArgumentBuilder(NotNull<NodeType> node) : node_(std::move(node)) {}

    [[nodiscard]] Derived self() const { return static_cast<const Derived &>(*this); }

    NotNull<NodeType> node_;
};

/**
 * Builds a node matched by typing a fixed word.
 */
class LiteralArgumentBuilder : public ArgumentBuilder<LiteralArgumentBuilder, LiteralCommandNode> {
public:
    explicit LiteralArgumentBuilder(std::string name)
        : ArgumentBuilder(std::make_shared<LiteralCommandNode>(std::move(name)))
    {
    }

    /**
     * Sets a brief description of this command.
     *
     * Only meaningful on the root of a command tree.
     *
     * @param description New command description
     * @return This builder
     */
    LiteralArgumentBuilder description(std::string description)
    {
        node_->setDescription(std::move(description));
        return *this;
    }

    /**
     * Adds alternative names this command may be typed as.
     *
     * Only meaningful on the root of a command tree.
     *
     * @param aliases Aliases to register
     * @return This builder
     */
    template <typename... Alias>
    LiteralArgumentBuilder aliases(Alias... aliases)
    {
        (node_->addAlias(std::move(aliases)), ...);
        return *this;
    }

    /**
     * Registers this command tree.
     *
     * @param command_map the CommandMap to register to
     * @param owner the plugin the command belongs to
     * @return true on success, false if a command with the same name is already registered
     */
    bool registerTo(CommandMap &command_map, Plugin &owner) const { return command_map.registerCommand(node_, owner); }
};

/**
 * Builds a node matched by parsing a value.
 */
class RequiredArgumentBuilder : public ArgumentBuilder<RequiredArgumentBuilder, ArgumentCommandNode> {
public:
    RequiredArgumentBuilder(std::string name, NotNull<ArgumentType> type)
        : ArgumentBuilder(std::make_shared<ArgumentCommandNode>(std::move(name), std::move(type)))
    {
    }
};

/**
 * Entry points for building a command tree.
 */
class Commands {
public:
    /**
     * Begins a branch matched by typing a fixed word.
     *
     * @param name Word the player types
     * @return A builder for the new node
     */
    static LiteralArgumentBuilder literal(std::string name) { return LiteralArgumentBuilder(std::move(name)); }

    /**
     * Begins a branch matched by parsing a value.
     *
     * @param name Name the value is retrieved by
     * @param type How the value is parsed
     * @return A builder for the new node
     */
    static RequiredArgumentBuilder argument(std::string name, NotNull<ArgumentType> type)
    {
        return {std::move(name), std::move(type)};
    }
};

}  // namespace endstone
