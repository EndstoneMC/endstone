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

#include "endstone/actor/mob.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/potion/effect.h"

namespace endstone {

/**
 * Called when an effect on a Mob changes.
 *
 * This is fired before the change is applied. Cancelling the event prevents it, and the effect may be replaced with
 * a different one via setEffect.
 */
class ActorEffectEvent : public Cancellable<ActorEvent<Mob>> {
public:
    /**
     * An enum to specify how the effect changed.
     */
    enum class Action {
        Added,
    };

    ENDSTONE_EVENT(ActorEffectEvent);

    explicit ActorEffectEvent(const NotNull<Mob> &mob, Action action, Effect effect)
        : Cancellable(mob), action_(action), effect_(std::move(effect))
    {
    }

    /**
     * Gets how the effect changed.
     *
     * @return the action taken on the effect
     */
    [[nodiscard]] Action getAction() const { return action_; }

    /**
     * Gets the effect involved in this event.
     *
     * @return the effect
     */
    [[nodiscard]] const Effect &getEffect() const { return effect_; }

    /**
     * Sets the effect involved in this event.
     *
     * @param effect the effect to apply instead
     */
    void setEffect(Effect effect) { effect_ = std::move(effect); }

private:
    Action action_;
    Effect effect_;
};

}  // namespace endstone
