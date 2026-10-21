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

#include "endstone/actor/item.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

/**
 * Called when an Actor picks an item up from the ground.
 *
 * This is not called for players; see PlayerPickupItemEvent instead.
 */
class ActorPickupItemEvent : public Cancellable<ActorEvent<Actor>> {
public:
    ENDSTONE_EVENT(ActorPickupItemEvent);

    explicit ActorPickupItemEvent(const NotNull<Actor> &actor, const NotNull<Item> &item, int remaining)
        : Cancellable(actor), item_(item), remaining_(remaining)
    {
    }

    /**
     * Gets the Item picked up by the actor.
     *
     * @return the item
     */
    [[nodiscard]] const NotNull<Item> &getItem() const { return item_; }

    /**
     * Gets the amount remaining on the ground, if any.
     *
     * It is the stack size minus what the actor takes, as its pickup rules decide (such as a piglin's shareables).
     * A hopper minecart takes what fits in the first slot with room for it, and its next pull calls another event for
     * the rest. A mob that takes the whole stack leaves 0.
     *
     * @return amount remaining on the ground
     */
    [[nodiscard]] int getRemaining() const { return remaining_; }

private:
    NotNull<Item> item_;
    int remaining_;
};

}  // namespace endstone
