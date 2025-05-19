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

#include "endstone/core/actor/mob.h"

#include "endstone/core/permissions/permissible.h"
#include "endstone/core/server.h"

namespace endstone::core {

EndstoneMob::EndstoneMob(EndstoneServer &server, ::Mob &mob) : EndstoneActor(server, mob) {}

Mob *EndstoneMob::asMob() const
{
    return Mob::asMob();
}

void EndstoneMob::sendMessage(const Message &message) const
{
    EndstoneActor::sendMessage(message);
}

void EndstoneMob::sendErrorMessage(const Message &message) const
{
    EndstoneActor::sendErrorMessage(message);
}

Server &EndstoneMob::getServer() const
{
    return EndstoneActor::getServer();
}

std::string EndstoneMob::getName() const
{
    return EndstoneActor::getName();
}

PermissionLevel EndstoneMob::getPermissionLevel() const
{
    return EndstoneActor::getPermissionLevel();
}

bool EndstoneMob::isPermissionSet(std::string name) const
{
    return EndstoneActor::isPermissionSet(name);
}

bool EndstoneMob::isPermissionSet(const Permission &perm) const
{
    return EndstoneActor::isPermissionSet(perm);
}

bool EndstoneMob::hasPermission(std::string name) const
{
    return EndstoneActor::hasPermission(name);
}

bool EndstoneMob::hasPermission(const Permission &perm) const
{
    return EndstoneActor::hasPermission(perm);
}

PermissionAttachment *EndstoneMob::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return EndstoneActor::addAttachment(plugin, name, value);
}

PermissionAttachment *EndstoneMob::addAttachment(Plugin &plugin)
{
    return EndstoneActor::addAttachment(plugin);
}

Result<void> EndstoneMob::removeAttachment(PermissionAttachment &attachment)
{
    return EndstoneActor::removeAttachment(attachment);
}

void EndstoneMob::recalculatePermissions()
{
    EndstoneActor::recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> EndstoneMob::getEffectivePermissions() const
{
    return EndstoneActor::getEffectivePermissions();
}

std::string EndstoneMob::getType() const
{
    return EndstoneActor::getType();
}

std::uint64_t EndstoneMob::getRuntimeId() const
{
    return EndstoneActor::getRuntimeId();
}

Location EndstoneMob::getLocation() const
{
    return EndstoneActor::getLocation();
}

Vector<float> EndstoneMob::getVelocity() const
{
    return EndstoneActor::getVelocity();
}

bool EndstoneMob::isOnGround() const
{
    return EndstoneActor::isOnGround();
}

bool EndstoneMob::isInWater() const
{
    return EndstoneActor::isInWater();
}

bool EndstoneMob::isInLava() const
{
    return EndstoneActor::isInLava();
}

Level &EndstoneMob::getLevel() const
{
    return EndstoneActor::getLevel();
}

Dimension &EndstoneMob::getDimension() const
{
    return EndstoneActor::getDimension();
}

void EndstoneMob::setRotation(float yaw, float pitch)
{
    EndstoneActor::setRotation(yaw, pitch);
    getMob().setYBodyRotation(yaw);
}

void EndstoneMob::teleport(Location location)
{
    EndstoneActor::teleport(location);
}

void EndstoneMob::teleport(Actor &target)
{
    EndstoneActor::teleport(target);
}

std::int64_t EndstoneMob::getId() const
{
    return EndstoneActor::getId();
}

void EndstoneMob::remove()
{
    EndstoneActor::remove();
}

bool EndstoneMob::isValid() const
{
    return EndstoneActor::isValid();
}

bool EndstoneMob::isDead() const
{
    return EndstoneActor::isDead();
}

int EndstoneMob::getHealth() const
{
    return EndstoneActor::getHealth();
}

Result<void> EndstoneMob::setHealth(int health) const
{
    return EndstoneActor::setHealth(health);
}

int EndstoneMob::getMaxHealth() const
{
    return EndstoneActor::getMaxHealth();
}

std::vector<std::string> EndstoneMob::getScoreboardTags() const
{
    return EndstoneActor::getScoreboardTags();
}

bool EndstoneMob::addScoreboardTag(std::string tag) const
{
    return EndstoneActor::addScoreboardTag(tag);
}

bool EndstoneMob::removeScoreboardTag(std::string tag) const
{
    return EndstoneActor::removeScoreboardTag(tag);
}

bool EndstoneMob::isNameTagVisible() const
{
    return EndstoneActor::isNameTagVisible();
}

void EndstoneMob::setNameTagVisible(bool visible)
{
    EndstoneActor::setNameTagVisible(visible);
}

bool EndstoneMob::isNameTagAlwaysVisible() const
{
    return EndstoneActor::isNameTagAlwaysVisible();
}

void EndstoneMob::setNameTagAlwaysVisible(bool visible)
{
    EndstoneActor::setNameTagAlwaysVisible(visible);
}

std::string EndstoneMob::getNameTag() const
{
    return EndstoneActor::getNameTag();
}

void EndstoneMob::setNameTag(std::string name)
{
    EndstoneActor::setNameTag(name);
}

std::string EndstoneMob::getScoreTag() const
{
    return EndstoneActor::getScoreTag();
}

void EndstoneMob::setScoreTag(std::string score)
{
    EndstoneActor::setScoreTag(score);
}

bool EndstoneMob::isGliding() const
{
    return getMob().isGliding();
}

::Mob &EndstoneMob::getMob() const
{
    return getHandle<::Mob>();
}

std::shared_ptr<EndstoneMob> EndstoneMob::create(EndstoneServer &server, ::Mob &mob)
{
    return PermissibleFactory::create<EndstoneMob>(server, mob);
}

}  // namespace endstone::core
