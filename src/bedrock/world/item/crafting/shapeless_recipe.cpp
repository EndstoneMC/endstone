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

#include "bedrock/world/item/crafting/shapeless_recipe.h"

#include "bedrock/world/item/item_stack.h"

ShapelessRecipe::ShapelessRecipe(ConstructionContext &&context) : Recipe(std::move(context))
{
    const auto count = my_ingredients_.size();
    if (count > 4) {
        width_ = 3;
        height_ = static_cast<int>((count - 1) / 3) + 1;
    }
    else {
        width_ = count < 2 ? static_cast<int>(count) : 2;
        height_ = count == 0 ? 0 : static_cast<int>((count - 1) / 2) + 1;
    }
}

const Recipe::ResultList &ShapelessRecipe::assemble(CraftingContainer & /*container*/,
                                                    CraftingContext & /*context*/) const
{
    return results_.getItems();
}

int ShapelessRecipe::getCraftingSize() const
{
    return my_ingredients_.size() >= 5 ? 1 : 0;
}

const RecipeIngredient &ShapelessRecipe::getIngredient(int x, int y) const
{
    if ((x | y) < 0 || x >= width_ || y >= height_) {
        return RecipeIngredient::EMPTY_INGREDIENT;
    }
    const auto index = static_cast<std::size_t>(x + y * width_);
    if (index >= my_ingredients_.size()) {
        return RecipeIngredient::EMPTY_INGREDIENT;
    }
    return my_ingredients_[index];
}

bool ShapelessRecipe::isShapeless() const
{
    return true;
}

bool ShapelessRecipe::matches(const CraftingContainer &craft_slots, const CraftingContext & /*context*/) const
{
    auto remaining = my_ingredients_;
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            const ItemStack *slot = &ItemStack::EMPTY_ITEM;
            if (x < craft_slots.getWidth()) {
                slot = &craft_slots.getItem(x + y * craft_slots.getWidth());
            }
            if (!static_cast<bool>(*slot)) {
                continue;
            }
            const auto descriptor = slot->getDescriptor();
            auto found = remaining.end();
            for (auto it = remaining.begin(); it != remaining.end(); ++it) {
                if (it->sameItem(descriptor, true)) {
                    found = it;
                    break;
                }
            }
            if (found == remaining.end()) {
                return false;
            }
            remaining.erase(found);
        }
    }
    return remaining.empty();
}

int ShapelessRecipe::size() const
{
    return static_cast<int>(my_ingredients_.size());
}
