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

#include "bedrock/world/actor/actor.h"

#include "bedrock/entity/components/actor_data_flag_component.h"
#include "bedrock/entity/components/actor_owner_component.h"
#include "bedrock/entity/components/actor_type_flag_component.h"
#include "bedrock/entity/components/actor_unique_id_component.h"
#include "bedrock/entity/components/passenger_component.h"
#include "bedrock/entity/components/player_component.h"
#include "bedrock/entity/components/runtime_id_component.h"
#include "bedrock/entity/utilities/rotation_utility.h"
#include "bedrock/world/actor/actor_collision.h"
#include "bedrock/world/actor/actor_environment.h"
#include "bedrock/world/actor/mob_jump.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/level.h"
#include "endstone/detail/actor/actor.h"
#include "endstone/detail/actor/mob.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/player.h"
#include "endstone/detail/server.h"
#include "endstone/event/actor/actor_remove_event.h"
#include "endstone/event/actor/actor_teleport_event.h"

using endstone::detail::EndstoneActor;
using endstone::detail::EndstoneActorComponent;
using endstone::detail::EndstoneMob;
using endstone::detail::EndstonePlayer;
using endstone::detail::EndstoneServer;

void Actor::remove()
{
    if (!isPlayer()) {
        auto &server = entt::locator<EndstoneServer>::value();
        endstone::ActorRemoveEvent e{getEndstoneActor()};
        server.getPluginManager().callEvent(e);
    }

    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Actor::remove, __FUNCDNAME__, this);
}

void Actor::teleportTo(const Vec3 &pos, bool should_stop_riding, int cause, int entity_type, bool keep_velocity)
{
    Vec3 position = pos;
    if (!isPlayer()) {
        auto &server = entt::locator<EndstoneServer>::value();
        auto &actor = getEndstoneActor();
        endstone::Location to{&actor.getDimension(), pos.x, pos.y, pos.z, getRotation().x, getRotation().y};
        endstone::ActorTeleportEvent e{actor, actor.getLocation(), to};
        server.getPluginManager().callEvent(e);

        if (e.isCancelled()) {
            return;
        }
        position = {e.getTo().getX(), e.getTo().getY(), e.getTo().getZ()};
    }
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Actor::teleportTo, __FUNCDNAME__, this, position, should_stop_riding, cause,
                                     entity_type, keep_velocity);
}

bool Actor::getStatusFlag(ActorFlags flags) const
{
    auto component = getPersistentComponent<ActorDataFlagComponent>();
    return component->getStatusFlag(flags);
}

bool Actor::isType(ActorType type) const
{
    auto component = getPersistentComponent<ActorTypeComponent>();
    return component->type == type;
}

bool Actor::hasType(ActorType type) const
{
    auto component = getPersistentComponent<ActorTypeComponent>();
    return !!(component->type & type);
}

bool Actor::isPlayer() const
{
    return hasComponent<PlayerComponent>();
}

bool Actor::isRemoved() const
{
    return !isAlive();
}

bool Actor::isOnGround() const
{
    return ActorCollision::isOnGround(entity_context_);
}

bool Actor::isInWater() const
{
    return ActorEnvironment::getIsInWater(entity_context_);
}

bool Actor::isInLava() const
{
    return ActorEnvironment::getIsInLava(entity_context_);
}

Dimension &Actor::getDimension() const
{
    return *dimension_;
}

Level &Actor::getLevel() const
{
    return *level_;
}

Vec3 const &Actor::getPosition() const
{
    return built_in_components_.state_vector_component->pos;
}

Vec3 const &Actor::getPosPrev() const
{
    return built_in_components_.state_vector_component->pos_prev;
}

void Actor::applyImpulse(Vec3 const &impulse)
{
    built_in_components_.state_vector_component->pos_delta += impulse;
}

Vec3 const &Actor::getPosDelta() const
{
    return built_in_components_.state_vector_component->pos_delta;
}

void Actor::setPosDelta(const Vec3 &pos_delta)
{
    built_in_components_.state_vector_component->pos_delta = pos_delta;
}

Vec2 const &Actor::getRotation() const
{
    return built_in_components_.actor_rotation_component->rotation_degree;
}

void Actor::setRotationWrapped(const Vec2 &rot)
{
    RotationUtility::setRot(rot, built_in_components_.actor_rotation_component->rotation_degree,
                            built_in_components_.actor_rotation_component->rotation_degree_previous);
}

AABB const &Actor::getAABB() const
{
    return built_in_components_.aabb_shape_component->aabb;
}

ActorRuntimeID Actor::getRuntimeID() const
{
    return tryGetComponent<RuntimeIDComponent>()->runtime_id;
}

ActorUniqueID Actor::getOrCreateUniqueID() const
{
    auto component = getPersistentComponent<ActorUniqueIDComponent const>();
    if (component->unique_id.raw_id != -1) {
        return component->unique_id;
    }
    auto unique_id = level_->getNewUniqueID();
    return entity_context_.getOrAddComponent<ActorUniqueIDComponent>(unique_id).unique_id;
}

Actor *Actor::getVehicle() const
{
    const auto *component = tryGetComponent<PassengerComponent>();
    if (component && !component->vehicle.entity.isNull()) {
        return level_->fetchEntity(component->vehicle.actor_id, false);
    }
    return nullptr;
}

bool Actor::isRiding() const
{
    return getVehicle() != nullptr;
}

bool Actor::hasCategory(ActorCategory category) const
{
    return !!(categories_ & category);
}

Actor *Actor::tryGetFromEntity(EntityContext const &ctx, bool include_removed)
{
    auto *component = ctx.tryGetComponent<ActorOwnerComponent>();
    if (!component) {
        return nullptr;
    }

    auto &actor = component->getActor();
    if (!actor.isRemoved() || include_removed) {
        return &actor;
    }
    return nullptr;
}

EndstoneActor &Actor::getEndstoneActor0() const
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto *self = const_cast<Actor *>(this);
    auto &component = entity_context_.getOrAddComponent<EndstoneActorComponent>();
    if (component.actor) {
        return *component.actor;
    }

    if (self->isType(ActorType::Player)) {
        if (!isPlayer()) {
            // Sanity check, should never be reachable
            throw std::runtime_error("Actor has a Player type but isPlayer() returns false.");
        }
        auto *player = static_cast<Player *>(self);
        component.actor = EndstonePlayer::create(server, *player);
    }
    else if (self->hasType(ActorType::Mob) || self->hasCategory(ActorCategory::Mob)) {
        auto *mob = static_cast<Mob *>(self);
        component.actor = EndstoneMob::create(server, *mob);
    }
    else {
        component.actor = EndstoneActor::create(server, *self);
    }
    return *component.actor;
}

bool Actor::isJumping() const
{
    return MobJump::isJumping(entity_context_);
}

const AttributeInstance &Actor::getAttribute(const HashedString &name) const
{
    auto component = getPersistentComponent<AttributesComponent>();
    return component->attributes.getInstance(name);
}

AttributeInstance &Actor::getMutableAttribute(const HashedString &name)
{
    auto component = getPersistentComponent<AttributesComponent>();
    return component->attributes.getMutableInstance(name);
}
