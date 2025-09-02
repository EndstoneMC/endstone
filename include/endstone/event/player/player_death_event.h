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

#include "endstone/event/actor/actor_death_event.h"
#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * @brief Called when a Player dies
 */
class PlayerDeathEvent : public ActorDeathEvent, public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerDeathEvent);

    explicit PlayerDeathEvent(Player &player, std::unique_ptr<DamageSource> damage_source,
                              std::optional<Message> death_message)
        : ActorDeathEvent(player, std::move(damage_source)), PlayerEvent(player),
          death_message_(std::move(death_message))
    {
    }

    /**
     * @brief Get the death message that will appear to everyone on the server.
     *
     * @return Message to appear to other players on the server.
     */
    [[nodiscard]] std::optional<Message> getDeathMessage() const
    {
        return death_message_;
    }

    /**
     * @brief Set the death message that will appear to everyone on the server.
     *
     * @param death_message Message to appear to other players on the server.
     */
    void setDeathMessage(std::optional<Message> death_message)
    {
        death_message_ = std::move(death_message);
    }

private:
    std::optional<Message> death_message_;

    // TODO(event): new exp, new level, new total exp, keep level, keep inventory
};

}  // namespace endstone
