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

#include "endstone/damage/damage_source.h"
#include "endstone/event/actor/actor_event.h"

namespace endstone {

/**
 * @brief Called when an Actor dies
 */
class ActorDeathEvent : public ActorEvent<Mob> {
public:
    ENDSTONE_EVENT(ActorDeathEvent);

    ActorDeathEvent(Mob &actor, std::unique_ptr<DamageSource> damage_source, int dropped_exp)
        : ActorEvent(actor), damage_source_(std::move(damage_source)), drop_exp_(dropped_exp)
    {
    }

    /**
     * @brief Gets the source of damage which caused the death.
     *
     * @return a DamageSource detailing the source of the damage for the death.
     */
    [[nodiscard]] DamageSource &getDamageSource() const
    {
        return *damage_source_;
    }

    /**
     * @brief Gets how much EXP should be dropped from this death.
     *
     * @note This does not indicate how much EXP should be taken from the entity in
     * question, merely how much should be created after its death.
     *
     * @return Amount of EXP to drop.
     */
    [[nodiscard]] int getDroppedExp() const
    {
        return drop_exp_;
    }

    /**
     * @brief Sets how much EXP should be dropped from this death.
     *
     * @note This does not indicate how much EXP should be taken from the entity in
     * question, merely how much should be created after its death.
     *
     * @param exp Amount of EXP to drop.
     */
    void setDroppedExp(int exp)
    {
        drop_exp_ = exp;
    }

private:
    std::unique_ptr<DamageSource> damage_source_;
    int drop_exp_;
};

}  // namespace endstone
