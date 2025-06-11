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
 * @brief Called when a player is almost about to enter the bed.
 */
class PlayerBedEnterEvent : public Cancellable<PlayerEvent> {
public:
    enum class BedSleepingResult : int {
        OK = 0,
        NOT_POSSIBLE_HERE = 1,
        NOT_POSSIBLE_NOW = 2,
        TOO_FAR_AWAY = 3,
        OTHER_PROBLEM = 4,
        NOT_SAFE = 5,
        BED_OBSTRUCTED = 6,
    };

public:
    ENDSTONE_EVENT(PlayerBedEnterEvent)

    explicit PlayerBedEnterEvent(Player &player, Block &bed, BedSleepingResult bed_enter_result)
        : Cancellable(player), bed_(bed), bed_enter_result_(bed_enter_result)
    {
    }
    ~PlayerBedEnterEvent() override = default;

    /**
     * @brief Returns the bed block involved in this event.
     *
     * @return The bed block involved in this event.
     */
    [[nodiscard]] Block &getBed() const
    {
        return bed_;
    }

    /**
     * @brief This describes the default outcome of this event.
     *
     * @return The bed enter result representing the default outcome of this event
     */
    [[nodiscard]] BedSleepingResult getBedEnterResult()
    {
        return bed_enter_result_;
    }

    void setBedEnterResult(BedSleepingResult bed_enter_result)
    {
        bed_enter_result_ = bed_enter_result;
    }

private:
    Block &bed_;
    BedSleepingResult bed_enter_result_;
};

}  // namespace endstone
