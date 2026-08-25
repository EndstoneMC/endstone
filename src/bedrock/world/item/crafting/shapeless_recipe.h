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
#include "bedrock/world/item/crafting/recipe.h"

class ShapelessRecipe : public Recipe {
public:
    explicit ShapelessRecipe(ConstructionContext &&context);
    ShapelessRecipe(ShapelessRecipe &&) = default;
    ShapelessRecipe(const ShapelessRecipe &) = default;
    ~ShapelessRecipe() override = default;

    const ResultList &assemble(CraftingContainer &, CraftingContext &) const override;
    int getCraftingSize() const override;
    const RecipeIngredient &getIngredient(int x, int y) const override;
    bool isShapeless() const override;
    bool matches(const CraftingContainer &craft_slots, const CraftingContext &) const override;
    int size() const override;
};
BEDROCK_STATIC_ASSERT_SIZE(ShapelessRecipe, 256, 256);
