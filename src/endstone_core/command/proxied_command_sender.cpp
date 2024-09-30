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

#include "endstone/detail/command/proxied_command_sender.h"

#include <nonstd/expected.hpp>

namespace endstone::detail {

EndstoneProxiedCommandSender::EndstoneProxiedCommandSender(Protected, CommandSender &caller, CommandSender &callee)
    : ProxiedCommandSender(Protected()), caller_(caller), callee_(callee)
{
}
bool EndstoneProxiedCommandSender::isOp() const
{
    return getCaller().isOp();
}

void EndstoneProxiedCommandSender::setOp(bool value)
{
    getCaller().setOp(value);
}

bool EndstoneProxiedCommandSender::isPermissionSet(std::string name) const
{
    return getCaller().isPermissionSet(name);
}

bool EndstoneProxiedCommandSender::isPermissionSet(const Permission &perm) const
{
    return getCaller().isPermissionSet(perm);
}

bool EndstoneProxiedCommandSender::hasPermission(std::string name) const
{
    return getCaller().hasPermission(name);
}

bool EndstoneProxiedCommandSender::hasPermission(const Permission &perm) const
{
    return getCaller().hasPermission(perm);
}

Result<PermissionAttachment *> EndstoneProxiedCommandSender::addAttachment(Plugin &plugin, const std::string &name,
                                                                           bool value)
{
    return getCaller().addAttachment(plugin, name, value);
}

Result<PermissionAttachment *> EndstoneProxiedCommandSender::addAttachment(Plugin &plugin)
{
    return getCaller().addAttachment(plugin);
}

Result<void> EndstoneProxiedCommandSender::removeAttachment(PermissionAttachment &attachment)
{
    return getCaller().removeAttachment(attachment);
}

void EndstoneProxiedCommandSender::recalculatePermissions()
{
    getCaller().recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> EndstoneProxiedCommandSender::getEffectivePermissions() const
{
    return getCaller().getEffectivePermissions();
}

CommandSender *EndstoneProxiedCommandSender::asCommandSender() const
{
    return ProxiedCommandSender::asCommandSender();
}

ConsoleCommandSender *EndstoneProxiedCommandSender::asConsole() const
{
    return ProxiedCommandSender::asConsole();
}

Player *EndstoneProxiedCommandSender::asPlayer() const
{
    return ProxiedCommandSender::asPlayer();
}

void EndstoneProxiedCommandSender::sendMessage(const Message &message) const
{
    getCaller().sendMessage(message);
}

void EndstoneProxiedCommandSender::sendErrorMessage(const Message &message) const
{
    getCaller().sendErrorMessage(message);
}

Server &EndstoneProxiedCommandSender::getServer() const
{
    return getCallee().getServer();
}

std::string EndstoneProxiedCommandSender::getName() const
{
    return getCallee().getName();
}

CommandSender &EndstoneProxiedCommandSender::getCaller() const
{
    return caller_;
}

CommandSender &EndstoneProxiedCommandSender::getCallee() const
{
    return callee_;
}

}  // namespace endstone::detail
