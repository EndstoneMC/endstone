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
 * Called when a player clicks a recipe in the recipe book.
 *
 * @note If the event is cancelled the recipe will not be crafted and no ingredients will be consumed.
 */
class PlayerRecipeBookClickEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerRecipeBookClickEvent);

    PlayerRecipeBookClickEvent(const NotNull<Player> &player, std::string recipe, int amount)
        : Cancellable(player), recipe_(std::move(recipe)), amount_(amount)
    {
    }

    /**
     * Gets the identifier of the recipe clicked by the player.
     *
     * @return the recipe identifier
     */
    [[nodiscard]] const std::string &getRecipe() const { return recipe_; }

    /**
     * Gets the number of times the recipe is being crafted.
     *
     * @return the number of crafts
     */
    [[nodiscard]] int getAmount() const { return amount_; }

    /**
     * Sets the number of times the recipe is being crafted.
     *
     * @note Values are clamped to the 0-255 range the server accepts.
     *
     * @param amount the number of crafts
     */
    void setAmount(int amount) { amount_ = amount; }

private:
    std::string recipe_;
    int amount_;
};

}  // namespace endstone
