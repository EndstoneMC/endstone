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

#include "bedrock/bedrock.h"
#include "bedrock/world/item/crafting/shapeless_recipe.h"

class SmithingTrimRecipe : public ShapelessRecipe {
public:
    SmithingTrimRecipe(const std::string &recipe_id, const RecipeIngredient &template_ingredient,
                       const RecipeIngredient &base_ingredient, const RecipeIngredient &addition_ingredient,
                       const HashedString &tag);

    bool matches(const CraftingContainer &crafting_container, const CraftingContext &crafting_context) const override;
    const ResultList &assemble(CraftingContainer &crafting_container, CraftingContext &crafting_context) const override;
    bool hasDataDrivenResult() const override;
    [[nodiscard]] const RecipeIngredient &getTemplateIngredient() const;
    [[nodiscard]] const RecipeIngredient &getBaseIngredient() const;
    [[nodiscard]] const RecipeIngredient &getAdditionIngredient() const;

    static const mce::UUID ID;

private:
    mutable ResultList runtime_results_;  // +256
};
BEDROCK_STATIC_ASSERT_SIZE(SmithingTrimRecipe, 280, 280);

