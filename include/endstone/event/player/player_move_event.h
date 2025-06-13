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
#include "endstone/level/location.h"

namespace endstone {

/**
 * @brief Called when a player moves.
 */
class PlayerMoveEvent : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerMoveEvent);

    explicit PlayerMoveEvent(Player &player, Location from, Location to) : Cancellable(player), from_(from), to_(to) {}

    /**
     * @brief Gets the location this player moved from
     *
     * @return Location the player moved from
     */
    [[nodiscard]] const Location &getFrom() const
    {
        return from_;
    }

    /**
     * @brief Sets the location to mark as where the player moved from
     *
     * @param from New location to mark as the players previous location
     */
    Result<void> setFrom(const Location &from)
    {
        ENDSTONE_CHECKF(from.getDimension() != nullptr, "Cannot set a location with no associated dimension!")
        from_ = from;
        return {};
    }

    /**
     * @brief Gets the location this player moved to
     *
     * @return Location the player moved to
     */
    [[nodiscard]] const Location &getTo() const
    {
        return to_;
    }

    /**
     * @brief Sets the location that this player will move to
     *
     * @param to New Location this player will move to
     */
    Result<void> setTo(const Location &to)
    {
        ENDSTONE_CHECKF(to.getDimension() != nullptr, "Cannot set a location with no associated dimension!")
        to_ = to;
        return {};
    }

    Location from_;
    Location to_;
};

}  // namespace endstone
