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

#include <memory>
#include <string>
#include <typeinfo>
#include <utility>

#include "bedrock/world/item/crafting/recipe.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/type.h"
#include "endstone/inventory/complex_alias_ingredient.h"
#include "endstone/inventory/exact_ingredient.h"
#include "endstone/inventory/item_tag_ingredient.h"
#include "endstone/inventory/item_type_ingredient.h"
#include "endstone/inventory/molang_ingredient.h"
#include "endstone/util/pointers.h"

namespace endstone::core {

template <typename Interface>
class EndstoneIngredientBase : public Interface {
public:
    EndstoneIngredientBase(std::shared_ptr<const ::Recipe> recipe, const ::RecipeIngredient &ingredient)
        : recipe_(std::move(recipe)), ingredient_(&ingredient)
    {
    }

    [[nodiscard]] ClassInfo getClassInfo() const override { return typeid(Interface); }

    [[nodiscard]] bool isInstanceOf(ClassInfo target) const override
    {
        return core::isInstanceOf(*this, target);
    }

    [[nodiscard]] int getCount() const override { return static_cast<int>(ingredient_->getStackSize()); }

    [[nodiscard]] bool test(const endstone::ItemStack &item) const override
    {
        return recipe_->itemValidForRecipe(*ingredient_, EndstoneItemStack::toMinecraft(item));
    }

protected:
    std::shared_ptr<const ::Recipe> recipe_;
    const ::RecipeIngredient *ingredient_;
};

class EndstoneExactIngredient final : public EndstoneIngredientBase<endstone::ExactIngredient> {
public:
    EndstoneExactIngredient(std::shared_ptr<const ::Recipe> recipe, const ::RecipeIngredient &ingredient,
                            endstone::ItemStack item)
        : EndstoneIngredientBase(std::move(recipe), ingredient), item_(std::move(item))
    {
    }

    [[nodiscard]] endstone::ItemStack getItemStack() const override { return item_; }

private:
    endstone::ItemStack item_;
};

class EndstoneItemTypeIngredient final : public EndstoneIngredientBase<endstone::ItemTypeIngredient> {
public:
    EndstoneItemTypeIngredient(std::shared_ptr<const ::Recipe> recipe, const ::RecipeIngredient &ingredient,
                               const endstone::ItemType &type)
        : EndstoneIngredientBase(std::move(recipe), ingredient), type_(&type)
    {
    }

    [[nodiscard]] const endstone::ItemType &getItemType() const override { return *type_; }

private:
    const endstone::ItemType *type_;
};

class EndstoneItemTagIngredient final : public EndstoneIngredientBase<endstone::ItemTagIngredient> {
public:
    EndstoneItemTagIngredient(std::shared_ptr<const ::Recipe> recipe, const ::RecipeIngredient &ingredient,
                              std::string item_tag)
        : EndstoneIngredientBase(std::move(recipe), ingredient), item_tag_(std::move(item_tag))
    {
    }

    [[nodiscard]] const std::string &getItemTag() const override { return item_tag_; }

private:
    std::string item_tag_;
};

class EndstoneMolangIngredient final : public EndstoneIngredientBase<endstone::MolangIngredient> {
public:
    EndstoneMolangIngredient(std::shared_ptr<const ::Recipe> recipe, const ::RecipeIngredient &ingredient,
                             std::string expression)
        : EndstoneIngredientBase(std::move(recipe), ingredient), expression_(std::move(expression))
    {
    }

    [[nodiscard]] const std::string &getExpression() const override { return expression_; }

private:
    std::string expression_;
};

class EndstoneComplexAliasIngredient final : public EndstoneIngredientBase<endstone::ComplexAliasIngredient> {
public:
    EndstoneComplexAliasIngredient(std::shared_ptr<const ::Recipe> recipe, const ::RecipeIngredient &ingredient,
                                   std::string alias)
        : EndstoneIngredientBase(std::move(recipe), ingredient), alias_(std::move(alias))
    {
    }

    [[nodiscard]] const std::string &getAlias() const override { return alias_; }

private:
    std::string alias_;
};

}  // namespace endstone::core
