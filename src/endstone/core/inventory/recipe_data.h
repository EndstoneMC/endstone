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
#include <optional>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "bedrock/world/item/crafting/recipe.h"
#include "bedrock/world/item/crafting/recipes.h"
#include "endstone/core/type.h"
#include "endstone/inventory/complex_recipe.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/inventory/recipe.h"
#include "endstone/inventory/recipe_ingredient.h"
#include "endstone/inventory/shaped_recipe.h"
#include "endstone/inventory/shapeless_recipe.h"
#include "endstone/inventory/smithing_transform_recipe.h"
#include "endstone/inventory/smithing_trim_recipe.h"

namespace endstone::core {

class EndstoneRecipe : public Recipe::Impl {
public:
    explicit EndstoneRecipe(std::shared_ptr<const ::Recipe> recipe) : recipe_(std::move(recipe)) {}

    static endstone::Recipe fromMinecraft(std::shared_ptr<const ::Recipe> recipe);
    static std::optional<endstone::Recipe> fromMinecraft(const ::Recipes &recipes, const ::Recipe &recipe);
    static std::unique_ptr<::Recipe> toMinecraft(const Recipe &recipe);

    [[nodiscard]] endstone::ItemStack getResult() const override;
    [[nodiscard]] const std::vector<std::optional<endstone::RecipeIngredient>> &getIngredients() const override;
    [[nodiscard]] std::optional<endstone::RecipeIngredient> getSmithingIngredient(std::size_t index) const override;
    [[nodiscard]] const ::Recipe &getHandle() const { return *recipe_; }
    [[nodiscard]] const std::string &getRecipeId() const override { return recipe_->getRecipeId(); }
    [[nodiscard]] const std::string &getTag() const override { return recipe_->getTag().getString(); }

protected:
    std::shared_ptr<const ::Recipe> recipe_;
    mutable std::vector<std::optional<endstone::RecipeIngredient>> ingredients_;
    mutable bool ingredients_built_ = false;
};

template <typename Interface>
class EndstoneRecipeBase : public EndstoneRecipe {
public:
    using EndstoneRecipe::EndstoneRecipe;

    [[nodiscard]] ClassInfo getClassInfo() const override { return typeid(Interface); }

    [[nodiscard]] bool isInstanceOf(ClassInfo target) const override
    {
        return core::isTypeInstanceOf(getClassInfo(), target);
    }
};

class EndstoneComplexRecipe final : public EndstoneRecipeBase<endstone::ComplexRecipe> {
public:
    using EndstoneRecipeBase::EndstoneRecipeBase;
    [[nodiscard]] std::unique_ptr<Impl> clone() const override { return std::make_unique<EndstoneComplexRecipe>(*this); }
};

class EndstoneShapedRecipe final : public EndstoneRecipeBase<endstone::ShapedRecipe> {
public:
    using EndstoneRecipeBase::EndstoneRecipeBase;
    [[nodiscard]] std::unique_ptr<Impl> clone() const override { return std::make_unique<EndstoneShapedRecipe>(*this); }
    [[nodiscard]] int getWidth() const override { return getHandle().getWidth(); }
    [[nodiscard]] int getHeight() const override { return getHandle().getHeight(); }
};

class EndstoneShapelessRecipe final : public EndstoneRecipeBase<endstone::ShapelessRecipe> {
public:
    using EndstoneRecipeBase::EndstoneRecipeBase;
    [[nodiscard]] std::unique_ptr<Impl> clone() const override
    {
        return std::make_unique<EndstoneShapelessRecipe>(*this);
    }
};

class EndstoneSmithingTransformRecipe final : public EndstoneRecipeBase<endstone::SmithingTransformRecipe> {
public:
    using EndstoneRecipeBase::EndstoneRecipeBase;
    [[nodiscard]] std::unique_ptr<Impl> clone() const override
    {
        return std::make_unique<EndstoneSmithingTransformRecipe>(*this);
    }
};

class EndstoneSmithingTrimRecipe final : public EndstoneRecipeBase<endstone::SmithingTrimRecipe> {
public:
    using EndstoneRecipeBase::EndstoneRecipeBase;
    [[nodiscard]] std::unique_ptr<Impl> clone() const override
    {
        return std::make_unique<EndstoneSmithingTrimRecipe>(*this);
    }
};

}  // namespace endstone::core
