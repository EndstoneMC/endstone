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

#include "bedrock/core/sem_ver/sem_version.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/platform/uuid.h"
#include "bedrock/world/inventory/network/item_stack_net_id_variant.h"
#include "bedrock/world/item/crafting/crafting_container.h"
#include "bedrock/world/item/crafting/crafting_context.h"
#include "bedrock/world/item/crafting/recipe_ingredient.h"
#include "bedrock/world/item/crafting/recipe_unlocking_requirement.h"
#include "bedrock/world/item/item_instance.h"
#include "bedrock/world/item/network_item_instance_descriptor.h"

struct RecipeNetIdTag {};
using RecipeNetId = TypedServerNetId<RecipeNetIdTag, unsigned int>;

template <>
struct std::hash<RecipeNetId> {
    std::size_t operator()(const RecipeNetId &net_id) const { return net_id.getHash(); }
};

class Recipe {
public:
    using Ingredients = std::vector<RecipeIngredient>;
    using ResultList = std::vector<ItemInstance>;
    using UnloadedItemInstanceResultList = std::vector<NetworkItemInstanceDescriptor>;

    class Results {
    public:
        Results() = default;
        Results(const ResultList &);
        Results(const UnloadedItemInstanceResultList &);

        [[nodiscard]] const ResultList &getItems() const { return results_; }

    private:
        friend class Recipe;
        bool results_are_loaded_{false};  // +0
        ResultList results_;              // +8
        UnloadedItemInstanceResultList unloaded_results_;
    };

    struct ConstructionContext {
        std::string recipe_id;
        Ingredients ingredients;
        Results results;
        HashedString tag;
        int priority{0};
        const mce::UUID *uuid{nullptr};
        RecipeUnlockingRequirement unlocking_requirement;
        SemVersion format_version;
    };

    static constexpr int SIZE_2X2 = 0;
    static constexpr int SIZE_3X3 = 1;

    Recipe(Recipe &&) = default;
    Recipe(const Recipe &) = default;
    virtual ~Recipe() = default;

    virtual const ResultList &assemble(CraftingContainer &, CraftingContext &) const = 0;
    virtual int getCraftingSize() const = 0;
    virtual const RecipeIngredient &getIngredient(int, int) const = 0;
    virtual bool isShapeless() const = 0;
    virtual bool matches(const CraftingContainer &, const CraftingContext &) const = 0;
    virtual int size() const = 0;
    [[nodiscard]] virtual const mce::UUID &getId() const;
    [[nodiscard]] virtual const ResultList &getResultItems() const;
    [[nodiscard]] virtual bool isMultiRecipe() const;
    [[nodiscard]] virtual bool hasDataDrivenResult() const;
    [[nodiscard]] virtual bool itemValidForRecipe(const ItemDescriptor &, const ItemStack &) const;
    [[nodiscard]] virtual bool itemsMatch(const ItemDescriptor &, const ItemDescriptor &) const;
    [[nodiscard]] virtual bool itemsMatch(const ItemDescriptor &, const ItemDescriptor &, const CompoundTag *) const;
    [[nodiscard]] virtual std::size_t getIngredientsHash() const;

    static bool isAnyAuxValue(const ItemDescriptor &ii);

    [[nodiscard]] const std::string &getRecipeId() const { return recipe_id_; }
    [[nodiscard]] int getWidth() const { return width_; }
    [[nodiscard]] int getHeight() const { return height_; }
    [[nodiscard]] int getPriority() const { return priority_; }
    [[nodiscard]] const RecipeNetId &getNetId() const { return recipe_net_id_; }
    [[nodiscard]] const Ingredients &getIngredients() const { return my_ingredients_; }
    [[nodiscard]] const RecipeUnlockingRequirement &getUnlockingRequirement() const { return unlocking_requirement_; }
    [[nodiscard]] const HashedString &getTag() const { return tag_; }
    ConstructionContext getConstructionContext() const;
    void setNetId(const RecipeNetId &recipe_net_id) { recipe_net_id_ = recipe_net_id; }
    void setId(const mce::UUID &uuid) { my_id_ = uuid; }
    void setUnlockingRequirement(RecipeUnlockingRequirement requirement)
    {
        unlocking_requirement_ = std::move(requirement);
    }
    void generateUUID();

protected:
    explicit Recipe(ConstructionContext &&);
    Recipe();

    std::string recipe_id_;
    mce::UUID my_id_;
    int width_{0};
    int height_{0};
    int priority_{0};
    RecipeNetId recipe_net_id_;
    Ingredients my_ingredients_;
    Results results_;
    RecipeUnlockingRequirement unlocking_requirement_;
    SemVersion recipe_data_version_;

private:
    HashedString tag_;
};
BEDROCK_STATIC_ASSERT_SIZE(Recipe, 256, 256);
BEDROCK_STATIC_ASSERT_SIZE(Recipe::Results, 56, 56);
BEDROCK_STATIC_ASSERT_SIZE(Recipe::ConstructionContext, 232, 232);
