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

#include "endstone/event/player/player_event.h"

namespace endstone {

class Actor;

/**
 * Called when a visible actor is hidden from a player.
 *
 * This event is only called when the actor's visibility status is actually changed. It is called regardless of
 * whether the actor is within the player's tracking range.
 *
 * @see Player::hideEntity()
 */
class PlayerHideEntityEvent final : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerHideEntityEvent);

    PlayerHideEntityEvent(const NotNull<Player> &player, const NotNull<Actor> &entity)
        : PlayerEvent(player), entity_(entity)
    {
    }

    /**
     * Gets the actor hidden from the player.
     *
     * @return the hidden actor
     */
    [[nodiscard]] const NotNull<Actor> &getEntity() const { return entity_; }

private:
    NotNull<Actor> entity_;
};

}  // namespace endstone
