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

#include "bedrock/world/item/crafting/shaped_recipe.h"
#include "bedrock/world/item/crafting/shapeless_recipe.h"
#include "bedrock/world/item/crafting/smithing_transform_recipe.h"
#include "bedrock/world/item/crafting/smithing_trim_recipe.h"
#include "bedrock/world/item/item_instance.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/inventory/recipe_ingredient.h"
#include "endstone/inventory/item_type.h"

namespace endstone::core {

endstone::Recipe EndstoneRecipe::fromMinecraft(std::shared_ptr<const ::Recipe> recipe)
{
    if (recipe->isMultiRecipe()) {
        return ComplexRecipe(std::make_unique<EndstoneComplexRecipe>(std::move(recipe)));
    }
    if (recipe->getTag().getString() == "smithing_table") {
        if (recipe->hasDataDrivenResult()) {
            return SmithingTransformRecipe(std::make_unique<EndstoneSmithingTransformRecipe>(std::move(recipe)));
        }
        return SmithingTrimRecipe(std::make_unique<EndstoneSmithingTrimRecipe>(std::move(recipe)));
    }
    if (recipe->isShapeless()) {
        return ShapelessRecipe(std::make_unique<EndstoneShapelessRecipe>(std::move(recipe)));
    }
    return ShapedRecipe(std::make_unique<EndstoneShapedRecipe>(std::move(recipe)));
}

std::optional<endstone::Recipe> EndstoneRecipe::fromMinecraft(const ::Recipes &recipes, const ::Recipe &recipe)
{
    const auto &all_tags = recipes.getRecipesAllTags();
    const auto by_tag = all_tags.find(recipe.getTag());
    if (by_tag == all_tags.end()) {
        return std::nullopt;
    }
    const auto by_id = by_tag->second.find(recipe.getRecipeId());
    if (by_id == by_tag->second.end()) {
        return std::nullopt;
    }
    return fromMinecraft(by_id->second);
}

namespace {

::Recipe::Ingredients toMinecraftIngredients(const std::vector<std::optional<endstone::RecipeIngredient>> &ingredients)
{
    ::Recipe::Ingredients slots;
    slots.reserve(ingredients.size());
    for (const auto &ingredient : ingredients) {
        if (ingredient) {
            slots.push_back(EndstoneIngredient::toMinecraft(*ingredient));
        }
        else {
            slots.emplace_back();
        }
    }
    return slots;
}

::Recipe::Results toMinecraftResults(const endstone::ItemStack &result)
{
    return ::Recipe::Results({ItemInstance(EndstoneItemStack::toMinecraft(result))});
}

RecipeUnlockingRequirement alwaysUnlocked()
{
    return RecipeUnlockingRequirement(RecipeUnlockingRequirement::UnlockingContext::AlwaysUnlocked);
}

}  // namespace

std::unique_ptr<::Recipe> EndstoneRecipe::toMinecraft(const Recipe &recipe)
{
    if (dynamic_cast<const EndstoneRecipe *>(recipe.impl_.get()) != nullptr) {
        const auto &handle = static_cast<const EndstoneRecipe &>(*recipe.impl_).getHandle();
        auto context = handle.getConstructionContext();
        if (handle.getTag().getString() == "smithing_table") {
            const auto &ingredients = handle.getIngredients();
            if (ingredients.size() < 3) {
                return nullptr;
            }
            if (handle.hasDataDrivenResult()) {
                return std::make_unique<::SmithingTransformRecipe>(handle.getRecipeId(), ingredients[0], ingredients[1],
                                                                   ingredients[2], std::move(context.results),
                                                                   handle.getTag());
            }
            return std::make_unique<::SmithingTrimRecipe>(handle.getRecipeId(), ingredients[0], ingredients[1],
                                                          ingredients[2], handle.getTag());
        }
        if (handle.isShapeless()) {
            return std::make_unique<::ShapelessRecipe>(std::move(context));
        }
        return std::make_unique<::ShapedRecipe>(std::move(context), handle.getWidth(), handle.getHeight(), true);
    }

    if (recipe.is<ComplexRecipe>()) {
        return nullptr;
    }

    if (const auto transform = recipe.as<SmithingTransformRecipe>()) {
        const auto template_ingredient = transform->getTemplate();
        const auto base = transform->getBase();
        const auto addition = transform->getAddition();
        if (!template_ingredient || !base || !addition) {
            return nullptr;
        }
        auto handle = std::make_unique<::SmithingTransformRecipe>(
            transform->getRecipeId(), EndstoneIngredient::toMinecraft(*template_ingredient),
            EndstoneIngredient::toMinecraft(*base), EndstoneIngredient::toMinecraft(*addition),
            toMinecraftResults(transform->getResult()), HashedString(transform->getTag()));
        handle->setUnlockingRequirement(alwaysUnlocked());
        return handle;
    }

    if (const auto trim = recipe.as<SmithingTrimRecipe>()) {
        const auto template_ingredient = trim->getTemplate();
        const auto base = trim->getBase();
        const auto addition = trim->getAddition();
        if (!template_ingredient || !base || !addition) {
            return nullptr;
        }
        auto handle = std::make_unique<::SmithingTrimRecipe>(
            trim->getRecipeId(), EndstoneIngredient::toMinecraft(*template_ingredient),
            EndstoneIngredient::toMinecraft(*base), EndstoneIngredient::toMinecraft(*addition),
            HashedString(trim->getTag()));
        handle->setUnlockingRequirement(alwaysUnlocked());
        return handle;
    }

    if (const auto shapeless = recipe.as<ShapelessRecipe>()) {
        ::Recipe::ConstructionContext context;
        context.recipe_id = shapeless->getRecipeId();
        context.ingredients = toMinecraftIngredients(shapeless->getIngredients());
        context.results = toMinecraftResults(shapeless->getResult());
        context.tag = HashedString(shapeless->getTag());
        context.unlocking_requirement = alwaysUnlocked();
        return std::make_unique<::ShapelessRecipe>(std::move(context));
    }

    if (const auto shaped = recipe.as<ShapedRecipe>()) {
        ::Recipe::ConstructionContext context;
        context.recipe_id = shaped->getRecipeId();
        context.ingredients = toMinecraftIngredients(shaped->getIngredients());
        context.results = toMinecraftResults(shaped->getResult());
        context.tag = HashedString(shaped->getTag());
        context.unlocking_requirement = alwaysUnlocked();
        return std::make_unique<::ShapedRecipe>(std::move(context), shaped->getWidth(), shaped->getHeight(), true);
    }

    return nullptr;
}

endstone::ItemStack EndstoneRecipe::getResult() const
{
    const auto &results = recipe_->getResultItems();
    return results.empty() ? endstone::ItemType::get(endstone::ItemType::Air)->createItemStack(0)
                           : EndstoneItemStack::fromMinecraft(results.front());
}

const std::vector<std::optional<endstone::RecipeIngredient>> &EndstoneRecipe::getIngredients() const
{
    if (!ingredients_built_) {
        const auto &ingredients = recipe_->getIngredients();
        ingredients_.reserve(ingredients.size());
        for (const auto &ingredient : ingredients) {
            ingredients_.push_back(EndstoneIngredient::fromMinecraft(recipe_, ingredient));
        }
        ingredients_built_ = true;
    }
    return ingredients_;
}

std::optional<endstone::RecipeIngredient> EndstoneRecipe::getSmithingIngredient(const std::size_t index) const
{
    const auto &ingredients = getIngredients();
    return index < ingredients.size() ? ingredients[index] : std::nullopt;
}

}  // namespace endstone::core
