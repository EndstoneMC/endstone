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

#include "endstone/block/sign.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * Called when a player begins editing a sign's text.
 *
 * Cancelling this event stops the sign editing menu from opening.
 */
class PlayerOpenSignEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerOpenSignEvent);

    /**
     * Represents the cause of the sign opening.
     */
    enum class Cause {
        /** The event was triggered by the placement of a sign. */
        Place,
        /** The event was triggered by an interaction with a sign. */
        Interact,
        /** The event was triggered via a plugin. */
        Plugin,
        /** Fallback cause for an unknown source. */
        Unknown,
    };

    PlayerOpenSignEvent(const NotNull<Player> &player, Sign &sign, Sign::Side side, Cause cause)
        : Cancellable(player), sign_(sign), side_(side), cause_(cause)
    {
    }

    /**
     * Gets a captured state of the sign involved in this event.
     *
     * @return the sign being opened
     */
    [[nodiscard]] Sign &getSign() const { return sign_; }

    /**
     * Gets the side of the sign being opened.
     *
     * @return the sign side being opened
     */
    [[nodiscard]] Sign::Side getSide() const { return side_; }

    /**
     * Gets the cause of the sign opening.
     *
     * @return the sign opening cause
     */
    [[nodiscard]] Cause getCause() const { return cause_; }

private:
    Sign &sign_;
    Sign::Side side_;
    Cause cause_;
};

}  // namespace endstone
