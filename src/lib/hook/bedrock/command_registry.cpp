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

#include "bedrock/command_origin.h"
#include "bedrock/i18n.h"
#include "endstone/command/bedrock/bedrock_command.h"
#include "endstone/endstone_server.h"
#include "lib/hook/hook.h"

void CommandRegistry::registerCommand(const std::string &name, const char *description, CommandPermissionLevel level,
                                      CommandFlag flag1, CommandFlag flag2)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerCommand, this, name, description, level, flag1, flag2)

    auto desc = I18n::get(description);
    if (desc == description) {
        desc = "A Mojang provided command.";
    }

    mBedrockCommands[name] =
        std::make_shared<BedrockCommand>(name, desc, std::vector<std::string>{}, std::vector<std::string>{});

    std::optional<PermissionDefault> value;
    switch (level) {
    case CommandPermissionLevel::Any:
        value = PermissionDefault::Any;
        break;
    case CommandPermissionLevel::GameDirectors:
        value = PermissionDefault::Operator;
        break;
    case CommandPermissionLevel::Admin:
    case CommandPermissionLevel::Host:
    case CommandPermissionLevel::Owner:
        value = PermissionDefault::Owner;
        break;
    case CommandPermissionLevel::Internal:
    default:
        value = PermissionDefault::None;
        break;
    }

    EndstoneServer::getInstance().getPluginManager().addPermission(std::make_shared<Permission>(
        "minecraft.command." + name, fmt::format("Allow the use of the {} command.", name), value));
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

const CommandRegistry::Signature *CommandRegistry::findCommand(const std::string &name) const
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::findCommand, this, name);
}

bool CommandRegistry::checkOriginCommandFlags(const CommandOrigin &origin, CommandFlag flag,
                                              CommandPermissionLevel permission_level) const
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::checkOriginCommandFlags, this, origin, flag, permission_level);
}
