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

#include "endstone/detail/actor/player.h"

#include "bedrock/actor/components/user_entity_identifier.h"
#include "bedrock/network/game/text_packet.h"
#include "endstone/detail/server.h"
#include "endstone/util/color_format.h"

namespace endstone::detail {

EndstonePlayer::EndstonePlayer(EndstoneServer &server, ServerPlayer &player)
    : EndstoneHumanActor(server, player), player_(player)
{
    auto *component = player.tryGetComponent<UserEntityIdentifierComponent>();
    if (!component) {
        server.getLogger().critical("UserEntityIdentifierComponent is not valid when initialising Player.");
        std::terminate();
    }
    uuid_ = {component->uuid.msb, component->uuid.lsb};
}

void EndstonePlayer::sendMessage(const std::string &message) const
{
    // TODO(fixme): create a TextPacket and send it through player_.sendNetworkPacket();
}

void EndstonePlayer::sendErrorMessage(const std::string &message) const
{
    sendMessage(ColorFormat::RED + message);
}

Server &EndstonePlayer::getServer() const
{
    return EndstoneHumanActor::getServer();
}

std::string EndstonePlayer::getName() const
{
    return EndstoneHumanActor::getName();
}

bool EndstonePlayer::isPermissionSet(std::string name) const
{
    return EndstoneHumanActor::isPermissionSet(name);
}

bool EndstonePlayer::isPermissionSet(const Permission &perm) const
{
    return EndstoneHumanActor::isPermissionSet(perm);
}

bool EndstonePlayer::hasPermission(std::string name) const
{
    return EndstoneHumanActor::hasPermission(name);
}

bool EndstonePlayer::hasPermission(const Permission &perm) const
{
    return EndstoneHumanActor::hasPermission(perm);
}

PermissionAttachment *EndstonePlayer::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return EndstoneHumanActor::addAttachment(plugin, name, value);
}

PermissionAttachment *EndstonePlayer::addAttachment(Plugin &plugin)
{
    return EndstoneHumanActor::addAttachment(plugin);
}

bool EndstonePlayer::removeAttachment(PermissionAttachment &attachment)
{
    return EndstoneHumanActor::removeAttachment(attachment);
}

void EndstonePlayer::recalculatePermissions()
{
    EndstoneHumanActor::recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> EndstonePlayer::getEffectivePermissions() const
{
    return EndstoneHumanActor::getEffectivePermissions();
}

bool EndstonePlayer::isOp() const
{
    return player_.getCommandPermissionLevel() != CommandPermissionLevel::Any;
}

void EndstonePlayer::setOp(bool value)
{
    player_.setPermissions(value ? CommandPermissionLevel::Any : CommandPermissionLevel::GameDirectors);
}

UUID EndstonePlayer::getUniqueId() const
{
    return uuid_;
}

}  // namespace endstone::detail
