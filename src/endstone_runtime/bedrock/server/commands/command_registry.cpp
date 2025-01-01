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

#include "bedrock/server/commands/command_registry.h"

#include <iostream>
#include <memory>
#include <utility>

#include <cpptrace/cpptrace.hpp>
#include <spdlog/spdlog.h>

#include "bedrock/server/commands/command.h"
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
    return ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, param);
}

std::string CommandRegistry::describe(const Signature &signature, const std::string &name, const Overload &overload,
                                      unsigned int a4, unsigned int *a5, unsigned int *a6) const
{
    std::string (CommandRegistry::*fp)(const Signature &, const std::string &, const Overload &, unsigned int,
                                       unsigned int *, unsigned int *) const = &CommandRegistry::describe;
    return ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, signature, name, overload, a4, a5, a6);
}

void CommandRegistry::registerOverloadInternal(Signature &signature, Overload &overload)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerOverloadInternal, this, signature, overload);
}

std::unique_ptr<Command> CommandRegistry::createCommand(const ParseToken &parse_token, const CommandOrigin &origin,
                                                        int version, std::string &error_message,
                                                        std::vector<std::string> &error_params) const
{
    spdlog::debug("ParseToken:\n{}", parse_token);
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::createCommand, this, parse_token, origin, version,
                                       error_message, error_params);
}

int CommandRegistry::addEnumValues(const std::string &name, const std::vector<std::string> &values)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::addEnumValues, this, name, values);
}

AvailableCommandsPacket CommandRegistry::serializeAvailableCommands() const
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::serializeAvailableCommands, this);
}

CommandRegistry::Overload::Overload(const CommandVersion &version, AllocFunction alloc) : version(version), alloc(alloc)
{
}
