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

#include <cmath>
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

void Mob::knockback(Actor *source, float damage, float dx, float dz, const KnockbackParameters &parameters)
{
    auto scale = 1.0F;
    if (parameters.extra_knockback_approach == ExtraKnockbackApproach::MultiplyReduced &&
        parameters.extra_knockback_power > 0.0F) {
        scale *= 1.0F + parameters.extra_knockback_power * 0.1F;
    }
    if (parameters.scale_with_damage) {
        scale *= std::fmax(std::sqrt(damage), 1.0F);
    }

    const auto length = std::sqrt(dx * dx + dz * dz);
    const auto horizontal = parameters.power.x * scale;
    const auto raw_x = length < 0.0001F ? 0.0F : -dx / length * horizontal;
    const auto raw_y = parameters.power.y * scale * 1.2F;
    const auto raw_z = length < 0.0001F ? 0.0F : -dz / length * horizontal;

    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::ActorKnockbackEvent e{getEndstoneActor<endstone::core::EndstoneMob>(),
                                    source == nullptr ? nullptr : source->getEndstoneActor(),
                                    horizontal,
                                    {raw_x, raw_y, raw_z}};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return;
    }

    const auto knockback = e.getKnockback();
    if (knockback.getX() == raw_x && knockback.getY() == raw_y && knockback.getZ() == raw_z) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&Mob::knockback, this, source, damage, dx, dz, parameters);
        return;
    }

    auto adjusted = parameters;
    adjusted.power.x = std::sqrt(knockback.getX() * knockback.getX() + knockback.getZ() * knockback.getZ()) / scale;
    adjusted.power.y = knockback.getY() / 1.2F / scale;
    ENDSTONE_HOOK_CALL_ORIGINAL(&Mob::knockback, this, source, damage, -knockback.getX(), -knockback.getZ(), adjusted);
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
