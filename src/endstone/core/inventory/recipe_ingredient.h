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
#include <optional>
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

namespace endstone::core {

class EndstoneIngredient : public RecipeIngredient::Impl {
public:
    EndstoneIngredient(std::shared_ptr<const ::Recipe> recipe, ::RecipeIngredient ingredient)
        : recipe_(std::move(recipe)), ingredient_(std::move(ingredient))
    {
    }

    [[nodiscard]] bool test(const endstone::ItemStack &item) const override;

    [[nodiscard]] int getCount() const override { return static_cast<int>(ingredient_.getStackSize()); }

    [[nodiscard]] const ::RecipeIngredient &getHandle() const { return ingredient_; }

    static std::optional<endstone::RecipeIngredient> fromMinecraft(std::shared_ptr<const ::Recipe> recipe,
                                                                   const ::RecipeIngredient &ingredient);
    static const ::RecipeIngredient &getHandle(const RecipeIngredient &ingredient);
    static ::RecipeIngredient toMinecraft(const RecipeIngredient &ingredient);

protected:
    std::shared_ptr<const ::Recipe> recipe_;
    ::RecipeIngredient ingredient_;
};

template <typename Interface>
class EndstoneIngredientBase : public EndstoneIngredient {
public:
    using EndstoneIngredient::EndstoneIngredient;

    [[nodiscard]] ClassInfo getClassInfo() const override { return typeid(Interface); }

    [[nodiscard]] bool isInstanceOf(ClassInfo target) const override
    {
        return core::isTypeInstanceOf(getClassInfo(), target);
    }
};

class EndstoneExactIngredient final : public EndstoneIngredientBase<endstone::ExactIngredient> {
public:
    EndstoneExactIngredient(std::shared_ptr<const ::Recipe> recipe, const ::RecipeIngredient &ingredient,
                            endstone::ItemStack item)
        : EndstoneIngredientBase(std::move(recipe), ingredient), item_(std::move(item))
    {
    }

    [[nodiscard]] std::unique_ptr<Impl> clone() const override { return std::make_unique<EndstoneExactIngredient>(*this); }

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

    [[nodiscard]] std::unique_ptr<Impl> clone() const override
    {
        return std::make_unique<EndstoneItemTypeIngredient>(*this);
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

    [[nodiscard]] std::unique_ptr<Impl> clone() const override
    {
        return std::make_unique<EndstoneItemTagIngredient>(*this);
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

    [[nodiscard]] std::unique_ptr<Impl> clone() const override
    {
        return std::make_unique<EndstoneMolangIngredient>(*this);
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

    [[nodiscard]] std::unique_ptr<Impl> clone() const override
    {
        return std::make_unique<EndstoneComplexAliasIngredient>(*this);
    }

    [[nodiscard]] const std::string &getAlias() const override { return alias_; }

private:
    std::string alias_;
};

}  // namespace endstone::core
