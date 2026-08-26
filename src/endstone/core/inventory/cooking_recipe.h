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
#include "endstone/inventory/blasting_recipe.h"
#include "endstone/inventory/campfire_recipe.h"
#include "endstone/inventory/furnace_recipe.h"
#include "endstone/inventory/smoking_recipe.h"

namespace endstone::core {

template <typename Interface>
class EndstoneCookingRecipeBase : public EndstoneRecipeBase<Interface> {
public:
    using EndstoneRecipeBase<Interface>::EndstoneRecipeBase;

    [[nodiscard]] Nullable<endstone::RecipeIngredient> getInputChoice() const override
    {
        return EndstoneRecipeData::getIngredient(0);
    }
};

class EndstoneBlastingRecipe final : public EndstoneCookingRecipeBase<endstone::BlastingRecipe> {
public:
    using EndstoneCookingRecipeBase::EndstoneCookingRecipeBase;
};

class EndstoneCampfireRecipe final : public EndstoneCookingRecipeBase<endstone::CampfireRecipe> {
public:
    using EndstoneCookingRecipeBase::EndstoneCookingRecipeBase;
};

class EndstoneFurnaceRecipe final : public EndstoneCookingRecipeBase<endstone::FurnaceRecipe> {
public:
    using EndstoneCookingRecipeBase::EndstoneCookingRecipeBase;
};

class EndstoneSmokingRecipe final : public EndstoneCookingRecipeBase<endstone::SmokingRecipe> {
public:
    using EndstoneCookingRecipeBase::EndstoneCookingRecipeBase;
};

}  // namespace endstone::core
