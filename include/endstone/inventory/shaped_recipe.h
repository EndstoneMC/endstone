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

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

#include "endstone/inventory/recipe.h"

namespace endstone {
/**
 * Represents a shaped (ie normal) crafting recipe.
 */
class ShapedRecipe : public Recipe {
public:
    ShapedRecipe(std::string recipe_id, std::vector<std::string> shape,
                 std::vector<std::pair<char, RecipeIngredient>> ingredients, ItemStack result,
                 std::string tag = "crafting_table")
        : Recipe([&] {
              int width = 0;
              const int height = static_cast<int>(shape.size());
              for (const auto &row : shape) {
                  width = std::max(width, static_cast<int>(row.size()));
              }
              std::vector<std::optional<RecipeIngredient>> slots;
              slots.reserve(static_cast<std::size_t>(width) * static_cast<std::size_t>(height));
              for (const auto &row : shape) {
                  for (int x = 0; x < width; ++x) {
                      const char key = x < static_cast<int>(row.size()) ? row[static_cast<std::size_t>(x)] : ' ';
                      auto it = ingredients.end();
                      if (key != ' ') {
                          it = std::ranges::find_if(ingredients, [key](const auto &pair) { return pair.first == key; });
                      }
                      if (it == ingredients.end()) {
                          slots.emplace_back();
                      }
                      else {
                          slots.emplace_back(it->second);
                      }
                  }
              }
              return std::make_unique<SimpleImpl>(
                  std::initializer_list<ClassInfo>{typeid(ShapedRecipe), typeid(Recipe)}, std::move(recipe_id),
                  std::move(tag), std::move(result), std::move(slots), width, height);
          }())
    {
    }
    ShapedRecipe(const ShapedRecipe &other) : Recipe(other) {}
    ShapedRecipe(ShapedRecipe &&other) noexcept = default;
    ShapedRecipe &operator=(const ShapedRecipe &other) = default;
    ShapedRecipe &operator=(ShapedRecipe &&other) noexcept = default;
    ~ShapedRecipe() override = default;

    [[nodiscard]] int getWidth() const { return impl_->getWidth(); }
    [[nodiscard]] int getHeight() const { return impl_->getHeight(); }

private:
    friend class Recipe;
    friend class core::EndstoneRecipe;
    explicit ShapedRecipe(std::unique_ptr<Impl> impl) : Recipe(std::move(impl)) {}
};
}  // namespace endstone
