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
#include "endstone/event/cancellable.h"

namespace endstone {

/**
 * @brief Called when an Actor is damaged
 */
class ActorDamageEvent : public Cancellable<ActorEvent<Mob>> {
public:
    ENDSTONE_EVENT(ActorDamageEvent);
    ActorDamageEvent(Mob &actor, std::unique_ptr<DamageSource> damage_source, const float damage)
        : Cancellable(actor), damage_source_(std::move(damage_source)), damage_(damage)
    {
    }

    /**
     * @brief Gets the raw amount of damage caused by the event
     *
     * @return The raw amount of damage caused by the event
     */
    [[nodiscard]] float getDamage() const { return damage_; }

    /**
     * @brief Sets the raw amount of damage caused by the event.
     *
     * @param damage The raw amount of damage caused by the event
     */
    void setDamage(const float damage) { damage_ = damage; }

    /**
     * @brief Get the source of damage.
     *
     * @return a DamageSource detailing the source of the damage.
     */
    [[nodiscard]] DamageSource &getDamageSource() const { return *damage_source_; }

private:
    std::unique_ptr<DamageSource> damage_source_;
    float damage_;
};

}  // namespace endstone
