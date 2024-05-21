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

#include "bedrock/world/actor/actor_collision.h"
#include "bedrock/world/actor/actor_environment.h"
#include "bedrock/world/actor/components/actor_owner_component.h"
#include "bedrock/world/actor/components/actor_unique_id_component.h"
#include "bedrock/world/actor/components/flag_component.h"
#include "bedrock/world/actor/components/runtime_id_component.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/level.h"
#include "endstone/detail/actor/actor.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/player.h"
#include "endstone/detail/server.h"
#include "endstone/event/actor/actor_remove_event.h"

using endstone::detail::EndstoneServer;

void Actor::remove()
{
    if (!isPlayer()) {
        auto &server = entt::locator<EndstoneServer>::value();
        endstone::ActorRemoveEvent e{getEndstoneActor()};
        server.getPluginManager().callEvent(e);
    }

#if _WIN32
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Actor::remove, __FUNCDNAME__, this);
#else
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Actor::remove, "_ZN5Actor6removeEv", this);
#endif
}

void Actor::setDimension(WeakRef<Dimension> dimension)
{
    // TODO(event): call PlayerChangedLevelEvent for player??
    ENDSTONE_HOOK_CALL_ORIGINAL(&Actor::setDimension, this, std::move(dimension));
}

bool Actor::isPlayer() const
{
    return hasComponent<FlagComponent<PlayerComponentFlag>>();
}

bool Actor::isRemoved() const
{
    return !isAlive();
}

bool Actor::isOnGround() const
{
    return ActorCollision::isOnGround(context_);
}

bool Actor::isInWater() const
{
    return ActorEnvironment::getIsInWater(context_);
}

bool Actor::isInLava() const
{
    return ActorEnvironment::getIsInLava(context_);
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
    return components_.state_vector->position;
}

Vec3 const &Actor::getPosPrev() const
{
    return components_.state_vector->position_prev;
}

Vec3 const &Actor::getPosDelta() const
{
    return components_.state_vector->position_delta;
}

Vec2 const &Actor::getRotation() const
{
    return components_.rotation->rotation;
}

AABB const &Actor::getAABB() const
{
    return components_.aabb->aabb;
}

ActorRuntimeID Actor::getRuntimeID() const
{
    return tryGetComponent<RuntimeIDComponent>()->id;
}

ActorUniqueID Actor::getOrCreateUniqueID() const
{
    auto component = getPersistentComponent<ActorUniqueIDComponent const>();
    if (component->id.id != -1) {
        return component->id;
    }
    auto unique_id = level_->getNewUniqueID();
    return context_.getOrAddComponent<ActorUniqueIDComponent>(unique_id).id;
}

Actor *Actor::tryGetFromEntity(EntityContext const &ctx, bool include_removed)
{
    auto *component = ctx.tryGetComponent<ActorOwnerComponent>();
    if (!component) {
        return nullptr;
    }

    auto *actor = component->owner;
    if (actor && (!actor->isRemoved() || include_removed)) {
        return actor;
    }
    return nullptr;
}

endstone::detail::EndstoneActor &Actor::getEndstoneActor() const
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto *self = const_cast<Actor *>(this);

    if (isPlayer()) {
        auto *player = static_cast<Player *>(self);
        return context_.getOrAddComponent<endstone::detail::EndstonePlayer>(server, *player);
    }
    // TODO(actor): add factory method for other actors
    return context_.getOrAddComponent<endstone::detail::EndstoneActor>(server, *self);
}
