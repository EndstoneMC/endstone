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

#include "endstone/core/command/server_command_sender.h"

#include <utility>

#include <entt/entt.hpp>

#include "endstone/core/server.h"

namespace endstone::core {

ServerCommandSender::ServerCommandSender(std::shared_ptr<PermissibleBase> perm)
{
    if (perm) {
        perm_ = std::move(perm);
    }
}

Server &ServerCommandSender::getServer() const
{
    return entt::locator<EndstoneServer>::value();
}

bool ServerCommandSender::isPermissionSet(std::string name) const
{
    return pimpl().isPermissionSet(name);
}

bool ServerCommandSender::isPermissionSet(const Permission &perm) const
{
    return pimpl().isPermissionSet(perm);
}

bool ServerCommandSender::hasPermission(std::string name) const
{
    return pimpl().hasPermission(name);
}

bool ServerCommandSender::hasPermission(const Permission &perm) const
{
    return pimpl().hasPermission(perm);
}

Result<PermissionAttachment *> ServerCommandSender::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return pimpl().addAttachment(plugin, name, value);
}

Result<PermissionAttachment *> ServerCommandSender::addAttachment(Plugin &plugin)
{
    return pimpl().addAttachment(plugin);
}

Result<void> ServerCommandSender::removeAttachment(PermissionAttachment &attachment)
{
    return pimpl().removeAttachment(attachment);
}

void ServerCommandSender::recalculatePermissions()
{
    pimpl().recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> ServerCommandSender::getEffectivePermissions() const
{
    return pimpl().getEffectivePermissions();
}

PermissibleBase &ServerCommandSender::pimpl() const
{
    if (!perm_) {
        perm_ = PermissibleBase::create(const_cast<ServerCommandSender *>(this));
    }
    return *perm_;
}

}  // namespace endstone::core
