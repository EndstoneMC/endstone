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

#include "endstone/inventory/smithing_recipe.h"

namespace endstone {
/**
 * Represents a smithing transform recipe.
 */
class SmithingTransformRecipe : public SmithingRecipe {
public:
    SmithingTransformRecipe(std::string recipe_id, RecipeIngredient smithing_template, RecipeIngredient base,
                            RecipeIngredient addition, ItemStack result)
        : SmithingRecipe(std::make_unique<SimpleImpl>(
              std::initializer_list<ClassInfo>{typeid(SmithingTransformRecipe), typeid(SmithingRecipe), typeid(Recipe)},
              std::move(recipe_id), "smithing_table", std::move(result),
              std::vector<std::optional<RecipeIngredient>>{std::move(smithing_template), std::move(base),
                                                           std::move(addition)}))
    {
    }
    SmithingTransformRecipe(const SmithingTransformRecipe &other) : SmithingRecipe(other) {}
    SmithingTransformRecipe(SmithingTransformRecipe &&other) noexcept = default;
    SmithingTransformRecipe &operator=(const SmithingTransformRecipe &other) = default;
    SmithingTransformRecipe &operator=(SmithingTransformRecipe &&other) noexcept = default;
    ~SmithingTransformRecipe() override = default;

    /**
     * Get the template recipe item.
     *
     * @return template choice
     */
    [[nodiscard]] std::optional<RecipeIngredient> getTemplate() const { return impl_->getSmithingIngredient(0); }

private:
    friend class Recipe;
    friend class SmithingRecipe;
    friend class core::EndstoneRecipe;
    explicit SmithingTransformRecipe(std::unique_ptr<Impl> impl) : SmithingRecipe(std::move(impl)) {}
};
}  // namespace endstone
