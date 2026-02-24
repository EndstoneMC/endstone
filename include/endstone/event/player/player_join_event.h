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

#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * @brief Called when a player joins a server.
 */
class PlayerJoinEvent : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerJoinEvent);

    explicit PlayerJoinEvent(Player &player, std::optional<Message> join_message)
        : PlayerEvent(player), join_message_(std::move(join_message))
    {
    }

    /**
     * @brief Gets the join message to send to all online players.
     *
     * @return Message to appear to other players on the server.
     */
    [[nodiscard]] std::optional<Message> getJoinMessage() const { return join_message_; }

    /**
     * @brief Sets the join message to send to all online players.
     *
     * @param message Message to appear to other players on the server.
     */
    void setJoinMessage(std::optional<Message> message) { join_message_ = std::move(message); }

private:
    std::optional<Message> join_message_;
};

}  // namespace endstone
