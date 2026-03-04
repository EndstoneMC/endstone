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
 * @brief Called when a player uses an emote.
 */
class PlayerEmoteEvent : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerEmoteEvent);
    explicit PlayerEmoteEvent(Player &player, std::string emote_id, bool muted)
        : Cancellable(player), emote_id_(std::move(emote_id)), muted_(muted)
    {
    }
    ~PlayerEmoteEvent() override = default;

    /**
     * @brief Gets the emote piece ID
     *
     * @return The emote piece ID
     */
    [[nodiscard]] std::string getEmoteId() const { return emote_id_; }

    /**
     * @brief Gets the muted state for the emote.
     *
     * This method determines whether the emote is being executed without sending a chat message about the emote.
     *
     * @return true if the emote is muted, false otherwise.
     */
    [[nodiscard]] bool isMuted() const { return muted_; }

    /**
     * @brief Sets the muted state for the emote.
     *
     * @note If set to true, the emote will be executed silently, and no chat messages will be sent.
     *
     * @param muted true to mute the emote and disable chat messages, false to unmute it.
     */
    void setMuted(bool muted) { muted_ = muted; }

private:
    std::string emote_id_;
    bool muted_;
};

}  // namespace endstone
