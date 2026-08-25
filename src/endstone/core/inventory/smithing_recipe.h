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

#include "endstone/core/inventory/recipe_data.h"
#include "endstone/inventory/smithing_transform_recipe.h"
#include "endstone/inventory/smithing_trim_recipe.h"

namespace endstone::core {

template <typename Interface>
class EndstoneSmithingRecipeBase : public EndstoneRecipeBase<Interface> {
public:
    using EndstoneRecipeBase<Interface>::EndstoneRecipeBase;

    [[nodiscard]] Nullable<endstone::RecipeIngredient> getTemplate() const override
    {
        return EndstoneRecipeData::getIngredient(0);
    }

    [[nodiscard]] Nullable<endstone::RecipeIngredient> getBase() const override
    {
        return EndstoneRecipeData::getIngredient(1);
    }

    [[nodiscard]] Nullable<endstone::RecipeIngredient> getAddition() const override
    {
        return EndstoneRecipeData::getIngredient(2);
    }
};

class EndstoneSmithingTransformRecipe final : public EndstoneSmithingRecipeBase<endstone::SmithingTransformRecipe> {
public:
    using EndstoneSmithingRecipeBase::EndstoneSmithingRecipeBase;
};

class EndstoneSmithingTrimRecipe final : public EndstoneSmithingRecipeBase<endstone::SmithingTrimRecipe> {
public:
    using EndstoneSmithingRecipeBase::EndstoneSmithingRecipeBase;
};

}  // namespace endstone::core
