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

#include <map>
#include <memory>

#include "bedrock/core/string/string_hash.h"
#include "bedrock/resources/min_engine_version.h"
#include "bedrock/world/item/crafting/recipe.h"
#include "bedrock/world/item/crafting/shaped_recipe.h"
#include "bedrock/world/item/crafting/shapeless_recipe.h"

class Recipes {
public:
    struct Type {
        Item *item;
        Block const *block;
        RecipeIngredient ingredient;
        char c;
    };
    using RecipesMap = std::map<HashedString, std::map<std::string, std::shared_ptr<Recipe>>>;
    bool loadRecipe(std::pair<std::string, Json::Value> const &recipe_obj_info,
                    MinEngineVersion const &min_engine_version, SemVersion const &format_version,
                    bool is_base_game_pack);
    RecipesMap &getRecipesAllTags();
    void addShapedRecipe(
        std::string recipe_id, std::vector<::ItemInstance> const &result, std::vector<std::string> const &rows,
        std::vector<::Recipes::Type> const &types, std::vector<::HashedString> const &tags, int priority,
        std::function<std::unique_ptr<ShapedRecipe>(
            std::string, int, int, std::vector<RecipeIngredient> const &, std::vector<ItemInstance> const &,
            HashedString, bool, int, mce::UUID const *, RecipeUnlockingRequirement const &, SemVersion const &)>
            constructor,
        RecipeUnlockingRequirement const &unlocking_req, SemVersion const &format_version, bool assume_symmetry);
    void addShapelessRecipe(
        std::string recipe_id, ::ItemInstance const &result, std::vector<::Recipes::Type> const &types,
        std::vector<::HashedString> const &tags, int priority,
        std::function<std::unique_ptr<::ShapelessRecipe>(
            std::string, std::vector<::RecipeIngredient> const &, std::vector<::ItemInstance> const &, HashedString,
            int, mce::UUID const *, RecipeUnlockingRequirement const &, SemVersion const &)>
            constructor,
        RecipeUnlockingRequirement const &unlocking_req, SemVersion const &format_version);
    void addFurnaceRecipeAuxData(ItemInstance const &input, ItemInstance const &result,
                                 std::vector<::HashedString> const &tags);
};
