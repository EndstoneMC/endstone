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
#include "lib/hook/hook.h"

void CommandRegistry::registerCommand(const std::string &name, const char *description,
                                      enum CommandPermissionLevel level, CommandFlag flag1, CommandFlag flag2)
{
    mBedrockCommands[name] = std::make_shared<BedrockCommand>(name, I18n::get(description), std::vector<std::string>{},
                                                              std::vector<std::string>{});
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerCommand, this, name, description, level, flag1, flag2)
}

void CommandRegistry::registerAlias(std::string name, std::string alias)
{
    auto command = mBedrockCommands[name];
    auto aliases = command->getAliases();
    aliases.push_back(alias);
    command->setAliases(aliases);
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerAlias, this, std::move(name), std::move(alias))
}

void CommandRegistry::registerOverloadInternal(CommandRegistry::Signature &signature,
                                               CommandRegistry::Overload &overload)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerOverloadInternal, this, signature, overload)

    auto label = signature.label;
    auto usage = describe(signature, label, overload, 0, nullptr, nullptr);
    auto command = mBedrockCommands[label];
    auto usages = command->getUsages();
    usages.push_back(usage);
    command->setUsages(usages);
}

std::string CommandRegistry::describe(CommandRegistry::Signature const &signature, const std::string &label,
                                      CommandRegistry::Overload const &overload, unsigned int unused,
                                      unsigned int *out_label_size, unsigned int *out_args_size) const
{
    std::string result;
    result = *ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&CommandRegistry::describe, this, &result, signature, label, overload,
                                              unused, out_label_size, out_args_size);
    return result;
}
