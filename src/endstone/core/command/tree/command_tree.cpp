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

#include "endstone/core/command/tree/command_tree.h"

#include <algorithm>
#include <format>

namespace endstone::core {

namespace {

bool isArgument(const NotNull<CommandNode> &node)
{
    return node->getType() == CommandNodeType::Argument;
}

bool isGreedy(const NotNull<CommandNode> &node)
{
    if (!isArgument(node)) {
        return false;
    }
    const auto kind = static_cast<const ArgumentCommandNode *>(node.get().get())->getArgumentType()->getKind();
    return kind == ArgumentKind::Message || kind == ArgumentKind::RawText;
}

std::string describePath(const std::vector<NotNull<CommandNode>> &path)
{
    std::string result;
    for (const auto &node : path) {
        if (!result.empty()) {
            result += " ";
        }
        result += node->getName();
    }
    return result;
}

nonstd::expected<void, std::string> walk(const NotNull<CommandNode> &node, std::vector<NotNull<CommandNode>> &path,
                                         std::vector<CommandTreeSlot> &slots, std::vector<CommandTreeOverload> &out)
{
    if (std::ranges::any_of(path, [&node](const auto &seen) { return seen.get() == node.get(); })) {
        return nonstd::make_unexpected(std::format(
            "'{}' is reachable from itself, which cannot be represented as a command.", describePath(path)));
    }

    path.push_back(node);

    if (node->isExecutable()) {
        if (out.size() >= MAX_OVERLOADS_PER_COMMAND) {
            return nonstd::make_unexpected(std::format("Too many branches: '{}' flattens to more than {} overloads.",
                                                       describePath(path), MAX_OVERLOADS_PER_COMMAND));
        }
        out.push_back({slots, path, node});
    }

    if (!node->getChildren().empty() && isGreedy(node)) {
        return nonstd::make_unexpected(std::format(
            "Argument '{}' consumes the rest of the line, so it must be the last one on its branch.", node->getName()));
    }

    for (const auto &child : node->getChildren()) {
        slots.push_back({child, !isArgument(child)});
        auto result = walk(child, path, slots, out);
        if (!result.has_value()) {
            return result;
        }
        slots.pop_back();
    }

    path.pop_back();
    return {};
}

}  // namespace

nonstd::expected<std::vector<CommandTreeOverload>, std::string> flattenCommandTree(const NotNull<CommandNode> &root)
{
    std::vector<CommandTreeOverload> out;
    std::vector<NotNull<CommandNode>> path;
    std::vector<CommandTreeSlot> slots;

    auto result = walk(root, path, slots, out);
    if (!result.has_value()) {
        return nonstd::make_unexpected(result.error());
    }

    if (out.empty()) {
        return nonstd::make_unexpected(std::format(
            "'{}' has no branch that does anything. Add executes() to at least one node.", root->getName()));
    }
    return out;
}

}  // namespace endstone::core
