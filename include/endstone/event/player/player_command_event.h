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
 * @brief Called whenever a player runs a command.
 */
class PlayerCommandEvent : public PlayerEvent {
public:
    explicit PlayerCommandEvent(Player &player, std::string command) : PlayerEvent(player), command_(std::move(command))
    {
    }
    ~PlayerCommandEvent() override = default;

    inline static const std::string NAME = "PlayerCommandEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    /**
     * Gets the command that the player is attempting to send.
     *
     * @return Command the player is attempting to send
     */
    [[nodiscard]] std::string getCommand() const
    {
        return command_;
    }

    /**
     * Sets the command that the player will send.
     *
     * @param command New command that the player will send
     */
    void setCommand(std::string command)
    {
        command_ = std::move(command);
    }

private:
    std::string command_;
};

}  // namespace endstone
