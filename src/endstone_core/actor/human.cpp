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

#include "endstone/detail/actor/human.h"

namespace endstone::detail {

EndstoneHumanActor::EndstoneHumanActor(EndstoneServer &server, ::Actor &actor)
    : EndstoneActor(server, actor), perm_(static_cast<HumanActor *>(this))
{
}

void EndstoneHumanActor::sendMessage(const std::string &message) const
{
    EndstoneActor::sendMessage(message);
}

void EndstoneHumanActor::sendErrorMessage(const std::string &message) const
{
    EndstoneActor::sendErrorMessage(message);
}

Server &EndstoneHumanActor::getServer() const
{
    return EndstoneActor::getServer();
}

std::string EndstoneHumanActor::getName() const
{
    return EndstoneActor::getName();
}

bool EndstoneHumanActor::isPermissionSet(std::string name) const
{
    return perm_.isPermissionSet(name);
}

bool EndstoneHumanActor::isPermissionSet(const Permission &perm) const
{
    return perm_.isPermissionSet(perm);
}

bool EndstoneHumanActor::hasPermission(std::string name) const
{
    return perm_.hasPermission(name);
}

bool EndstoneHumanActor::hasPermission(const Permission &perm) const
{
    return perm_.hasPermission(perm);
}

PermissionAttachment *EndstoneHumanActor::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return perm_.addAttachment(plugin, name, value);
}

PermissionAttachment *EndstoneHumanActor::addAttachment(Plugin &plugin)
{
    return perm_.addAttachment(plugin);
}

bool EndstoneHumanActor::removeAttachment(PermissionAttachment &attachment)
{
    return perm_.removeAttachment(attachment);
}

void EndstoneHumanActor::recalculatePermissions()
{
    perm_.recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> EndstoneHumanActor::getEffectivePermissions() const
{
    return perm_.getEffectivePermissions();
}

bool EndstoneHumanActor::isOp() const
{
    return op_;
}

void EndstoneHumanActor::setOp(bool value)
{
    op_ = value;
    perm_.recalculatePermissions();
}

std::uint64_t EndstoneHumanActor::getRuntimeId()
{
    return EndstoneActor::getRuntimeId();
}

}  // namespace endstone::detail
