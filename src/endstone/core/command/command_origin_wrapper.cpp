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

#include "endstone/core/command/command_origin_wrapper.h"

#include "endstone/core/message.h"
#include "endstone/core/permissions/permissible.h"

namespace endstone::core {

CommandOriginWrapper::CommandOriginWrapper(const CommandOrigin &origin, CommandOutput &output)
    : origin_(origin), output_(output)
{
}

void CommandOriginWrapper::sendMessage(const Message &message) const
{
    const auto tr = EndstoneMessage::toTranslatable(message);
    std::vector<CommandOutputParameter> params;
    for (const auto &param : tr.getParameters()) {
        params.emplace_back(param);
    }
    output_.forceOutput(tr.getText(), params);
}

void CommandOriginWrapper::sendErrorMessage(const Message &message) const
{
    const auto tr = EndstoneMessage::toTranslatable(message);
    std::vector<CommandOutputParameter> params;
    for (const auto &param : tr.getParameters()) {
        params.emplace_back(param);
    }
    output_.error(tr.getText(), params);
}

std::string CommandOriginWrapper::getName() const
{
    return origin_.getName();
}

PermissionLevel CommandOriginWrapper::getPermissionLevel() const
{
    if (origin_.getPermissionsLevel() >= CommandPermissionLevel::Host) {
        return PermissionLevel::Console;
    }
    if (origin_.getPermissionsLevel() > CommandPermissionLevel::Any) {
        return PermissionLevel::Operator;
    }
    return PermissionLevel::Default;
}

std::shared_ptr<CommandOriginWrapper> CommandOriginWrapper::create(const CommandOrigin &origin, CommandOutput &output)
{
    return PermissibleFactory::create<CommandOriginWrapper>(origin, output);
}

}  // namespace endstone::core
