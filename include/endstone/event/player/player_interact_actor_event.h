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
 * @brief Represents an event that is called when a player right-clicks an actor.
 */
class PlayerInteractActorEvent : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerInteractActorEvent);
    explicit PlayerInteractActorEvent(Player &player, Actor &actor) : Cancellable(player), actor_(actor) {}
    ~PlayerInteractActorEvent() override = default;

    /**
     * @brief Gets the actor that was right-clicked by the player.
     *
     * @return actor right-clicked by player
     */
    [[nodiscard]] Actor &getActor() const { return actor_; }

private:
    Actor &actor_;
};

}  // namespace endstone
