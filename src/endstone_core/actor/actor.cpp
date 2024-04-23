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

EndstoneActor::EndstoneActor(EndstoneServer &server, ::Actor &actor) : server_(server), actor_(actor)
{
    getPermissibleBase();
}

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
    return getPermissibleBase().isPermissionSet(name);
}

bool EndstoneActor::isPermissionSet(const Permission &perm) const
{
    return getPermissibleBase().isPermissionSet(perm);
}

bool EndstoneActor::hasPermission(std::string name) const
{
    return getPermissibleBase().hasPermission(name);
}

bool EndstoneActor::hasPermission(const Permission &perm) const
{
    return getPermissibleBase().hasPermission(perm);
}

PermissionAttachment *EndstoneActor::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return getPermissibleBase().addAttachment(plugin, name, value);
}

PermissionAttachment *EndstoneActor::addAttachment(Plugin &plugin)
{
    return getPermissibleBase().addAttachment(plugin);
}

bool EndstoneActor::removeAttachment(PermissionAttachment &attachment)
{
    return getPermissibleBase().removeAttachment(attachment);
}

void EndstoneActor::recalculatePermissions()
{
    getPermissibleBase().recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> EndstoneActor::getEffectivePermissions() const
{
    return getPermissibleBase().getEffectivePermissions();
}

bool EndstoneActor::isOp() const
{
    return getPermissibleBase().isOp();
}

void EndstoneActor::setOp(bool value)
{
    getPermissibleBase().setOp(value);
}

std::uint64_t EndstoneActor::getRuntimeId()
{
    return actor_.getRuntimeID().id;
}

PermissibleBase &EndstoneActor::getPermissibleBase()
{
    static std::unique_ptr<PermissibleBase> perm = []() {
        auto instance = std::make_unique<PermissibleBase>(nullptr);
        instance->recalculatePermissions();
        return instance;
    }();
    return *perm;
}

}  // namespace endstone::detail
