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

#include <vector>

#include "endstone/actor/actor.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/event.h"
#include "endstone/util/pointers.h"

namespace endstone {

/**
 * Called when two Actors collide with each other.
 *
 * If this event is cancelled, the Actors will not be pushed away from each other. Cancelling also stops either
 * Actor from being pulled onto the other when the other is a rideable vehicle, so a listener that cancels every
 * collision also stops boats and minecarts from being boarded by walking into them.
 *
 * The server fires this before it decides whether the collision leads to a push, so it is also called for pairs
 * the server then leaves alone, and it is called more than once per tick for a pair that keeps overlapping.
 */
class ActorCollideWithActorEvent final : public Cancellable<Event> {
public:
    ENDSTONE_EVENT(ActorCollideWithActorEvent);

    ActorCollideWithActorEvent(const NotNull<Actor> &actor1, const NotNull<Actor> &actor2) : actors_{actor1, actor2} {}

    /**
     * Returns the Actors involved in this event.
     *
     * @return Actors that are involved in this event
     */
    [[nodiscard]] const std::vector<NotNull<Actor>> &getActors() const { return actors_; }

private:
    std::vector<NotNull<Actor>> actors_;
};

}  // namespace endstone
