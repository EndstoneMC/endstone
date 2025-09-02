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

#include "endstone/block/block.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * @brief Called when a player is leaving a bed.
 */
class PlayerBedLeaveEvent final : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerBedLeaveEvent)

    explicit PlayerBedLeaveEvent(Player &player, Block &bed) : PlayerEvent(player), bed_(bed) {}
    ~PlayerBedLeaveEvent() override = default;

    /**
     * @brief Returns the bed block involved in this event.
     *
     * @return The bed block involved in this event.
     */
    [[nodiscard]] Block &getBed() const
    {
        return bed_;
    }

private:
    Block &bed_;
};

}  // namespace endstone
