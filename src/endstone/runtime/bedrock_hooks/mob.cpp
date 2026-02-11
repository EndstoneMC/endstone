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

#include <iostream>

#include "bedrock/entity/components/damage_sensor_component.h"
#include "bedrock/entity/components/no_action_time_component.h"
#include "endstone/actor/actor.h"
#include "endstone/actor/mob.h"
#include "endstone/core/actor/mob.h"
#include "endstone/core/damage/damage_source.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_damage_event.h"
#include "endstone/event/actor/actor_knockback_event.h"
#include "endstone/runtime/hook.h"

void Mob::knockback(Actor *source, int damage, float dx, float dz, float horizontal_force, float vertical_force,
                    float height_cap)
{
    const auto before = getPosDelta();
    ENDSTONE_HOOK_CALL_ORIGINAL(&Mob::knockback, this, source, damage, dx, dz, horizontal_force, vertical_force,
                                height_cap);
    const auto after = getPosDelta();
    auto diff = after - before;

    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    endstone::ActorKnockbackEvent e{getEndstoneActor<endstone::core::EndstoneMob>(),
                                    source == nullptr ? nullptr : &source->getEndstoneActor(),
                                    {diff.x, diff.y, diff.z}};
    server.getPluginManager().callEvent(e);

    const auto knockback = e.getKnockback();
    diff = e.isCancelled() ? Vec3::ZERO : Vec3{knockback.getX(), knockback.getY(), knockback.getZ()};
    setPosDelta(before + diff);
}

// bool Mob::_hurt(const ActorDamageSource &source, float damage, bool knock, bool ignite)
// {
//     addOrRemoveComponent<endstone::core::MobHurtFlagComponent>(true);
//     auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&Mob::_hurt, this, source, damage, knock, ignite);
//     if (!hasComponent<endstone::core::MobHurtFlagComponent>()) {
//         // A related ActorDamageEvent is triggered and cancelled, propagate the result to the caller to prevent kb
//         // See also: HealthAttributeDelegate::change
//         return false;
//     }
//     addOrRemoveComponent<endstone::core::MobHurtFlagComponent>(false);
//     return result;
// }
