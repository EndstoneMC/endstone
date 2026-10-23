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

#include "endstone/check.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/level/location.h"

namespace endstone {

/**
 * Called when a player jumps.
 *
 * If this event is cancelled, the player will be moved back to the location given by getFrom(). This will not call an
 * event.
 */
class PlayerJumpEvent : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerJumpEvent);

    explicit PlayerJumpEvent(const NotNull<Player> &player, Location from, Location to)
        : Cancellable(player), from_(std::move(from)), to_(std::move(to))
    {
    }

    /**
     * Gets the location this player jumped from.
     *
     * @return Location the player jumped from
     */
    [[nodiscard]] const Location &getFrom() const { return from_; }

    /**
     * Sets the location to mark as where the player jumped from.
     *
     * @param from New location to mark as the players previous location
     * @throws std::invalid_argument if the location has no dimension
     */
    void setFrom(const Location &from)
    {
        Preconditions::checkArgument(static_cast<bool>(from.getDimension()), "Location has no dimension.");
        from_ = from;
    }

    /**
     * Gets the location this player jumped to.
     *
     * This information is based on what the client sends, it typically has little relation to the arc of the jump at
     * any given point.
     *
     * @return Location the player jumped to
     */
    [[nodiscard]] const Location &getTo() const { return to_; }

private:
    Location from_;
    Location to_;
};

}  // namespace endstone
