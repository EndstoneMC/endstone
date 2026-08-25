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

class ShapedRecipe : public Recipe {
public:
    ShapedRecipe(ConstructionContext &&context, int width, int height, bool assume_symmetry);
    ShapedRecipe(ShapedRecipe &&) = default;
    ShapedRecipe(const ShapedRecipe &) = default;
    ~ShapedRecipe() override = default;

    const ResultList &assemble(CraftingContainer &, CraftingContext &) const override;
    int getCraftingSize() const override;
    const RecipeIngredient &getIngredient(int x, int y) const override;
    bool isShapeless() const override;
    bool matches(const CraftingContainer &craft_slots, const CraftingContext &) const override;
    int size() const override;
    [[nodiscard]] size_t getIngredientsHashOffset(int simulated_width, int simulated_height, int offset_x,
                                                  int offset_y) const;
    [[nodiscard]] bool assumeSymmetry() const { return assume_symmetry_; }

private:
    [[nodiscard]] bool matches(const CraftingContainer &craft_slots, int x_offs, int y_offs, bool x_flip) const;

    bool assume_symmetry_;  // +256
};
BEDROCK_STATIC_ASSERT_SIZE(ShapedRecipe, 264, 264);
