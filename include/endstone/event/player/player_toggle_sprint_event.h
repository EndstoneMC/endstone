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
 * Called when a player toggles their sprinting state.
 */
class PlayerToggleSprintEvent final : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerToggleSprintEvent);
    explicit PlayerToggleSprintEvent(const NotNull<Player> &player, bool sprinting) : PlayerEvent(player), sprinting_(sprinting) {}

    /**
     * Returns whether the player is now sprinting or not.
     *
     * @return the new sprinting state
     */
    [[nodiscard]] bool isSprinting() const { return sprinting_; }

private:
    bool sprinting_;
};

}  // namespace endstone
