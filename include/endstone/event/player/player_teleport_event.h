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
#include "endstone/level/location.h"

namespace endstone {

/**
 * @brief Called when a player is teleported from one location to another.
 */
class PlayerTeleportEvent : public PlayerEvent {
public:
    explicit PlayerTeleportEvent(Player &player, Location from, Location to) : PlayerEvent(player), from_(from), to_(to)
    {
    }
    ~PlayerTeleportEvent() override = default;

    inline static const std::string NAME = "PlayerTeleportEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    /**
     * @brief Gets the location that this player moved from
     *
     * @return Location this player moved from
     */
    [[nodiscard]] const Location &getFrom() const
    {
        return from_;
    }

    /**
     * @brief Sets the location that this player moved from
     *
     * @param from New location this player moved from
     */
    void setFrom(const Location &from)
    {
        from_ = from;
    }

    /**
     * @brief Gets the location that this player moved to
     *
     * @return Location this player moved to
     */
    [[nodiscard]] const Location &getTo() const
    {
        return to_;
    }

    /**
     * @brief Sets the location that this player moved to
     *
     * @param to New Location this player moved to
     */
    void setTo(const Location &to)
    {
        to_ = to;
    }

private:
    Location from_;
    Location to_;
};

}  // namespace endstone
