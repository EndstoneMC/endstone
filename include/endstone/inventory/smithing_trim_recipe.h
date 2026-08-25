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

#include "endstone/inventory/item_type.h"
#include "endstone/inventory/smithing_recipe.h"

namespace endstone {
/**
 * Represents a smithing trim recipe.
 */
class SmithingTrimRecipe : public SmithingRecipe {
public:
    SmithingTrimRecipe(std::string recipe_id, RecipeIngredient smithing_template, RecipeIngredient base,
                       RecipeIngredient addition)
        : SmithingRecipe(std::make_unique<SimpleImpl>(
              std::initializer_list<ClassInfo>{typeid(SmithingTrimRecipe), typeid(SmithingRecipe), typeid(Recipe)},
              std::move(recipe_id), "smithing_table", ItemType::get(ItemType::Air)->createItemStack(0),
              std::vector<std::optional<RecipeIngredient>>{std::move(smithing_template), std::move(base),
                                                           std::move(addition)}))
    {
    }
    SmithingTrimRecipe(const SmithingTrimRecipe &other) : SmithingRecipe(other) {}
    SmithingTrimRecipe(SmithingTrimRecipe &&other) noexcept = default;
    SmithingTrimRecipe &operator=(const SmithingTrimRecipe &other) = default;
    SmithingTrimRecipe &operator=(SmithingTrimRecipe &&other) noexcept = default;
    ~SmithingTrimRecipe() override = default;

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
    explicit SmithingTrimRecipe(std::unique_ptr<Impl> impl) : SmithingRecipe(std::move(impl)) {}
};
}  // namespace endstone
