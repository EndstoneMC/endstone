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
#include "endstone/core/permissions/permissible.h"
#include "endstone/core/server.h"

namespace endstone::core {

ConsoleCommandSender *EndstoneConsoleCommandSender::asConsole() const
{
    return const_cast<EndstoneConsoleCommandSender *>(this);
}

void EndstoneConsoleCommandSender::sendMessage(const Message &message) const
{
    getServer().getLogger().info(EndstoneMessage::toString(message));
}

void EndstoneConsoleCommandSender::sendErrorMessage(const Message &message) const
{
    getServer().getLogger().error(EndstoneMessage::toString(message));
}

Server &EndstoneConsoleCommandSender::getServer() const
{
    return ServerCommandSender::getServer();
}

std::string EndstoneConsoleCommandSender::getName() const
{
    return "Server";
}

bool EndstoneConsoleCommandSender::isOp() const
{
    return true;
}

void EndstoneConsoleCommandSender::setOp(bool value)
{
    getServer().getLogger().error("Cannot change operator status of server console");
}

bool EndstoneConsoleCommandSender::isPermissionSet(std::string name) const
{
    return ServerCommandSender::isPermissionSet(name);
}

bool EndstoneConsoleCommandSender::isPermissionSet(const Permission &perm) const
{
    return ServerCommandSender::isPermissionSet(perm);
}

bool EndstoneConsoleCommandSender::hasPermission(std::string name) const
{
    return ServerCommandSender::hasPermission(name);
}

bool EndstoneConsoleCommandSender::hasPermission(const Permission &perm) const
{
    return ServerCommandSender::hasPermission(perm);
}

Result<PermissionAttachment *> EndstoneConsoleCommandSender::addAttachment(Plugin &plugin, const std::string &name,
                                                                           bool value)
{
    return ServerCommandSender::addAttachment(plugin, name, value);
}

Result<PermissionAttachment *> EndstoneConsoleCommandSender::addAttachment(Plugin &plugin)
{
    return ServerCommandSender::addAttachment(plugin);
}

Result<void> EndstoneConsoleCommandSender::removeAttachment(PermissionAttachment &attachment)
{
    return ServerCommandSender::removeAttachment(attachment);
}

void EndstoneConsoleCommandSender::recalculatePermissions()
{
    ServerCommandSender::recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> EndstoneConsoleCommandSender::getEffectivePermissions() const
{
    return ServerCommandSender::getEffectivePermissions();
}

std::shared_ptr<EndstoneConsoleCommandSender> EndstoneConsoleCommandSender::create()
{
    return PermissibleFactory::create<EndstoneConsoleCommandSender>();
}

}  // namespace endstone::core
