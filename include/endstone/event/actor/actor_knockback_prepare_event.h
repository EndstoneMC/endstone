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

#include <utility>

#include "endstone/actor/knockback_parameters.h"
#include "endstone/actor/mob.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

/**
 * Called before Bedrock calculates the knockback received by a mob.
 *
 * The damage, horizontal direction and every native KnockbackParameters value may be changed before they are passed
 * to Bedrock. Cancelling the event prevents the knockback calculation entirely. Bedrock may perform an additional
 * calculation when applying extra knockback, in which case this event is called for each calculation.
 */
class ActorKnockbackPrepareEvent final : public Cancellable<ActorEvent<Mob>> {
public:
    ENDSTONE_EVENT(ActorKnockbackPrepareEvent);

    ActorKnockbackPrepareEvent(const NotNull<Mob> &mob, const Nullable<Actor> &source, float damage, Vector direction,
                               KnockbackParameters parameters)
        : Cancellable(mob), source_(source), damage_(damage), direction_(direction), parameters_(std::move(parameters))
    {
    }

    /**
     * Gets the source actor that caused the knockback, if one exists.
     *
     * @return the source actor, or null if the knockback was not caused by an actor
     */
    [[nodiscard]] const Nullable<Actor> &getSource() const { return source_; }

    /**
     * Gets the damage used when scaling the knockback power.
     *
     * @return the damage
     */
    [[nodiscard]] float getDamage() const { return damage_; }

    /**
     * Sets the damage used when scaling the knockback power.
     *
     * @param damage the damage
     */
    void setDamage(float damage) { damage_ = damage; }

    /**
     * Gets the horizontal direction of the knockback.
     *
     * The Y component is ignored by Bedrock.
     *
     * @return the knockback direction
     */
    [[nodiscard]] Vector getDirection() const { return direction_; }

    /**
     * Sets the horizontal direction of the knockback.
     *
     * The Y component is ignored by Bedrock.
     *
     * @param direction the knockback direction
     */
    void setDirection(Vector direction) { direction_ = direction; }

    /**
     * Gets the parameters used to calculate the knockback.
     *
     * Changes made to the returned value are passed to Bedrock.
     *
     * @return the knockback parameters
     */
    [[nodiscard]] KnockbackParameters &getParameters() { return parameters_; }

    /**
     * Gets the parameters used to calculate the knockback.
     *
     * @return the knockback parameters
     */
    [[nodiscard]] const KnockbackParameters &getParameters() const { return parameters_; }

private:
    Nullable<Actor> source_;
    float damage_;
    Vector direction_;
    KnockbackParameters parameters_;
};

}  // namespace endstone
