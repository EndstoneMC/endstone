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

#include <utility>
#include <vector>

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/inventory/recipe.h"

namespace endstone {

/**
 * Called when a player crafts an item, either inside a crafting grid or straight from the recipe book.
 *
 * @note If the event is cancelled the item will not be crafted and the ingredients will not be consumed.
 */
class PlayerCraftItemEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerCraftItemEvent);

    PlayerCraftItemEvent(const NotNull<Player> &player, Recipe recipe, std::vector<ItemStack> ingredients,
                         std::vector<ItemStack> results, int repetitions)
        : Cancellable(player), recipe_(std::move(recipe)), ingredients_(std::move(ingredients)),
          results_(std::move(results)), repetitions_(repetitions)
    {
    }

    /**
     * @return A copy of the current recipe on the crafting matrix.
     */
    [[nodiscard]] const Recipe &getRecipe() const { return recipe_; }

    /**
     * Gets the ingredients a single craft consumes.
     *
     * @note These are the items in the crafting grid where the player used one. Crafting from the recipe book never
     * fills the grid, so the ingredients then come from the recipe instead, and an ingredient that accepts several
     * items reports the one the recipe names rather than the one the player supplied.
     *
     * @return the ingredients the craft consumes
     */
    [[nodiscard]] const std::vector<ItemStack> &getIngredients() const { return ingredients_; }

    /**
     * Gets the items a single craft produces.
     *
     * A recipe usually produces one item, but may produce several, and an ingredient that leaves a remainder behind
     * contributes one too.
     *
     * @return the items the craft produces
     */
    [[nodiscard]] const std::vector<ItemStack> &getResults() const { return results_; }

    /**
     * Sets the items a single craft produces.
     *
     * @note Results are replaced one for one, so any beyond the number the recipe produces are ignored. Cancel the
     * event to stop the craft instead.
     *
     * @param results the items the craft should produce
     */
    void setResults(std::vector<ItemStack> results) { results_ = std::move(results); }

    /**
     * Gets the number of times the recipe is being crafted.
     *
     * This is usually 1, but is higher when a batch is crafted at once, such as a shift click in the recipe book.
     *
     * @return the number of times the recipe is being crafted
     */
    [[nodiscard]] int getRepetitions() const { return repetitions_; }

    /**
     * Sets the number of times the recipe is being crafted.
     *
     * @note Values are clamped to the 0-255 range the server accepts.
     *
     * @param repetitions the number of times the recipe is being crafted
     */
    void setRepetitions(int repetitions) { repetitions_ = repetitions; }

private:
    Recipe recipe_;
    std::vector<ItemStack> ingredients_;
    std::vector<ItemStack> results_;
    int repetitions_;
};

}  // namespace endstone
