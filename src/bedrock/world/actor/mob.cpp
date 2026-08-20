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

#include "bedrock/world/actor/mob.h"

#include "bedrock/entity/components/mob_body_rotation_component.h"
#include "bedrock/entity/components/no_action_time_component.h"
#include "bedrock/world/actor/actor_flags.h"
#include "bedrock/world/level/level.h"

Mob *Mob::tryGetFromEntity(EntityContext &entity, bool include_removed)
{
    auto *actor = Actor::tryGetFromEntity(entity, include_removed);
    if (!actor) {
        return nullptr;
    }
    if (!actor->isType(ActorType::Mob) && !actor->hasCategory(ActorCategory::Mob)) {
        return nullptr;
    }
    return static_cast<Mob *>(actor);
}

bool Mob::isGliding() const
{
    return getStatusFlag(ActorFlags::GLIDING);
}

bool Mob::isSprinting() const
{
    return getStatusFlag(ActorFlags::SPRINTING);
}

void Mob::setYBodyRotation(float rotation)
{
    getPersistentComponent<MobBodyRotationComponent>()->y_body_rot = rotation;
}

void Mob::resetNoActionTime()
{
    getPersistentComponent<NoActionTimeComponent>()->value = 0;
}

bool Mob::checkForPostHitDamageImmunity(float damage_difference, const ActorDamageSource &source)
{
    return !getLevel().isClientSide() && (invulnerable_time != 10 || !getChainedDamageEffects()) &&
           source.getCause() != ActorDamageCause::SelfDestruct && invulnerable_time > 0 && damage_difference <= 0.0;
}
