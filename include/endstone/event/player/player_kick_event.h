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

#include <string>
#include <utility>

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * @brief Called when a player gets kicked from the server
 */
class PlayerKickEvent : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerKickEvent);

    explicit PlayerKickEvent(Player &player, std::string reason) : Cancellable(player), reason_(std::move(reason)) {}

    /**
     * @brief Gets the reason why the player is getting kicked
     *
     * @return string kick reason
     */
    [[nodiscard]] std::string getReason() const
    {
        return reason_;
    }

    /**
     * @brief Sets the reason why the player is getting kicked
     *
     * @param reason kick reason
     */
    void setReason(std::string reason)
    {
        reason_ = std::move(reason);
    }

private:
    std::string reason_;
};

}  // namespace endstone
