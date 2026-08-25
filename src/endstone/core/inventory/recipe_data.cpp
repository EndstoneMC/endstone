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

#include "endstone/core/inventory/recipe_data.h"

#include "bedrock/world/item/item_descriptor.h"
#include "endstone/core/inventory/complex_recipe.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/inventory/recipe_ingredient.h"
#include "endstone/core/inventory/shaped_recipe.h"
#include "endstone/core/inventory/shapeless_recipe.h"
#include "endstone/core/inventory/smithing_recipe.h"
#include "endstone/inventory/item_type.h"

namespace endstone::core {
namespace {

Nullable<endstone::RecipeIngredient> makeIngredient(std::shared_ptr<const ::Recipe> recipe,
                                                    const ::RecipeIngredient &ingredient)
{
    const auto &descriptor = static_cast<const ::ItemDescriptor &>(ingredient);
    switch (descriptor.getType()) {
    case ItemDescriptor::InternalType::Invalid:
        return nullptr;
    case ItemDescriptor::InternalType::ItemTag:
        return std::make_shared<EndstoneItemTagIngredient>(std::move(recipe), ingredient, descriptor.getFullName());
    case ItemDescriptor::InternalType::Molang:
        return std::make_shared<EndstoneMolangIngredient>(std::move(recipe), ingredient, descriptor.getFullName());
    case ItemDescriptor::InternalType::ComplexAlias:
        return std::make_shared<EndstoneComplexAliasIngredient>(std::move(recipe), ingredient,
                                                                descriptor.getFullName());
    case ItemDescriptor::InternalType::Default: {
        const auto *type = endstone::ItemType::get(descriptor.getFullName());
        if (descriptor.getItem() == nullptr || type == nullptr) {
            return nullptr;
        }
        const auto aux_value = descriptor.getAuxValue();
        if (aux_value == ItemDescriptor::ANY_AUX_VALUE) {
            return std::make_shared<EndstoneItemTypeIngredient>(std::move(recipe), ingredient, *type);
        }
        auto item = type->createItemStack(static_cast<int>(ingredient.getStackSize()));
        item.setData(aux_value);
        return std::make_shared<EndstoneExactIngredient>(std::move(recipe), ingredient, std::move(item));
    }
    default:
        return nullptr;
    }
}

}  // namespace

NotNull<endstone::Recipe> EndstoneRecipeData::fromMinecraft(std::shared_ptr<const ::Recipe> recipe)
{
    if (recipe->isMultiRecipe()) {
        return std::make_shared<EndstoneComplexRecipe>(std::move(recipe));
    }
    if (recipe->getTag().getString() == "smithing_table") {
        if (recipe->hasDataDrivenResult()) {
            return std::make_shared<EndstoneSmithingTransformRecipe>(std::move(recipe));
        }
        return std::make_shared<EndstoneSmithingTrimRecipe>(std::move(recipe));
    }
    if (recipe->isShapeless()) {
        return std::make_shared<EndstoneShapelessRecipe>(std::move(recipe));
    }
    return std::make_shared<EndstoneShapedRecipe>(std::move(recipe));
}

Nullable<endstone::Recipe> EndstoneRecipeData::fromMinecraft(const ::Recipes &recipes, const ::Recipe &recipe)
{
    const auto &all_tags = recipes.getRecipesAllTags();
    const auto by_tag = all_tags.find(recipe.getTag());
    if (by_tag == all_tags.end()) {
        return nullptr;
    }
    const auto by_id = by_tag->second.find(recipe.getRecipeId());
    if (by_id == by_tag->second.end()) {
        return nullptr;
    }
    return fromMinecraft(by_id->second);
}

endstone::ItemStack EndstoneRecipeData::getResult() const
{
    const auto &results = recipe_->getResultItems();
    return results.empty() ? endstone::ItemType::get(endstone::ItemType::Air)->createItemStack(0)
                           : EndstoneItemStack::fromMinecraft(results.front());
}

const std::vector<Nullable<endstone::RecipeIngredient>> &EndstoneRecipeData::getIngredients() const
{
    if (!ingredients_built_) {
        const auto &ingredients = recipe_->getIngredients();
        ingredients_.reserve(ingredients.size());
        for (const auto &ingredient : ingredients) {
            ingredients_.push_back(makeIngredient(recipe_, ingredient));
        }
        ingredients_built_ = true;
    }
    return ingredients_;
}

Nullable<endstone::RecipeIngredient> EndstoneRecipeData::getIngredient(const std::size_t index) const
{
    const auto &ingredients = getIngredients();
    return index < ingredients.size() ? ingredients[index] : nullptr;
}

}  // namespace endstone::core
