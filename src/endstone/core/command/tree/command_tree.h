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

#include <string>
#include <vector>

#include <nonstd/expected.hpp>

#include "endstone/command/command_node.h"

namespace endstone::core {

/**
 * One parameter of a flattened overload.
 */
struct CommandTreeSlot {
    NotNull<CommandNode> node;
    bool is_literal;
};

/**
 * One root-to-leaf path of a command tree, as a flat parameter list.
 */
struct CommandTreeOverload {
    std::vector<CommandTreeSlot> slots;
    std::vector<NotNull<CommandNode>> path;
    NotNull<CommandNode> leaf;
};

/**
 * The maximum number of overloads a single command tree may flatten to.
 *
 * The full command list is serialized and sent to every player, so a tree that fans out without
 * bound would cost every client real bandwidth.
 */
constexpr std::size_t MAX_OVERLOADS_PER_COMMAND = 64;

/**
 * Flattens a command tree into the overloads the command registry accepts.
 *
 * Returns an error message describing the offending branch if the tree cannot be represented.
 */
nonstd::expected<std::vector<CommandTreeOverload>, std::string> flattenCommandTree(const NotNull<CommandNode> &root);

}  // namespace endstone::core
