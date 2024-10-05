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

#include "endstone/detail/command/minecraft_command_adapter.h"

#include <stack>
#include <string>
#include <vector>

#include <entt/entt.hpp>

#include "endstone/detail/command/command_origin_wrapper.h"
#include "endstone/detail/command/command_output_with_sender.h"

namespace endstone::detail {

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
        sender = CommandOriginWrapper::create(origin, output);
    }
    if (command->unwrap().execute(*sender, args_)) {
        output.success();
    }
}

}  // namespace endstone::detail

namespace {

std::string removeQuotes(const std::string &str)
{
    if (str.size() < 2) {
        return str;
    }
    if (str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}
}  // namespace

template <>
bool CommandRegistry::parse<endstone::detail::MinecraftCommandAdapter>(void *value, const ParseToken &parse_token,
                                                                       const CommandOrigin &, int, std::string &,
                                                                       std::vector<std::string> &) const
{
    auto &output = static_cast<endstone::detail::MinecraftCommandAdapter *>(value)->args_;

    std::string result;
    std::stack<const ParseToken *> stack;
    stack.push(&parse_token);

    while (!stack.empty()) {
        const auto *top = stack.top();
        stack.pop();

        if (top->length > 0) {
            if (!result.empty()) {
                result += " ";
            }

            auto str = std::string(top->text, top->length);
            if (top->type.value() == static_cast<std::uint32_t>(HardNonTerminal::Id)) {
                str = removeQuotes(str);
            }

            result += str;
        }

        if (top != &parse_token && top->next != nullptr) {
            stack.push(top->next.get());
        }

        if (top->child != nullptr) {
            stack.push(top->child.get());
        }
    }

    output.push_back(result);
    return true;
}
