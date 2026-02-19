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

namespace endstone {

/**
 * @brief Called when a player changes their skin.
 */
class PlayerSkinChangeEvent : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerSkinChangeEvent);

    explicit PlayerSkinChangeEvent(Player &player, Skin new_skin, std::optional<Message> message)
        : Cancellable(player), new_skin_(std::move(new_skin)), message_(std::move(message))
    {
    }

    /**
     * @brief Gets the player's new skin.
     *
     * @return The skin that will be applied.
     */
    [[nodiscard]] Skin getNewSkin() const { return new_skin_; }

    /**
     * @brief Gets the message to send to all online players for this skin change.
     *
     * @return Message to appear to other players on the server.
     */
    [[nodiscard]] std::optional<Message> getSkinChangeMessage() const { return message_; }

    /**
     * @brief Sets the message to send to all online players for this skin change.
     *
     * @param message Message to appear to other players on the server.
     */
    void setSkinChangeMessage(std::optional<Message> message) { message_ = std::move(message); }

private:
    Skin new_skin_;
    std::optional<Message> message_;
};

}  // namespace endstone
