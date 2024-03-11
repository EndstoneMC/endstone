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

#include "bedrock/command/command_registry.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <stack>
#include <utility>

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include "bedrock/command/command.h"
#include "bedrock/type_id.h"
#include "endstone/detail/hook.h"

void CommandRegistry::registerCommand(const std::string &name, const char *description, CommandPermissionLevel level,
                                      CommandFlag flag1, CommandFlag flag2)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerCommand, this, name, description, level, flag1, flag2);
}

void CommandRegistry::registerAlias(std::string name, std::string alias)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerAlias, this, std::move(name), std::move(alias));
}

const CommandRegistry::Signature *CommandRegistry::findCommand(const std::string &name) const
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::findCommand, this, name);
}

std::string CommandRegistry::describe(const CommandParameterData &param) const
{
    std::string (CommandRegistry::*fp)(const CommandParameterData &param) const = &CommandRegistry::describe;
    std::string result;
    ENDSTONE_HOOK_CALL_ORIGINAL_RVO(fp, result, this, param);
    return result;
}

std::string CommandRegistry::describe(const CommandRegistry::Signature &signature, const std::string &name,
                                      const CommandRegistry::Overload &overload, unsigned int a4, unsigned int *a5,
                                      unsigned int *a6) const
{
    std::string (CommandRegistry::*fp)(const CommandRegistry::Signature &, const std::string &,
                                       const CommandRegistry::Overload &, unsigned int, unsigned int *, unsigned int *)
        const = &CommandRegistry::describe;
    std::string result;
    ENDSTONE_HOOK_CALL_ORIGINAL_RVO(fp, result, this, signature, name, overload, a4, a5, a6);
    return result;
}

void CommandRegistry::registerOverloadInternal(CommandRegistry::Signature &signature,
                                               CommandRegistry::Overload &overload)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerOverloadInternal, this, signature, overload);
}

std::unique_ptr<Command> CommandRegistry::createCommand(const CommandRegistry::ParseToken &parse_token,
                                                        const CommandOrigin &origin, int version,
                                                        std::string &error_message,
                                                        std::vector<std::string> &error_params) const
{
    spdlog::debug("ParseToken:\n{}", parse_token);
    std::unique_ptr<Command> result;
    ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&CommandRegistry::createCommand, result, this, parse_token, origin, version,
                                    error_message, error_params);
    return result;
}

template <>
struct fmt::formatter<CommandRegistry::ParseToken> : ostream_formatter {};

std::ostream &operator<<(std::ostream &os, const CommandRegistry::ParseToken &token)
{
    std::stack<std::pair<const CommandRegistry::ParseToken *, int>> to_visit;

    to_visit.emplace(&token, 0);
    while (!to_visit.empty()) {
        auto node_level = to_visit.top();
        to_visit.pop();

        const auto *node = node_level.first;
        auto level = node_level.second;

        os << std::string(level * 4, ' ');

        if (node) {
            os << "Symbol: 0x" << std::hex << node->symbol.value << std::dec;
            if (node->size > 0) {
                os << ", Data: " << std::string(node->data, node->size);
            }
            os << "\n";

            if (node->next) {
                to_visit.emplace(node->next.get(), level);
            }
            if (node->child) {
                to_visit.emplace(node->child.get(), level + 1);
            }
        }
    }
    return os;
}

int CommandRegistry::addEnumValues(const std::string &name, const std::vector<std::string> &values)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::addEnumValues, this, name, values);
}

const CommandRegistry::Symbol CommandRegistry::Symbol::INT{0x100001};
const CommandRegistry::Symbol CommandRegistry::Symbol::FLOAT{0x100003};
const CommandRegistry::Symbol CommandRegistry::Symbol::RELATIVE_FLOAT{0x100004};
const CommandRegistry::Symbol CommandRegistry::Symbol::WILDCARD_INT{0x100005};
const CommandRegistry::Symbol CommandRegistry::Symbol::OPERATOR{0x100006};
const CommandRegistry::Symbol CommandRegistry::Symbol::COMPARE_OPERATOR{0x100007};
const CommandRegistry::Symbol CommandRegistry::Symbol::SELECTOR{0x100008};
const CommandRegistry::Symbol CommandRegistry::Symbol::WILDCARD_ACTOR_SELECTOR{0x10000a};
const CommandRegistry::Symbol CommandRegistry::Symbol::FILE_PATH{0x100011};
const CommandRegistry::Symbol CommandRegistry::Symbol::INTEGER_RANGE{0x100017};
const CommandRegistry::Symbol CommandRegistry::Symbol::EQUIPMENT_SLOT{0x10002b};
const CommandRegistry::Symbol CommandRegistry::Symbol::STRING{0x10002c};
const CommandRegistry::Symbol CommandRegistry::Symbol::POSITION{0x100034};
const CommandRegistry::Symbol CommandRegistry::Symbol::POSITION_FLOAT{0x100035};
const CommandRegistry::Symbol CommandRegistry::Symbol::MESSAGE{0x100038};
const CommandRegistry::Symbol CommandRegistry::Symbol::RAW_TEXT{0x10003a};
const CommandRegistry::Symbol CommandRegistry::Symbol::JSON{0x10003e};
const CommandRegistry::Symbol CommandRegistry::Symbol::BLOCK_STATE{0x100048};
const CommandRegistry::Symbol CommandRegistry::Symbol::COMMAND{0x10004b};
