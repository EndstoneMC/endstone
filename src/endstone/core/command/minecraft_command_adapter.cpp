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

#include "endstone/core/command/minecraft_command_adapter.h"

#include <string>
#include <vector>

#include <entt/entt.hpp>

#include "endstone/core/command/command_origin_wrapper.h"

namespace endstone::core {

void MinecraftCommandAdapter::execute(const CommandOrigin &origin, CommandOutput &output) const
{
    const auto &server = entt::locator<EndstoneServer>::value();
    const auto &command_map = server.getCommandMap();
    const auto command_name = getCommandName();
    const auto command = command_map.getCommand(command_name);
    if (!command) {
        throw std::runtime_error("Command not found");
    }

    auto sender = origin.getEndstoneSender(output);
    if (command->execute(*sender, args_)) {
        output.success();
    }
}

}  // namespace endstone::core

namespace {
std::string_view removeQuotes(const std::string_view &str)
{
    if (str.size() < 2) {
        return str;
    }
    if (str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

const CommandRegistry::ParseToken *findFirstWithText(const CommandRegistry::ParseToken *node)
{
    if (!node) {
        return nullptr;
    }

    // Check current node first (pre-order)
    if (node->text) {
        return node;
    }

    // Recurse into child
    if (const CommandRegistry::ParseToken *found = findFirstWithText(node->child.get())) {
        return found;
    }

    // Recurse into next sibling
    return findFirstWithText(node->next.get());
}

std::string_view rstrip(std::string_view str, std::string_view chars = " \t\n\r\f\v")
{
    size_t end = str.size();
    while (end > 0 && chars.find(str[end - 1]) != std::string_view::npos) {
        --end;
    }
    return str.substr(0, end);
}

std::string_view parseNode(const CommandRegistry::ParseToken &root)
{
    // If there are no children, this is a leaf text node.
    if (!root.child) {
        if (root.type == CommandRegistry::HardNonTerminal::Id) {
            return removeQuotes({root.text, root.length});
        }
        return {root.text, root.length};
    }

    // Otherwise:
    const auto &child = *root.child;
    const auto *begin = findFirstWithText(&child);
    if (!begin) {
        return "";
    }

    // (1) If this node has no next sibling, it's the last argument: consume all remaining text until the end
    if (!root.next) {
        const auto view = rstrip(begin->text);
        if (child.type == CommandRegistry::HardNonTerminal::Id) {
            return removeQuotes(view);
        }
        return view;
    }

    // (2) Otherwise, find the start of the next argument and extract the substring between begin and end, then trim
    // trailing whitespace.
    const auto *end = findFirstWithText(root.next.get());
    if (!end) {
        const auto view = rstrip(begin->text);
        if (child.type == CommandRegistry::HardNonTerminal::Id) {
            return removeQuotes(view);
        }
        return view;
    }

    const auto view = rstrip({begin->text, end->text});
    if (child.type == CommandRegistry::HardNonTerminal::Id) {
        return removeQuotes(view);
    }
    return view;
}
}  // namespace

template <>
bool CommandRegistry::parse<endstone::core::MinecraftCommandAdapter>(void *storage, const ParseToken &token,
                                                                     const CommandOrigin &origin, int version,
                                                                     std::string &error,
                                                                     std::vector<std::string> &error_params) const
{
    if (!storage) {
        return false;
    }
    auto &output = static_cast<endstone::core::MinecraftCommandAdapter *>(storage)->args_;
    output.emplace_back(parseNode(token));
    return true;
}
