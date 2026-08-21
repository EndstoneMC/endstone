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

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * Called when a player picks up an experience orb.
 *
 * Cancelling the event leaves the orb in the world.
 */
class PlayerPickupExperienceEvent : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerPickupExperienceEvent);

    explicit PlayerPickupExperienceEvent(const NotNull<Player> &player, int amount)
        : Cancellable(player), amount_(amount)
    {
    }

    /**
     * Gets the amount of experience the orb is worth.
     *
     * @return the experience amount
     */
    [[nodiscard]] int getAmount() const { return amount_; }

private:
    int amount_;
};

}  // namespace endstone
