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

#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/player.h"

namespace endstone {

/**
 * Called when a player's food level changes.
 *
 * If this event is cancelled, the player's food level will not change.
 */
class FoodLevelChangeEvent final : public Cancellable<ActorEvent<Mob>> {
public:
    ENDSTONE_EVENT(FoodLevelChangeEvent);

    FoodLevelChangeEvent(const NotNull<Player> &player, int food_level, std::optional<ItemStack> item)
        : Cancellable(player), player_(player), food_level_(food_level), item_(std::move(item))
    {
    }

    /**
     * Gets the player whose food level is changing.
     *
     * @return The player involved in this event.
     */
    [[nodiscard]] const NotNull<Player> &getActor() const { return player_; }

    /**
     * Gets the item that triggered this event, if any.
     *
     * @return A copy of the ItemStack being consumed, or `std::nullopt` if no item triggered the change.
     */
    [[nodiscard]] std::optional<ItemStack> getItem() const { return item_; }

    /**
     * Gets the resultant food level the player should be set to.
     *
     * A value of 20 is a full food bar and 0 is an empty one.
     *
     * @return The resultant food level.
     */
    [[nodiscard]] int getFoodLevel() const { return food_level_; }

    /**
     * Sets the resultant food level the player should be set to.
     *
     * Values below 0 are clamped to 0.
     *
     * @param food_level The resultant food level.
     */
    void setFoodLevel(int food_level)
    {
        if (food_level < 0) {
            food_level = 0;
        }
        food_level_ = food_level;
    }

private:
    NotNull<Player> player_;
    int food_level_;
    std::optional<ItemStack> item_;
};

}  // namespace endstone
