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

namespace endstone {

/**
 * Called when an Actor's gliding state is toggled with an elytra.
 */
class ActorToggleGlideEvent final : public ActorEvent<Mob> {
public:
    ENDSTONE_EVENT(ActorToggleGlideEvent)

    explicit ActorToggleGlideEvent(const NotNull<Mob> &actor, bool gliding) : ActorEvent(actor), gliding_(gliding) {}

    /**
     * Returns whether the actor is now gliding or not.
     *
     * @return the new gliding state
     */
    [[nodiscard]] bool isGliding() const { return gliding_; }

private:
    bool gliding_;
};

}  // namespace endstone
