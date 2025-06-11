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
    ENDSTONE_EVENT(PlayerBedEnterEvent)

    /**
     * @brief Represents the default possible outcomes of this event.
     */
    enum class BedEnterResult : int {
        /**
         * The player will enter the bed.
         */
        Ok,
        /**
         * The dimension doesn't allow sleeping or saving the spawn point (eg,Nether, The End).
         */
        NotPossibleHere,
        /**
         * Entering the bed is prevented due to it not being night nor thundering currently.
         */
        NotPossibleNow,
        /**
         * Entering the bed is prevented due to the player being too far away.
         */
        TooFarAway,
        /**
         * Entering the bed is prevented due to there being monsters nearby.
         */
        NotSafe,
        /**
         * Entering the bed is prevented due to there being some other problem.
         */
        OtherProblem,
    };

    explicit PlayerBedEnterEvent(Player &player, Block &bed, BedEnterResult bed_enter_result)
        : Cancellable(player), bed_(bed), bed_enter_result_(bed_enter_result)
    {
    }

    /**
     * @brief This describes the default outcome of this event.
     *
     * @return The bed enter result representing the default outcome of this event
     */
    [[nodiscard]] BedEnterResult getBedEnterResult() const
    {
        return bed_enter_result_;
    }

    /**
     * @brief This controls the action to take with the bed that was clicked on.
     *
     * @return the action to take with the interacted bed
     */
    [[nodiscard]] Result useBed() const
    {
        return use_bed_;
    }

    /**
     * @brief Sets the action to take with the interacted bed.
     *
     * - Result::Allow will result in the player sleeping, regardless of the default outcome described by
     * getBedEnterResult().
     * - Result::Deny will prevent the player from sleeping. This has the same effect as canceling the event
     * - Result::Default will result in the outcome described by getBedEnterResult().
     *
     * @param use_bed the action to take with the interacted bed
     */
    void setUseBed(Result use_bed)
    {
        use_bed_ = use_bed;
    }

    /**
     * @brief Gets the cancellation state of this event. Set to true if you want to prevent the player from sleeping.
     *
     * @note Canceling the event has the same effect as setting useBed() to Result::Deny.
     *
     * @return boolean cancellation state
     */
    [[nodiscard]] bool isCancelled() const override
    {
        return use_bed_ == Result::Deny || (use_bed_ == Result::Default && bed_enter_result_ != BedEnterResult::Ok);
    }

    /**
     * @brief Sets the cancellation state of this event. A canceled event will not be executed in the server, but will
     * still pass to other plugins.
     *
     * @note Canceling this event will prevent use of the bed.
     *
     * @param cancel true if you wish to cancel this event
     */
    void setCancelled(bool cancel) override
    {
        if (useBed() == Result::Deny) {
            setUseBed(cancel ? Result::Deny : Result::Default);
        }
        else {
            setUseBed(cancel ? Result::Deny : useBed());
        }
    }

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
    BedEnterResult bed_enter_result_;
    Result use_bed_ = Result::Default;
};

}  // namespace endstone
