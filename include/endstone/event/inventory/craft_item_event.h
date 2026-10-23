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

#include <cstddef>
#include <utility>
#include <vector>

#include "endstone/check.h"
#include "endstone/event/inventory/inventory_interact_event.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/inventory/recipe.h"

namespace endstone {

/**
 * Called when a player crafts an item, either inside a crafting grid or straight from the recipe book.
 *
 * getInventory() is the player's UI container, which holds the crafting grid: slots 28 to 31 for the 2x2 grid of the
 * inventory screen and slots 32 to 40 for the 3x3 grid of a crafting table.
 *
 * @note If the event is cancelled the item will not be crafted and the ingredients will not be consumed.
 */
class CraftItemEvent final : public InventoryInteractEvent {
public:
    ENDSTONE_EVENT(CraftItemEvent);

    CraftItemEvent(const NotNull<Inventory> &inventory, const NotNull<Player> &who_clicked, NotNull<Recipe> recipe,
                   std::vector<ItemStack> ingredients, std::vector<ItemStack> results, int repetitions)
        : InventoryInteractEvent(inventory, who_clicked), recipe_(std::move(recipe)),
          ingredients_(std::move(ingredients)), results_(std::move(results)), repetitions_(repetitions)
    {
        for (const auto &result : results_) {
            amounts_.push_back(result.getAmount());
        }
    }

    /**
     * @return A copy of the current recipe on the crafting matrix.
     */
    [[nodiscard]] NotNull<Recipe> getRecipe() const { return recipe_; }

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
     * A result may be a different item, but its amount must match the amount the recipe produces in its place.
     *
     * @note Results are replaced one for one, so any beyond the number the recipe produces are ignored. Cancel the
     * event to stop the craft instead.
     *
     * @param results the items the craft should produce
     * @throws std::invalid_argument if a result's amount differs from the recipe's
     */
    void setResults(std::vector<ItemStack> results)
    {
        for (std::size_t i = 0; i < results.size() && i < amounts_.size(); ++i) {
            Preconditions::checkArgument(results[i].getAmount() == amounts_[i],
                                         "Result {} must have an amount of {}, got {}.", i, amounts_[i],
                                         results[i].getAmount());
        }
        results_ = std::move(results);
    }

    /**
     * Gets the number of times the recipe is being crafted.
     *
     * This is usually 1, but is higher when a batch is crafted at once, such as a shift click in the recipe book.
     *
     * @return the number of times the recipe is being crafted
     */
    [[nodiscard]] int getRepetitions() const { return repetitions_; }

private:
    NotNull<Recipe> recipe_;
    std::vector<ItemStack> ingredients_;
    std::vector<ItemStack> results_;
    int repetitions_;
    std::vector<int> amounts_;
};

}  // namespace endstone
