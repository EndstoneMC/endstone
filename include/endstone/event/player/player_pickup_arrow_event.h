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

class Actor;

/**
 * Called when a player picks up an arrow or a thrown trident from the ground.
 */
class PlayerPickupArrowEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerPickupArrowEvent);

    PlayerPickupArrowEvent(const NotNull<Player> &player, const NotNull<Actor> &arrow)
        : Cancellable(player), arrow_(arrow)
    {
    }

    /**
     * Gets the arrow picked up by the player.
     *
     * @return the arrow picked up by the player
     */
    [[nodiscard]] const NotNull<Actor> &getArrow() const { return arrow_; }

private:
    NotNull<Actor> arrow_;
};

}  // namespace endstone
