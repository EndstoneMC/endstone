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

#include "endstone/core/command/console_command_sender.h"

#include "endstone/core/message.h"
#include "endstone/core/server.h"

namespace endstone::core {

void EndstoneConsoleCommandSender::sendMessage(const Message &message) const
{
    getServer().getLogger().info(EndstoneMessage::toString(message));
}

void EndstoneConsoleCommandSender::sendErrorMessage(const Message &message) const
{
    getServer().getLogger().error(EndstoneMessage::toString(message));
}

std::string EndstoneConsoleCommandSender::getName() const
{
    return "Server";
}

PermissionLevel EndstoneConsoleCommandSender::getPermissionLevel() const
{
    return PermissionLevel::Console;
}

}  // namespace endstone::core
