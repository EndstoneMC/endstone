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

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "endstone/detail/message.h"

namespace endstone::detail {
CommandSenderAdapter::CommandSenderAdapter(const CommandOrigin &origin, CommandOutput &output)
    : origin_(origin), output_(output)
{
}

void CommandSenderAdapter::sendMessage(const Message &message) const
{
    const auto tr = EndstoneMessage::toTranslatable(message);
    std::vector<CommandOutputParameter> params;
    for (const auto &param : tr.getWith()) {
        params.emplace_back(param);
    }
    output_.forceOutput(tr.getTranslate(), params);
}

void CommandSenderAdapter::sendErrorMessage(const Message &message) const
{
    const auto tr = EndstoneMessage::toTranslatable(message);
    std::vector<CommandOutputParameter> params;
    for (const auto &param : tr.getWith()) {
        params.emplace_back(param);
    }
    output_.error(tr.getTranslate(), params);
}

std::string CommandSenderAdapter::getName() const
{
    return origin_.getName();
}

bool CommandSenderAdapter::isOp() const
{
    switch (origin_.getPermissionsLevel()) {
    case CommandPermissionLevel::Any:
        return false;
    default:
        return true;
    }
}

void CommandSenderAdapter::setOp(bool value)
{
    getServer().getLogger().error("Changing the operator status of {} is not supported.", getName());
}

void CommandAdapter::execute(const CommandOrigin &origin, CommandOutput &output) const
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto &command_map = server.getCommandMap();
    auto command_name = getCommandName();
    auto *command = command_map.getCommand(command_name);

    if (!command) {
        server.getLogger().error("An unregistered command '{}' was executed by {}.", command_name, origin.getName());
        return;
    }

    bool success;
    if (auto *sender = origin.toEndstone(); sender) {
        success = command->execute(*sender, args_);
    }
    else {
        auto dummy_sender = CommandSenderAdapter(origin, output);
        success = command->execute(dummy_sender, args_);
    }

    if (success) {
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
bool CommandRegistry::parse<endstone::detail::CommandAdapter>(void *value, const ParseToken &parse_token,
                                                              const CommandOrigin &, int, std::string &,
                                                              std::vector<std::string> &) const
{
    spdlog::info("ParseToken:\n{}", parse_token);
    auto &output = static_cast<endstone::detail::CommandAdapter *>(value)->args_;

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
