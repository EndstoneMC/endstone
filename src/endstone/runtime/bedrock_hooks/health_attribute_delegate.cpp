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
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_damage_event.h"
#include "endstone/runtime/hook.h"

float HealthAttributeDelegate::change(float old_value, float new_value, const AttributeBuff &buff)
{
    const auto original_damage = old_value - new_value;
    if (original_damage > 0) {
        const auto &source = buff.getSource();
        const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
        auto &mob = mob_->getEndstoneActor<endstone::core::EndstoneMob>();
        endstone::ActorDamageEvent e{mob, std::make_unique<endstone::core::EndstoneDamageSource>(source),
                                     original_damage};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return old_value;
        }
        new_value = old_value - e.getDamage();
    }

    new_value = ENDSTONE_HOOK_CALL_ORIGINAL(&HealthAttributeDelegate::change, this, old_value, new_value, buff);
    mob_->setLastHurtDamage(original_damage);
    return new_value;
}
