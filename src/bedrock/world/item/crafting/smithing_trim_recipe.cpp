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

#include "bedrock/world/item/crafting/smithing_trim_recipe.h"
#include "bedrock/world/item/registry/armor_trim.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"

const mce::UUID SmithingTrimRecipe::ID = mce::UUID::fromString("8e1dc2a4-ee40-4e2d-a3ed-674c566d3d3d");

SmithingTrimRecipe::SmithingTrimRecipe(const std::string &recipe_id, const RecipeIngredient &template_ingredient,
                                       const RecipeIngredient &base_ingredient,
                                       const RecipeIngredient &addition_ingredient, const HashedString &tag)
    : ShapelessRecipe(ConstructionContext{
          .recipe_id = recipe_id,
          .ingredients = {template_ingredient, base_ingredient, addition_ingredient},
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

bool SmithingTrimRecipe::matches(const CraftingContainer &crafting_container,
                                 const CraftingContext & /*crafting_context*/) const
{
    if (my_ingredients_.size() < 3) {
        return false;
    }
    const auto template_desc = crafting_container.getItem(0).getDescriptor();
    if (!my_ingredients_[0].sameItem(template_desc, true)) {
        return false;
    }
    const auto base_desc = crafting_container.getItem(1).getDescriptor();
    if (!my_ingredients_[1].sameItem(base_desc, true)) {
        return false;
    }
    const auto addition_desc = crafting_container.getItem(2).getDescriptor();
    return my_ingredients_[2].sameItem(addition_desc, true);
}

const Recipe::ResultList &SmithingTrimRecipe::assemble(CraftingContainer &crafting_container,
                                                       CraftingContext &crafting_context) const
{
    runtime_results_.clear();
    runtime_results_.emplace_back(crafting_container.getItem(1));

    const auto *pattern_item = crafting_container.getItem(0).getItem();
    const auto *material_item = crafting_container.getItem(2).getItem();
    if (pattern_item == nullptr || material_item == nullptr || runtime_results_.empty()) {
        return runtime_results_;
    }

    const auto pattern_registry = crafting_context.getTrimPatternRegistry().lock();
    const auto material_registry = crafting_context.getTrimMaterialRegistry().lock();
    if (pattern_registry == nullptr || material_registry == nullptr) {
        return runtime_results_;
    }

    const auto pattern_id = pattern_registry->getPatternIdByItem(pattern_item->getFullNameHash());
    const auto material = material_registry->getTrimMaterialByItemName(material_item->getFullNameHash());
    if (!pattern_id.has_value() || !material.has_value()) {
        return runtime_results_;
    }

    ArmorTrim::setTrim(runtime_results_.front(), ArmorTrim(*pattern_id, material->material_id));
    return runtime_results_;
}

bool SmithingTrimRecipe::hasDataDrivenResult() const
{
    return false;
}

const RecipeIngredient &SmithingTrimRecipe::getTemplateIngredient() const
{
    return my_ingredients_.empty() ? RecipeIngredient::EMPTY_INGREDIENT : my_ingredients_[0];
}

const RecipeIngredient &SmithingTrimRecipe::getBaseIngredient() const
{
    return my_ingredients_.size() < 2 ? RecipeIngredient::EMPTY_INGREDIENT : my_ingredients_[1];
}

const RecipeIngredient &SmithingTrimRecipe::getAdditionIngredient() const
{
    return my_ingredients_.size() < 3 ? RecipeIngredient::EMPTY_INGREDIENT : my_ingredients_[2];
}
