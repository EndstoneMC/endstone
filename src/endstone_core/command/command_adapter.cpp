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

#include "endstone/detail/command/command_adapter.h"

#include <stack>
#include <string>
#include <vector>

namespace endstone::detail {
CommandSenderAdapter::CommandSenderAdapter(EndstoneServer &server, const CommandOrigin &origin, CommandOutput &output)
    : server_(server), origin_(origin), output_(output)
{
}

void CommandSenderAdapter::sendMessage(const std::string &message) const
{
    output_.forceOutput(message, {});
}

void CommandSenderAdapter::sendErrorMessage(const std::string &message) const
{
    output_.error(message, {});
}

Server &CommandSenderAdapter::getServer() const
{
    return server_;
}

std::string CommandSenderAdapter::getName() const
{
    return origin_.getName();
}

void CommandAdapter::execute(const CommandOrigin &origin, CommandOutput &output) const
{
    auto &server = Singleton<EndstoneServer>::getInstance();
    auto sender = CommandSenderAdapter(server, origin, output);

    auto &command_map = server.getCommandMap();
    auto command_name = getCommandName();
    auto *command = command_map.getCommand(command_name);
    if (command) {
        bool success = command->execute(sender, {});
        if (success) {
            output.success();
        }
    }
    else {
        sender.CommandSender::sendErrorMessage("Command '{}' was executed but not registered.", command_name);
    }
}

bool parseRuleAdapter(CommandRegistry *, void *value, const CommandRegistry::ParseToken &parse_token,
                      const CommandOrigin &, int, std::string &, std::vector<std::string> &)
{
    auto &output = *reinterpret_cast<std::vector<std::string> *>(value);
    if (!output.empty()) {
        return true;
    }

    const auto *root = &parse_token;
    while (root->parent != nullptr) {  // Find root
        root = root->parent;
    }

    if (root->child == nullptr) {  // this shouldn't happen
        return false;
    }

    // This is where the magic happen, we recreate the command args from the parse token!
    for (auto *node = root->child.get(); node; node = node->next.get()) {
        std::string result;
        std::stack<CommandRegistry::ParseToken *> stack;
        stack.push(node);

        while (!stack.empty()) {
            auto *top = stack.top();
            stack.pop();

            if (top->size > 0) {
                if (!result.empty()) {
                    result += " ";
                }
                result += std::string(top->data, top->size);
            }

            if (top != node && top->next != nullptr) {
                stack.push(top->next.get());
            }

            if (top->child != nullptr) {
                stack.push(top->child.get());
            }
        }

        output.push_back(result);
    }
    return true;
}
}  // namespace endstone::detail
