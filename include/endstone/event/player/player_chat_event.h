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
#include <string>
#include <utility>
#include <vector>

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * @brief Called when a player sends a chat message.
 */
class PlayerChatEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerChatEvent);
    explicit PlayerChatEvent(Player &player, std::string message, std::optional<std::vector<Player *>> recipients,
                             std::string format = "<{0}> {1}")
        : Cancellable(player), message_(std::move(message)), format_(std::move(format)),
          recipients_(std::move(recipients))
    {
    }

    /**
     * @brief Gets the message that the player is attempting to send.
     *
     * @return Message the player is attempting to send
     */
    [[nodiscard]] std::string getMessage() const { return message_; }

    /**
     * @brief Sets the message that the player will send.
     *
     * @param message New message that the player will send
     */
    void setMessage(std::string message) { message_ = std::move(message); }

    /**
     * @brief Sets the player that this message will display as
     *
     * @param player New player which this event will execute as
     */
    void setPlayer(Player &player) { player_ = player; }

    /**
     * @brief Gets the format to use to display this chat message
     *
     * See <a href="https://en.cppreference.com/w/cpp/utility/format/spec.html">the format string syntax</a>
     *
     * @return format string
     */
    [[nodiscard]] std::string getFormat() const { return format_; }

    /**
     * @brief Sets the format to use to display this chat message
     *
     * @param format format string
     */

    void setFormat(std::string format) { format_ = std::move(format); }

    /**
     * @brief Gets a set of recipients that this chat message will be displayed to
     *
     * @return All Players who will see this chat message
     */
    [[nodiscard]] std::vector<Player *> getRecipients() const
    {
        if (!recipients_) {
            recipients_ = player_.get().getServer().getOnlinePlayers();
        }
        return recipients_.value();
    }

private:
    std::string message_;
    std::string format_;
    mutable std::optional<std::vector<Player *>> recipients_;
};

}  // namespace endstone
