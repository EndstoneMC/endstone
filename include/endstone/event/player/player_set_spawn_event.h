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

#include <optional>
#include <utility>

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/level/location.h"

namespace endstone {

/**
 * Called when a player's spawn is set, either by themselves or otherwise.
 *
 * Assigning a new location through setLocation() redirects the spawn that is about to be written; cancelling leaves
 * the respawn point untouched.
 *
 * @note Only the location's block coordinates and dimension are written back; Bedrock does not persist yaw/pitch for
 * a respawn point. Cancelling stops the respawn point from changing, but not the feedback around it: `/spawnpoint`
 * still reports success and a respawn anchor still plays its sound and message, because neither consults the setter.
 * The event is not fired when Bedrock clears a respawn point, so `/clearspawnpoint` and breaking the bed a player is
 * bound to are both silent.
 */
class PlayerSetSpawnEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerSetSpawnEvent);

    /**
     * Represents the cause of the spawn change.
     */
    enum class Cause {
        /** When a player interacts successfully with a bed. */
        Bed,
        /** When a player interacts successfully with a respawn anchor. */
        RespawnAnchor,
        /** When the spawnpoint command, or the script API equivalent, is used on a player. */
        Command,
        /** When a plugin uses Player::setRespawnLocation. */
        Plugin,
        /** Fallback cause. */
        Unknown,
    };

    PlayerSetSpawnEvent(const NotNull<Player> &player, Cause cause, std::optional<Location> location)
        : Cancellable(player), cause_(cause), location_(std::move(location))
    {
    }

    /**
     * Gets the cause of this event.
     *
     * @return the cause
     */
    [[nodiscard]] Cause getCause() const { return cause_; }

    /**
     * Gets the location that the spawn is set to.
     *
     * @return the spawn location, or std::nullopt if removing the location
     */
    [[nodiscard]] const std::optional<Location> &getLocation() const { return location_; }

    /**
     * Sets the location to be set as the spawn location.
     *
     * @param location the spawn location, or std::nullopt to remove the spawn location
     */
    void setLocation(std::optional<Location> location) { location_ = std::move(location); }

private:
    Cause cause_;
    std::optional<Location> location_;
};

}  // namespace endstone
