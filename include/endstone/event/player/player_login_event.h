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
 * @brief Called when a player attempts to login in.
 */
class PlayerLoginEvent : public PlayerEvent {
public:
    explicit PlayerLoginEvent(Player &player, std::string message = "")
        : PlayerEvent(player), message_(std::move(message))
    {
    }
    ~PlayerLoginEvent() override = default;

    inline static const std::string NAME = "PlayerLoginEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    /**
     * Gets the current kick message that will be used if event is cancelled
     *
     * @return Current kick message
     */
    [[nodiscard]] const std::string &getKickMessage() const
    {
        return message_;
    }

    /**
     * Sets the kick message to display if event is cancelled
     *
     * @param message New kick message
     */
    void setKickMessage(const std::string &message)
    {
        message_ = message;
    }

private:
    std::string message_;
};

}  // namespace endstone
