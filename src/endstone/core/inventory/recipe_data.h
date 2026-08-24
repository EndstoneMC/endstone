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

#include <cstddef>
#include <memory>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "bedrock/world/item/crafting/recipe.h"
#include "bedrock/world/item/crafting/recipes.h"
#include "endstone/core/type.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/inventory/recipe.h"
#include "endstone/inventory/recipe_ingredient.h"

namespace endstone::core {

class EndstoneRecipeData {
public:
    explicit EndstoneRecipeData(std::shared_ptr<const ::Recipe> recipe) : recipe_(std::move(recipe)) {}

    static NotNull<endstone::Recipe> fromMinecraft(std::shared_ptr<const ::Recipe> recipe);
    static Nullable<endstone::Recipe> fromMinecraft(const ::Recipes &recipes, const ::Recipe &recipe);

    [[nodiscard]] endstone::ItemStack getResult() const;
    [[nodiscard]] const std::vector<Nullable<endstone::RecipeIngredient>> &getIngredients() const;
    [[nodiscard]] Nullable<endstone::RecipeIngredient> getIngredient(std::size_t index) const;
    [[nodiscard]] const ::Recipe &getHandle() const { return *recipe_; }
    [[nodiscard]] const std::string &getId() const { return recipe_->getRecipeId(); }
    [[nodiscard]] const std::string &getTag() const { return recipe_->getTag().getString(); }

protected:
    const std::shared_ptr<const ::Recipe> recipe_;
    mutable std::vector<Nullable<endstone::RecipeIngredient>> ingredients_;
    mutable bool ingredients_built_ = false;
};

template <typename Interface>
class EndstoneRecipeBase : public Interface, protected EndstoneRecipeData {
public:
    explicit EndstoneRecipeBase(std::shared_ptr<const ::Recipe> recipe) : EndstoneRecipeData(std::move(recipe)) {}

    [[nodiscard]] ClassInfo getClassInfo() const override { return typeid(Interface); }

    [[nodiscard]] bool isInstanceOf(ClassInfo target) const override
    {
        return core::isInstanceOf(*this, target);
    }

    [[nodiscard]] endstone::ItemStack getResult() const override { return EndstoneRecipeData::getResult(); }

    [[nodiscard]] const std::vector<Nullable<endstone::RecipeIngredient>> &getIngredients() const override
    {
        return EndstoneRecipeData::getIngredients();
    }

    [[nodiscard]] const std::string &getId() const override { return EndstoneRecipeData::getId(); }

    [[nodiscard]] const std::string &getTag() const override { return EndstoneRecipeData::getTag(); }
};

}  // namespace endstone::core
