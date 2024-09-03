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
#include "bedrock/world/actor/actor_flags.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/event/actor/actor_death_event.h"
#include "endstone/event/actor/actor_knockback_event.h"

using endstone::detail::EndstoneServer;

void Mob::die(const ActorDamageSource &source)
{
    if (!isPlayer()) {
        auto &server = entt::locator<EndstoneServer>::value();
        endstone::ActorDeathEvent e{getEndstoneActor()};
        server.getPluginManager().callEvent(e);
    }

    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Mob::die, __FUNCDNAME__, this, source);
}

void Mob::knockback(Actor *source, int damage, float dx, float dz, float horizontal_force, float vertical_force,
                    float height_cap)
{
    auto before = getPosDelta();
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Mob::knockback, __FUNCDNAME__, this, source, damage, dx, dz, horizontal_force,
                                     vertical_force, height_cap);
    auto after = getPosDelta();
    auto diff = after - before;

    auto &server = entt::locator<EndstoneServer>::value();
    endstone::ActorKnockbackEvent e{getEndstoneMob(),
                                    source == nullptr ? nullptr : &source->getEndstoneActor(),
                                    {diff.x, diff.y, diff.z},
                                    {after.x, after.y, after.z}};
    server.getPluginManager().callEvent(e);

    auto knockback = e.getKnockback();
    diff = e.isCancelled() ? Vec3::ZERO : (Vec3{knockback.getX(), knockback.getY(), knockback.getZ()} - before);
    setPosDelta(before + diff);
}

bool Mob::isGliding() const
{
    return getStatusFlag(ActorFlags::GLIDING);
}

void Mob::setYBodyRotation(float rotation)
{
    getPersistentComponent<MobBodyRotationComponent>()->y_body_rot = rotation;
}

endstone::detail::EndstoneMob &Mob::getEndstoneMob() const
{
    return static_cast<endstone::detail::EndstoneMob &>(getEndstoneActor());
}
