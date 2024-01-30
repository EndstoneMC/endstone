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

#include <spdlog/spdlog.h>

#include "bedrock/type_id.h"
#include "endstone_runtime/hook.h"

namespace {

class CommandParameterDataTemplate {
public:
    explicit CommandParameterDataTemplate(const CommandParameterData &data)
        : type_id_(data.getTypeId()), parse_rule_(data.getParseRule())
    {
    }

    CommandParameterData create(const char *name, int offset_value, bool optional, int offset_has_value)
    {
        return {type_id_,        parse_rule_, name,         static_cast<CommandParameterDataType>(0),
                nullptr,         nullptr,     offset_value, optional,
                offset_has_value};
    }

private:
    Bedrock::typeid_t<CommandRegistry> type_id_;
    CommandRegistry::ParseRule parse_rule_;
};

std::unordered_map<uint32_t, CommandParameterDataTemplate> gCommandParameterTemplate;
}  // namespace

void CommandRegistry::registerCommand(const std::string &name, const char *description, CommandPermissionLevel level,
                                      CommandFlag flag1, CommandFlag flag2)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerCommand, this, name, description, level, flag1, flag2);
}

const CommandRegistry::Signature *CommandRegistry::findCommand(const std::string &name) const
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::findCommand, this, name);
}

std::string CommandRegistry::describe(const CommandParameterData &param) const
{
    std::string result;
    ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&CommandRegistry::describe, result, this, param);
    return result;
}

void CommandRegistry::registerOverloadInternal(CommandRegistry::Signature &signature,
                                               CommandRegistry::Overload &overload)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerOverloadInternal, this, signature, overload);

    for (const auto &param : overload.params) {
        auto type_id = param.getTypeId();
        if (gCommandParameterTemplate.find(type_id.id) != gCommandParameterTemplate.end()) {
            continue;
        }

        // NOTE: Retrieve the typeid_t for each type after each game update and update values in `type_id.cpp`.
        // spdlog::debug("Bedrock::typeid_t<CommandRegistry> = {}, Description = {}", type_id.id, describe(param));

        // save the param as a template
        gCommandParameterTemplate.emplace(type_id.id, CommandParameterDataTemplate(param));
    }
}

CommandParameterData CommandParameterData::create(const Bedrock::typeid_t<CommandRegistry> &type_id, const char *name,
                                                  int offset_value, bool optional, int offset_has_value)
{
    auto it = gCommandParameterTemplate.find(type_id.id);
    if (it == gCommandParameterTemplate.end()) {
        spdlog::error("Bedrock::typeid_t<CommandRegistry> = {} is not registered, terminating...", type_id.id);
        std::terminate();
    }

    return it->second.create(name, offset_value, optional, offset_has_value);
}
