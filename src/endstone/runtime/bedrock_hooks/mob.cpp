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
#include "endstone/event/actor/actor_knockback_prepare_event.h"
#include "endstone/runtime/hook.h"

void Mob::knockback(Actor *source, float damage, float dx, float dz, const KnockbackParameters &parameters)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    const auto &plugin_manager = server.getEndstonePluginManager();
    const bool has_prepare_event = plugin_manager.isEventRegistered<endstone::ActorKnockbackPrepareEvent>();
    const bool has_knockback_event = plugin_manager.isEventRegistered<endstone::ActorKnockbackEvent>();
    if (!has_prepare_event && !has_knockback_event) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&Mob::knockback, this, source, damage, dx, dz, parameters);
        return;
    }

    auto mob = getEndstoneActor<endstone::core::EndstoneMob>();
    endstone::Nullable<endstone::Actor> endstone_source;
    if (source != nullptr) {
        endstone_source = endstone::Nullable<endstone::Actor>{source->getEndstoneActor()};
    }
    auto adjusted_damage = damage;
    auto adjusted_dx = dx;
    auto adjusted_dz = dz;
    auto adjusted_parameters = parameters;

    if (has_prepare_event) {
        endstone::KnockbackParameters event_parameters{
            parameters.power.x,
            parameters.power.y,
            parameters.vertical_velocity_cap,
            parameters.slowdown_scale,
            parameters.scale_with_damage,
            parameters.slow_down_attacker,
            parameters.check_legacy_pre_nether_update_knockback,
            parameters.extra_knockback_power,
            static_cast<endstone::KnockbackParameters::ExtraKnockbackApproach>(parameters.extra_knockback_approach),
        };
        endstone::ActorKnockbackPrepareEvent e{mob, endstone_source, damage, {dx, 0.0F, dz}, event_parameters};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return;
        }

        const auto direction = e.getDirection();
        const auto &updated_parameters = e.getParameters();
        adjusted_damage = e.getDamage();
        adjusted_dx = direction.getX();
        adjusted_dz = direction.getZ();
        adjusted_parameters.power = {updated_parameters.getHorizontalPower(), updated_parameters.getVerticalPower()};
        adjusted_parameters.vertical_velocity_cap = updated_parameters.getVerticalVelocityCap();
        adjusted_parameters.slowdown_scale = updated_parameters.getSlowdownScale();
        adjusted_parameters.scale_with_damage = updated_parameters.isScaleWithDamage();
        adjusted_parameters.slow_down_attacker = updated_parameters.isSlowDownAttacker();
        adjusted_parameters.check_legacy_pre_nether_update_knockback = updated_parameters.isCheckLegacyKnockback();
        adjusted_parameters.extra_knockback_power = updated_parameters.getExtraKnockbackPower();
        adjusted_parameters.extra_knockback_approach =
            static_cast<ExtraKnockbackApproach>(updated_parameters.getExtraKnockbackApproach());
    }

    const auto before = getPosDelta();
    ENDSTONE_HOOK_CALL_ORIGINAL(&Mob::knockback, this, source, adjusted_damage, adjusted_dx, adjusted_dz,
                                adjusted_parameters);
    if (!has_knockback_event) {
        return;
    }

    const auto after = getPosDelta();
    auto diff = after - before;

    endstone::ActorKnockbackEvent e{mob, endstone_source, {diff.x, diff.y, diff.z}};
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
