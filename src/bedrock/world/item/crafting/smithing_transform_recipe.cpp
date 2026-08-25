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

#include "bedrock/world/item/crafting/smithing_transform_recipe.h"

#include "bedrock/world/item/item_stack.h"

const mce::UUID SmithingTransformRecipe::ID = mce::UUID::fromString("9d304ae7-8b66-4423-80a0-eea8d3abd2a5");

SmithingTransformRecipe::SmithingTransformRecipe(const std::string &recipe_id,
                                                 const RecipeIngredient &template_ingredient,
                                                 const RecipeIngredient &base_ingredient,
                                                 const RecipeIngredient &addition_ingredient, Results &&results,
                                                 const HashedString &tag)
    : ShapelessRecipe(ConstructionContext{
          .recipe_id = recipe_id,
          .ingredients = {template_ingredient, base_ingredient, addition_ingredient},
          .results = std::move(results),
          .tag = tag,
          .priority = 0,
          .uuid = nullptr,
          .unlocking_requirement = RecipeUnlockingRequirement(),
          .format_version = SemVersion(1, 20, 10),
      })
{
    my_id_ = ID;
    width_ = 3;
    height_ = 1;
}

bool SmithingTransformRecipe::matches(const CraftingContainer &crafting_container,
                                      const CraftingContext & /*context*/) const
{
    if (my_ingredients_.size() < 3) {
        return false;
    }
    const auto template_desc = crafting_container.getItem(0).getDescriptor();
    const auto &template_ingredient = my_ingredients_[0];
    if (!template_ingredient.isNull() && !template_ingredient.sameItem(RecipeIngredient::EMPTY_INGREDIENT, true) &&
        !template_ingredient.sameItem(template_desc, true)) {
        return false;
    }
    const auto base_desc = crafting_container.getItem(1).getDescriptor();
    if (!my_ingredients_[1].sameItem(base_desc, true)) {
        return false;
    }
    const auto addition_desc = crafting_container.getItem(2).getDescriptor();
    return my_ingredients_[2].sameItem(addition_desc, true);
}

const Recipe::ResultList &SmithingTransformRecipe::assemble(CraftingContainer &crafting_container,
                                                            CraftingContext & /*context*/) const
{
    runtime_results_ = results_.getItems();
    if (!runtime_results_.empty()) {
        const auto &base = crafting_container.getItem(1);
        if (static_cast<bool>(base) && base.getUserData() != nullptr) {
            runtime_results_.front() = ItemInstance(base);
        }
    }
    return runtime_results_;
}

const RecipeIngredient &SmithingTransformRecipe::getTemplateIngredient() const
{
    return my_ingredients_.empty() ? RecipeIngredient::EMPTY_INGREDIENT : my_ingredients_[0];
}

const RecipeIngredient &SmithingTransformRecipe::getBaseIngredient() const
{
    return my_ingredients_.size() < 2 ? RecipeIngredient::EMPTY_INGREDIENT : my_ingredients_[1];
}

const RecipeIngredient &SmithingTransformRecipe::getAdditionIngredient() const
{
    return my_ingredients_.size() < 3 ? RecipeIngredient::EMPTY_INGREDIENT : my_ingredients_[2];
}

const ItemInstance &SmithingTransformRecipe::getResult() const
{
    const auto &items = results_.getItems();
    return items.empty() ? ItemInstance::EMPTY_ITEM : items.front();
}
