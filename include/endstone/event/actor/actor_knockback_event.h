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

#include "endstone/actor/mob.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

/**
 * Called when a living entity receives knockback.
 */
class ActorKnockbackEvent : public Cancellable<ActorEvent<Mob>> {
public:
    ENDSTONE_EVENT(ActorKnockbackEvent);

    ActorKnockbackEvent(const NotNull<Mob> &mob, const Nullable<Actor> &source, const float force, Vector knockback)
        : Cancellable(mob), source_(source), force_(force), knockback_(knockback)
    {
    }

    /**
     * Get the source actor that has caused knockback to the defender, if exists.
     *
     * @return actor that caused knockback, or null if the knockback is not caused by an actor.
     */
    [[nodiscard]] const Nullable<Actor> &getSource() const { return source_; }

    /**
     * Gets the raw force of the knockback.
     *
     * @return the knockback force
     */
    [[nodiscard]] float getForce() const { return force_; }

    /**
     * Gets the knockback that will be applied to the entity.
     *
     * <p>
     * <b>Note:</b> this is the knockback before the entity's knockback resistance is taken into account, and this
     * method returns a copy; changes must be applied via setKnockback().
     *
     * @return the knockback
     */
    [[nodiscard]] Vector getKnockback() const { return knockback_; }

    /**
     * Sets the knockback that will be applied to the entity.
     *
     * @param knockback the knockback to apply
     */
    void setKnockback(Vector knockback) { knockback_ = knockback; }

private:
    Nullable<Actor> source_;
    float force_;
    Vector knockback_;
};

}  // namespace endstone
