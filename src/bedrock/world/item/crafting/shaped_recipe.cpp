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

#include "bedrock/world/item/crafting/shaped_recipe.h"

#include "bedrock/world/item/item_stack.h"

ShapedRecipe::ShapedRecipe(ConstructionContext &&context, int width, int height, bool assume_symmetry)
    : Recipe(std::move(context)), assume_symmetry_(assume_symmetry)
{
    width_ = width;
    height_ = height;
}

const Recipe::ResultList &ShapedRecipe::assemble(CraftingContainer & /*container*/, CraftingContext & /*context*/) const
{
    return results_.getItems();
}

int ShapedRecipe::getCraftingSize() const
{
    return (width_ >= 3) | (height_ >= 3);
}

const RecipeIngredient &ShapedRecipe::getIngredient(int x, int y) const
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

bool ShapedRecipe::isShapeless() const
{
    return false;
}

bool ShapedRecipe::matches(const CraftingContainer &craft_slots, const CraftingContext & /*context*/) const
{
    if (width_ > 3 || height_ > 3) {
        return false;
    }
    for (int x_off = 0; x_off <= 3 - width_; ++x_off) {
        for (int y_off = 0; y_off <= 3 - height_; ++y_off) {
            if (matches(craft_slots, x_off, y_off, false) ||
                (assume_symmetry_ && matches(craft_slots, x_off, y_off, true))) {
                return true;
            }
        }
    }
    return false;
}

int ShapedRecipe::size() const
{
    return width_ * height_;
}

size_t ShapedRecipe::getIngredientsHashOffset(int simulated_width, int simulated_height, int offset_x,
                                              int offset_y) const
{
    size_t hash = 0;
    for (int y = 0; y < simulated_height; ++y) {
        for (int x = 0; x < simulated_width; ++x) {
            hash += getIngredient(x - offset_x, y - offset_y).getHash();
        }
    }
    return hash;
}

bool ShapedRecipe::matches(const CraftingContainer &craft_slots, int x_offs, int y_offs, bool x_flip) const
{
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            const auto &ingredient = getIngredient(x_flip ? width_ - 1 - (x - x_offs) : x - x_offs, y - y_offs);
            const ItemStack *slot = &ItemStack::EMPTY_ITEM;
            if (x < craft_slots.getWidth()) {
                slot = &craft_slots.getItem(x + y * craft_slots.getWidth());
            }
            const auto descriptor = slot->getDescriptor();
            const bool occupied = static_cast<bool>(*slot);
            const bool has_ingredient = !ingredient.isNull();
            if (occupied != has_ingredient) {
                return false;
            }
            if (occupied && !ingredient.sameItem(descriptor, true)) {
                return false;
            }
        }
    }
    return true;
}
