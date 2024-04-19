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

/**
 * Called when a player sends a chat message
 */
class PlayerChatEvent : public PlayerEvent {
public:
    explicit PlayerChatEvent(Player &player, std::string message) : PlayerEvent(player), message_(std::move(message)) {}
    ~PlayerChatEvent() override = default;

    inline static const std::string NAME = "PlayerChatEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    /**
     * Gets the message that the player is attempting to send.
     *
     * @return Message the player is attempting to send
     */
    [[nodiscard]] std::string getMessage() const
    {
        return message_;
    }

    /**
     * Sets the message that the player will send.
     *
     * @param message New message that the player will send
     */
    void setMessage(std::string message)
    {
        message_ = std::move(message);
    }

private:
    std::string message_;
};

}  // namespace endstone
