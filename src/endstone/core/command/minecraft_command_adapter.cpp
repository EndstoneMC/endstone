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
#include "endstone/core/command/command_output_with_sender.h"

namespace endstone::core {

void MinecraftCommandAdapter::execute(const CommandOrigin &origin, CommandOutput &output) const
{
    const auto &server = entt::locator<EndstoneServer>::value();
    const auto &command_map = server.getCommandMap();
    const auto command_name = getCommandName();
    const auto *command = static_cast<CommandWrapper *>(command_map.getCommand(command_name));
    if (!command) {
        throw std::runtime_error("Command not found");
    }

    // We already have a sender passed down from CommandWrapper::execute
    if (output.has_command_sender) {
        if (command->unwrap().execute(static_cast<CommandOutputWithSender &>(output).sender_, args_)) {
            output.success();
        }
        return;
    }

    // We don't have a sender yet - this is called from the original dispatching route
    auto sender = origin.getEndstoneSender();
    if (!sender) {
        // Fallback to command origin via a wrapper for unsupported types
        const auto wrapper = CommandOriginWrapper::create(origin, output);
        wrapper->init();
        sender = wrapper;
    }
    if (command->unwrap().execute(*sender, args_)) {
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

std::string parseNode(const CommandRegistry::ParseToken &root)
{
    auto get_string = [](const CommandRegistry::ParseToken &token) -> std::string {
        const std::string_view result = {token.text, token.length};
        if (token.type == CommandRegistry::HardNonTerminal::Id) {
            return std::string(removeQuotes(result));
        }
        return std::string(result);
    };

    if (!root.child) {
        return get_string(root);
    }

    auto *child = root.child.get();
    auto *last_sibling = child;
    for (auto *it = child->next.get(); it; it = it->next.get()) {
        last_sibling = it;
    }

    auto *begin = child;
    for (auto *it = child->child.get(); it; it = it->child.get()) {
        begin = it;
    }

    auto *end = last_sibling;
    for (auto *it = last_sibling->child.get(); it; it = it->child.get()) {
        end = it;
    }

    if (begin == end) {
        return get_string(*begin);
    }
    return {begin->text, end->text + end->length};
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
