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
#include <vector>

#include "endstone/inventory/recipe.h"

namespace endstone {
/**
 * Represents a shapeless recipe, where the arrangement of the ingredients on the crafting grid does not matter.
 */
class ShapelessRecipe : public Recipe {
public:
    ShapelessRecipe(std::string recipe_id, std::vector<RecipeIngredient> ingredients, ItemStack result,
                    std::string tag = "crafting_table")
        : Recipe([&] {
              std::vector<std::optional<RecipeIngredient>> slots;
              slots.reserve(ingredients.size());
              for (auto &ingredient : ingredients) {
                  slots.emplace_back(std::move(ingredient));
              }
              return std::make_unique<SimpleImpl>(
                  std::initializer_list<ClassInfo>{typeid(ShapelessRecipe), typeid(Recipe)}, std::move(recipe_id),
                  std::move(tag), std::move(result), std::move(slots));
          }())
    {
    }
    ShapelessRecipe(const ShapelessRecipe &other) : Recipe(other) {}
    ShapelessRecipe(ShapelessRecipe &&other) noexcept = default;
    ShapelessRecipe &operator=(const ShapelessRecipe &other) = default;
    ShapelessRecipe &operator=(ShapelessRecipe &&other) noexcept = default;
    ~ShapelessRecipe() override = default;

private:
    friend class Recipe;
    friend class core::EndstoneRecipe;
    explicit ShapelessRecipe(std::unique_ptr<Impl> impl) : Recipe(std::move(impl)) {}
};
}  // namespace endstone
