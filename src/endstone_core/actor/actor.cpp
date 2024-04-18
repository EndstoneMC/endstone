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

#include "endstone/detail/actor/actor.h"

#include "bedrock/command/command_utils.h"
#include "bedrock/world/actor/actor.h"

namespace endstone::detail {

EndstoneActor::EndstoneActor(EndstoneServer &server, ::Actor &actor) : server_(server), actor_(actor) {}

void EndstoneActor::sendMessage(const std::string &message) const {}

void EndstoneActor::sendErrorMessage(const std::string &message) const {}

Server &EndstoneActor::getServer() const
{
    return server_;
}

std::string EndstoneActor::getName() const
{
    return CommandUtils::getActorName(actor_);
}

bool EndstoneActor::isPermissionSet(std::string name) const
{
    return mPerm.isPermissionSet(name);
}

bool EndstoneActor::isPermissionSet(const Permission &perm) const
{
    return mPerm.isPermissionSet(perm);
}

bool EndstoneActor::hasPermission(std::string name) const
{
    return mPerm.hasPermission(name);
}

bool EndstoneActor::hasPermission(const Permission &perm) const
{
    return mPerm.hasPermission(perm);
}

PermissionAttachment *EndstoneActor::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return mPerm.addAttachment(plugin, name, value);
}

PermissionAttachment *EndstoneActor::addAttachment(Plugin &plugin)
{
    return mPerm.addAttachment(plugin);
}

bool EndstoneActor::removeAttachment(PermissionAttachment &attachment)
{
    return mPerm.removeAttachment(attachment);
}

void EndstoneActor::recalculatePermissions()
{
    mPerm.recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> EndstoneActor::getEffectivePermissions() const
{
    return mPerm.getEffectivePermissions();
}

bool EndstoneActor::isOp() const
{
    return mPerm.isOp();
}

void EndstoneActor::setOp(bool value)
{
    mPerm.setOp(value);
}

std::uint64_t EndstoneActor::getRuntimeId()
{
    return actor_.getRuntimeID().id;
}

PermissibleBase EndstoneActor::mPerm(nullptr);

}  // namespace endstone::detail
