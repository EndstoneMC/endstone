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

#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "bedrock/server/commands/command.h"
#include "bedrock/server/commands/command_registry.h"
#include "endstone/command/command.h"
#include "endstone/command/command_node.h"
#include "endstone/core/command/tree/command_tree.h"

namespace endstone::core {

class TreeCommandAdapter;

/**
 * Type-erased storage for one parsed parameter.
 *
 * A command tree is shaped at runtime, so its parameters cannot live at fixed offsets in a struct
 * the way a vanilla command's do. The command registry supports exactly this through
 * CommandParameterData's custom storage accessors, which is how Mojang's own script command layer
 * works.
 */
class ArgumentStorage {
public:
    virtual ~ArgumentStorage() = default;
    [[nodiscard]] virtual void *data() = 0;
};

template <typename T>
class TypedArgumentStorage : public ArgumentStorage {
public:
    [[nodiscard]] void *data() override { return &value_; }
    [[nodiscard]] const T &get() const { return value_; }

private:
    T value_{};
};

/**
 * The Command a registered command tree is exposed as.
 *
 * Owns the tree, the flattened overloads the adapters point back into, and the enum name strings
 * the command registry borrows.
 */
class TreeCommand : public Command {
public:
    TreeCommand(NotNull<LiteralCommandNode> root, std::vector<CommandTreeOverload> overloads);

    [[nodiscard]] bool execute(const NotNull<CommandSender> &sender,
                               const std::vector<std::string> &args) const override;

    [[nodiscard]] const NotNull<LiteralCommandNode> &getRoot() const { return root_; }
    [[nodiscard]] const std::vector<CommandTreeOverload> &getOverloads() const { return overloads_; }

    /**
     * Runs the branch an adapter matched.
     *
     * @param overload the overload that parsed
     * @param sender source of the command
     * @param slots the parsed value of each slot, in slot order
     * @return true if the command reported success
     */
    [[nodiscard]] bool run(const CommandTreeOverload &overload, const NotNull<CommandSender> &sender,
                           const std::vector<std::unique_ptr<ArgumentStorage>> &slots) const;

    /**
     * Tests every node on an overload's path against a sender.
     *
     * @param overload the overload to test
     * @param sender the sender to test
     * @return true if the sender may use the whole branch
     */
    [[nodiscard]] static bool testOverloadSilently(const CommandTreeOverload &overload,
                                                   const NotNull<CommandSender> &sender);

    /** Keeps an enum name alive for as long as the command registry borrows it. */
    const char *intern(const std::string &name);

private:
    NotNull<LiteralCommandNode> root_;
    std::vector<CommandTreeOverload> overloads_;
    std::deque<std::string> interned_;
};

/**
 * The bedrock Command a flattened overload allocates.
 *
 * Each overload binds its own adapter through a capturing allocator, so an adapter always knows
 * which branch of the tree parsed.
 */
class TreeCommandAdapter : public ::Command {
public:
    TreeCommandAdapter(const TreeCommand &command, const CommandTreeOverload &overload);

    /** Hands the command registry the storage for one parameter. */
    static void *getStorageValue(::Command *command, int index);

    void execute(const CommandOrigin &origin, CommandOutput &output) const override;

    /**
     * Runs the branch this adapter parsed against a sender directly.
     *
     * Used by CommandMap::dispatch, which compiles the command line itself rather than letting
     * the command registry run it.
     *
     * @param sender source of the command
     * @return true if the command reported success
     */
    [[nodiscard]] bool runFrom(const NotNull<CommandSender> &sender) const;

private:
    friend class ::CommandRegistry;

    const TreeCommand *command_;
    const CommandTreeOverload *overload_;
    std::vector<std::unique_ptr<ArgumentStorage>> slots_;
};

}  // namespace endstone::core

template <>
const CommandRegistry::ParamParseRule CommandRegistry::ParseRuleFor<endstone::core::TreeCommandAdapter>::instance;
