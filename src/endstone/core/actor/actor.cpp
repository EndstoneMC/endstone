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

#include "endstone/core/actor/actor.h"

#include "bedrock/entity/components/offsets_component.h"
#include "bedrock/entity/components/post_tick_position_delta_component.h"
#include "bedrock/server/commands/command_utils.h"
#include "bedrock/server/commands/standard/teleport_command.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/core/level/dimension.h"
#include "endstone/core/level/level.h"
#include "endstone/core/permissions/permissible.h"

namespace endstone::core {

EndstoneActor::EndstoneActor(EndstoneServer &server, ::Actor &actor) : server_(server), actor_(actor.getWeakEntity())
{
    getPermissibleBase();
}

void EndstoneActor::sendMessage(const Message &message) const {}

void EndstoneActor::sendErrorMessage(const Message &message) const {}

Server &EndstoneActor::getServer() const
{
    return server_;
}

std::string EndstoneActor::getName() const
{
    return CommandUtils::getActorName(getActor());
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

Result<PermissionAttachment *> EndstoneActor::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return getPermissibleBase().addAttachment(plugin, name, value);
}

Result<PermissionAttachment *> EndstoneActor::addAttachment(Plugin &plugin)
{
    return getPermissibleBase().addAttachment(plugin);
}

Result<void> EndstoneActor::removeAttachment(PermissionAttachment &attachment)
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

std::string EndstoneActor::getType() const
{
    return getActor().getActorIdentifier().getCanonicalName();
}

std::uint64_t EndstoneActor::getRuntimeId() const
{
    return getActor().getRuntimeID().raw_id;
}

Location EndstoneActor::getLocation() const
{
    auto position = getActor().getPosition();
    position.y -= getActor().getPersistentComponent<OffsetsComponent>()->height_offset;
    const auto &rotation = getActor().getRotation();

    return {&getDimension(), position.x, position.y, position.z, rotation.x, rotation.y};
}

Vector<float> EndstoneActor::getVelocity() const
{
    if (getActor().hasCategory(ActorCategory::Mob) || getActor().hasCategory(ActorCategory::Ridable)) {
        auto *actor = getActor().getVehicle();
        if (!actor) {
            actor = &getActor();
        }
        auto *component = actor->tryGetComponent<PostTickPositionDeltaComponent>();
        if (component) {
            const auto &delta = component->value;
            return {delta.x, delta.y, delta.z};
        }
    }

    const auto &delta = getActor().getPosDelta();
    return {delta.x, delta.y, delta.z};
}

bool EndstoneActor::isOnGround() const
{
    return getActor().isOnGround();
}

bool EndstoneActor::isInWater() const
{
    return getActor().isInWater();
}

bool EndstoneActor::isInLava() const
{
    return getActor().isInLava();
}

Level &EndstoneActor::getLevel() const
{
    return *server_.getLevel();
}

Dimension &EndstoneActor::getDimension() const
{
    return *getLevel().getDimension(getActor().getDimension().getName());
}

void EndstoneActor::setRotation(float yaw, float pitch)
{
    getActor().setRotationWrapped({pitch, yaw});
}

void EndstoneActor::teleport(Location location)
{
    DimensionType destination_dimension = VanillaDimensions::Undefined;
    if (auto *dimension = location.getDimension(); dimension) {
        destination_dimension = static_cast<EndstoneDimension *>(dimension)->getHandle().getDimensionId();
    }

    auto rotation = RotationCommandUtils::RotationData{RelativeFloat{location.getPitch(), 0},
                                                       RelativeFloat{location.getYaw(), 0}, std::nullopt};

    auto target = TeleportCommand::computeTarget(getActor(),                                           // victim
                                                 {location.getX(), location.getY(), location.getZ()},  // destination
                                                 nullptr,                                              // facing
                                                 destination_dimension,                                //  dimension
                                                 rotation,                                             // rotation
                                                 CommandVersion::CurrentVersion);

    TeleportCommand::applyTarget(getActor(), std::move(target), /*keep_velocity*/ false);
}

void EndstoneActor::teleport(Actor &target)
{
    teleport(target.getLocation());
}

std::int64_t EndstoneActor::getId() const
{
    return getActor().getOrCreateUniqueID().raw_id;
}

void EndstoneActor::remove()
{
    getActor().remove();
}

bool EndstoneActor::isDead() const
{
    return !getActor().isAlive();
}

bool EndstoneActor::isValid() const
{
    const auto *handle = actor_.tryUnwrap<::Actor>();
    if (!handle) {
        return false;
    }
    return handle->isAlive();
}

int EndstoneActor::getHealth() const
{
    return getActor().getHealth();
}

Result<void> EndstoneActor::setHealth(int health) const
{
    if (health < 0 || health > getMaxHealth()) {
        return nonstd::make_unexpected(
            make_error("Health value ({}) must be between 0 and {}.", health, getMaxHealth()));
    }
    auto mutable_attr = getActor().getMutableAttribute("minecraft:health");
    mutable_attr.instance->setCurrentValue(static_cast<float>(health), mutable_attr.context);
    return {};
}

int EndstoneActor::getMaxHealth() const
{
    return getActor().getMaxHealth();
}

std::vector<std::string> EndstoneActor::getScoreboardTags() const
{
    return getActor().getTags();
}

bool EndstoneActor::addScoreboardTag(std::string tag) const
{
    return getActor().addTag(tag);
}

bool EndstoneActor::removeScoreboardTag(std::string tag) const
{
    return getActor().removeTag(tag);
}

bool EndstoneActor::isNameTagVisible() const
{
    return getActor().canShowNameTag();
}

void EndstoneActor::setNameTagVisible(bool visible)
{
    getActor().setNameTagVisible(visible);
}

bool EndstoneActor::isNameTagAlwaysVisible() const
{
    return getActor().entity_data.getInt8(static_cast<SynchedActorData::ID>(ActorDataIDs::NAMETAG_ALWAYS_SHOW)) != 0;
}

void EndstoneActor::setNameTagAlwaysVisible(bool visible)
{
    getActor().entity_data.set<SynchedActorData::TypeInt8>(
        static_cast<SynchedActorData::ID>(ActorDataIDs::NAMETAG_ALWAYS_SHOW), visible);
}

std::string EndstoneActor::getNameTag() const
{
    return getActor().getNameTag();
}

void EndstoneActor::setNameTag(std::string name)
{
    getActor().setNameTag(name);
}

std::string EndstoneActor::getScoreTag() const
{
    return getActor().getScoreTag();
}

void EndstoneActor::setScoreTag(std::string score)
{
    getActor().setScoreTag(score);
}

PermissibleBase &EndstoneActor::getPermissibleBase()
{
    static std::shared_ptr<PermissibleBase> perm = PermissibleBase::create(nullptr);
    return *perm;
}

::Actor &EndstoneActor::getActor() const
{
    return getHandle<::Actor>();
}

std::shared_ptr<EndstoneActor> EndstoneActor::create(EndstoneServer &server, ::Actor &actor)
{
    return PermissibleFactory::create<EndstoneActor>(server, actor);
}

}  // namespace endstone::core

endstone::core::EndstoneActor &Actor::getEndstoneActor0() const
{
    auto *self = const_cast<Actor *>(this);
    auto &component = entity_context_.getOrAddComponent<endstone::core::EndstoneActorComponent>();
    if (component.actor) {
        return *component.actor;
    }

    auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    if (auto *player = Player::tryGetFromEntity(self->entity_context_); player) {
        component.actor = endstone::core::EndstonePlayer::create(server, *player);
    }
    else if (auto *mob = Mob::tryGetFromEntity(self->entity_context_); mob) {
        component.actor = endstone::core::EndstoneMob::create(server, *mob);
    }
    else {
        component.actor = endstone::core::EndstoneActor::create(server, *self);
    }
    return *component.actor;
}
