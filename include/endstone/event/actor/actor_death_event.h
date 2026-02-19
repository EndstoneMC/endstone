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

#pragma once

#include <memory>
#include <utility>

#include "endstone/damage/damage_source.h"
#include "endstone/event/actor/actor_event.h"

namespace endstone {

/**
 * @brief Called when an Actor dies
 */
class ActorDeathEvent : public ActorEvent<Mob> {
public:
    ENDSTONE_EVENT(ActorDeathEvent);
    ActorDeathEvent(Mob &actor, std::unique_ptr<DamageSource> damage_source)
        : ActorEvent(actor), damage_source_(std::move(damage_source))
    {
    }

    /**
     * @brief Gets the source of damage which caused the death.
     *
     * @return a DamageSource detailing the source of the damage for the death.
     */
    [[nodiscard]] DamageSource &getDamageSource() const { return *damage_source_; }

private:
    std::unique_ptr<DamageSource> damage_source_;
    // TODO(event): add drops and dropExp
};

}  // namespace endstone
