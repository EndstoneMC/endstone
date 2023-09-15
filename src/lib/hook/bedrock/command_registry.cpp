// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "bedrock/command_registry.h"

#include "bedrock/i18n.h"
#include "endstone/command/bedrock/bedrock_command.h"
#include "hook/hook.h"

void CommandRegistry::registerCommand(const std::string &name, const char *description,
                                      enum CommandPermissionLevel level, CommandFlag flag1, CommandFlag flag2)
{
    bedrock_commands[name] = std::make_shared<BedrockCommand>(name, I18n::get(description), std::vector<std::string>{},
                                                              std::vector<std::string>{});
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerCommand, this, name, description, level, flag1, flag2);
}

void CommandRegistry::registerAlias(std::string name, std::string alias)
{
    auto command = bedrock_commands[name];
    auto aliases = command->getAliases();
    aliases.push_back(alias);
    command->setAliases(aliases);
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerAlias, this, std::move(name), std::move(alias));
}

void CommandRegistry::registerOverloadInternal(CommandRegistry::Signature &signature,
                                               CommandRegistry::Overload &overload)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerOverloadInternal, this, signature, overload);

    auto name = signature.label;
    // The description is fetched after registration to ensure any new types associated with this overload are added.
    std::vector<std::string> parts = {"/{command}"};
    for (const auto &parameter : overload.parameters) {
        parts.push_back(describe(parameter));
    }
    auto usage = fmt::format("{}", fmt::join(parts.begin(), parts.end(), " "));

    auto command = bedrock_commands[name];
    auto usages = command->getUsages();
    usages.push_back(usage);
    command->setUsages(usages);
}

std::string CommandRegistry::describe(const CommandParameterData &parameter) const
{
    std::string result;
    result = *ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&CommandRegistry::describe, this, &result, parameter);
    return result;
}
