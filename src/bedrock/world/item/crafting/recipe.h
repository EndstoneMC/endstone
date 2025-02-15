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

#include "bedrock/platform/uuid.h"
#include "bedrock/resources/base_game_version.h"
#include "bedrock/world/inventory/network/item_stack_net_id_variant.h"
#include "bedrock/world/item/crafting/recipe_ingredient.h"
#include "bedrock/world/item/crafting/recipe_unlocking_requirement.h"
#include "bedrock/world/item/item_instance.h"
#include "bedrock/world/item/network_item_instance_descriptor.h"

struct RecipeNetIdTag {};
using RecipeNetId = TypedServerNetId<RecipeNetIdTag, unsigned int>;

class Recipe {
public:
    using Ingredients = std::vector<RecipeIngredient>;
    using ResultList = std::vector<ItemInstance>;
    using UnloadedItemInstanceResultList = std::vector<NetworkItemInstanceDescriptor>;

    class Results {
    public:
        Results(const ResultList &);
        Results(const UnloadedItemInstanceResultList &);

        [[nodiscard]] const ResultList &getItems() const
        {
            return results_;
        }

    private:
        bool results_are_loaded_;            // +0
        std::vector<ItemInstance> results_;  // +8
        std::vector<NetworkItemInstanceDescriptor> unloaded_results_;
    };

    virtual ~Recipe() = 0;
    virtual std::vector<ItemInstance> const &assemble(CraftingContainer &, CraftingContext &) const = 0;
    virtual int getCraftingSize() const = 0;
    virtual RecipeIngredient const &getIngredient(int, int) const = 0;
    virtual bool isShapeless() const = 0;
    virtual bool matches(CraftingContainer const &, CraftingContext const &) const = 0;
    virtual int size() const = 0;
    virtual mce::UUID const &getId() const = 0;
    virtual std::vector<ItemInstance> const &getResultItems() const = 0;
    virtual bool isMultiRecipe() const = 0;
    virtual bool hasDataDrivenResult() const = 0;
    virtual bool itemValidForRecipe(ItemDescriptor const &, ItemStack const &) const = 0;
    virtual bool itemsMatch(ItemDescriptor const &, ItemDescriptor const &) const = 0;
    virtual bool itemsMatch(ItemDescriptor const &, ItemDescriptor const &, CompoundTag const *) const = 0;
    virtual std::size_t getIngredientsHash() const = 0;

    [[nodiscard]] const std::string &getRecipeId() const
    {
        return recipe_id_;
    }

    [[nodiscard]] int getWidth() const
    {
        return width_;
    }

    [[nodiscard]] int getHeight() const
    {
        return height_;
    }

    [[nodiscard]] int getPriority() const
    {
        return priority_;
    }

    [[nodiscard]] const RecipeNetId &getNetId() const
    {
        return recipe_net_id_;
    }

    [[nodiscard]] const std::vector<RecipeIngredient> &getIngredients() const
    {
        return my_ingredients_;
    }

    [[nodiscard]] const RecipeUnlockingRequirement &getUnlockingRequirement() const
    {
        return unlocking_requirement_;
    }

    [[nodiscard]] const HashedString &getTag() const
    {
        return tag_;
    }

protected:
    Recipe();

    std::string recipe_id_;
    mce::UUID my_id_;
    int width_;
    int height_;
    int priority_;
    RecipeNetId recipe_net_id_;
    Ingredients my_ingredients_;
    Results results_;
    RecipeUnlockingRequirement unlocking_requirement_;
    SemVersion recipe_data_version_;

private:
    HashedString tag_;
};
