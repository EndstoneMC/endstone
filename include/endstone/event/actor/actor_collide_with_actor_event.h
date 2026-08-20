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

#include "endstone/actor/actor.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

/**
 * Called when an Actor collides with another Actor.
 *
 * The server fires this before it decides whether the collision leads to a push, so it is also called for pairs
 * the server then leaves alone, and it is called more than once per tick for a pair that keeps overlapping.
 *
 * If this event is cancelled, the two Actors are not pushed apart. Cancelling also stops either Actor from being
 * pulled onto the other when the other is a rideable vehicle, so a listener that cancels every collision also
 * stops boats and minecarts from being boarded by walking into them.
 */
class ActorCollideWithActorEvent final : public Cancellable<ActorEvent<Actor>> {
public:
    ENDSTONE_EVENT(ActorCollideWithActorEvent);

    ActorCollideWithActorEvent(const NotNull<Actor> &actor, const NotNull<Actor> &target)
        : Cancellable(actor), target_(target)
    {
    }

    /**
     * Returns the other Actor involved in this collision.
     *
     * @return Actor that the Actor of this event collided with
     */
    [[nodiscard]] const NotNull<Actor> &getTarget() const { return target_; }

private:
    NotNull<Actor> target_;
};

}  // namespace endstone
