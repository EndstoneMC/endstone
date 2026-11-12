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

#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

/**
 * Called when an Actor stops riding another Actor.
 */
class ActorDismountEvent : public Cancellable<ActorEvent<Actor>> {
public:
    ENDSTONE_EVENT(ActorDismountEvent);

    explicit ActorDismountEvent(const NotNull<Actor> &actor, const NotNull<Actor> &dismounted, bool cancellable = true)
        : Cancellable(actor), dismounted_(dismounted), cancellable_(cancellable)
    {
    }

    void setCancelled(bool cancel) override
    {
        if (cancel && !cancellable_) {
            return;
        }
        Cancellable::setCancelled(cancel);
    }

    /**
     * Gets whether this dismount can be cancelled.
     *
     * Dismounts forced by the server, such as when the actor is destroyed, ignore cancellation.
     *
     * @return `true` if cancelling this event will keep the actor mounted
     */
    [[nodiscard]] bool isCancellable() const override { return cancellable_; }

    /**
     * Gets the actor which will no longer be ridden.
     *
     * @return dismounted actor
     */
    [[nodiscard]] const NotNull<Actor> &getDismounted() const { return dismounted_; }

private:
    NotNull<Actor> dismounted_;
    bool cancellable_;
};

}  // namespace endstone
