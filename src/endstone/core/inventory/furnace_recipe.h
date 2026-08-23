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

#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "bedrock/world/item/item_instance.h"
#include "endstone/core/type.h"
#include "endstone/inventory/furnace_recipe.h"

class HashedString;
class ItemStackBase;
class Recipes;

namespace endstone::core {

class EndstoneFurnaceRecipe final : public endstone::FurnaceRecipe {
public:
    static Nullable<endstone::FurnaceRecipe> fromMinecraft(int input_id_aux, const ::ItemInstance &result,
                                                           std::string tag);
    static Nullable<endstone::FurnaceRecipe> fromMinecraft(const ::Recipes &recipes, const ::ItemStackBase &input,
                                                           const ::HashedString &tag);

    EndstoneFurnaceRecipe(endstone::ItemStack result, std::string tag, NotNull<endstone::RecipeIngredient> input)
        : result_(std::move(result)), tag_(std::move(tag)), input_(std::move(input)), ingredients_{input_}
    {
    }

    [[nodiscard]] const std::type_info &getClassTypeId() const override { return typeid(endstone::FurnaceRecipe); }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return core::isInstanceOf(*this, target);
    }

    [[nodiscard]] endstone::ItemStack getResult() const override { return result_; }

    [[nodiscard]] const std::vector<Nullable<endstone::RecipeIngredient>> &getIngredients() const override
    {
        return ingredients_;
    }

    [[nodiscard]] const std::string &getRecipeId() const override { return recipe_id_; }

    [[nodiscard]] const std::string &getTag() const override { return tag_; }

    [[nodiscard]] NotNull<endstone::RecipeIngredient> getInput() const override { return input_; }

private:
    endstone::ItemStack result_;
    std::string recipe_id_;
    std::string tag_;
    NotNull<endstone::RecipeIngredient> input_;
    std::vector<Nullable<endstone::RecipeIngredient>> ingredients_;
};

}  // namespace endstone::core
