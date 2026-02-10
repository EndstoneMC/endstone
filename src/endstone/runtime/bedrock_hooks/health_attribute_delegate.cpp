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

#include "bedrock/world/attribute/health_attribute_delegate.h"

#include <iostream>

#include "bedrock/world/actor/mob.h"
#include "endstone/core/actor/mob.h"
#include "endstone/core/damage/damage_source.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_damage_event.h"
#include "endstone/runtime/hook.h"

std::optional<float> HealthAttributeDelegate::change(float old_value, float new_value, const AttributeBuff &buff)
{
    // TODO(refactor): use ActorBeforeHurtEvent?
    const auto damage = old_value - new_value;
    if (damage <= 0) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&HealthAttributeDelegate::change, this, old_value, new_value, buff);
    }

    // Calculate last hurt damage independently of the final damage set in the ActorDamageEvent.
    // This ensures that during the invulnerable window, damage tracking is accurate.
    // Without this, if the damage is reduced in ActorDamageEvent, the server might incorrectly
    // allow further damage during the invulnerability period, effectively bypassing it.
    auto last_hurt = mob_->getLastHurtDamage();
    if (mob_->invulnerable_time <= 0) {
        last_hurt = damage;
    }
    else {
        last_hurt += damage;
    }

    const auto &source = buff.getSource();
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    auto &mob = mob_->getEndstoneActor<endstone::core::EndstoneMob>();
    endstone::ActorDamageEvent e{mob, std::make_unique<endstone::core::EndstoneDamageSource>(source), damage};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        // Remove the flag to signal the damage is cancelled
        // See also: Mob::_hurt
        mob_->addOrRemoveComponent<endstone::core::MobHurtFlagComponent>(false);
        return old_value;
    }
    new_value = ENDSTONE_HOOK_CALL_ORIGINAL(&HealthAttributeDelegate::change,  //
                                            this, old_value, old_value - e.getDamage(), buff);

    // Make sure the correct value of last hurt damage is set (see notes above)
    mob_->setLastHurtDamage(last_hurt);
    return new_value;
}
