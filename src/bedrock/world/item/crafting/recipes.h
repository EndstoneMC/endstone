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

#include "bedrock/core/string/string_hash.h"
#include "bedrock/resources/resource_pack_manager.h"
#include "bedrock/world/item/crafting/recipe.h"

class ExternalRecipeStore {
    char filler_[8];
};

struct SortItemInstanceIdAux {};

class Level;

class Recipes {
    struct FurnaceRecipeKey {
        int id;
        HashedString tag;
        bool operator<(const FurnaceRecipeKey &) const;
    };
    ResourcePackManager *resource_pack_manager_;
    ExternalRecipeStore external_recipe_store_;
    std::map<HashedString, std::map<std::string, std::shared_ptr<Recipe>>> recipes_;
    std::map<FurnaceRecipeKey, ItemInstance> furnace_recipes_;
    bool initializing_;
    std::map<ItemInstance, std::unordered_map<std::string, Recipe *>, SortItemInstanceIdAux> recipes_by_output_;
    std::unordered_map<RecipeNetId, Recipe *> recipes_by_net_id_;
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, std::shared_ptr<std::vector<ItemInstance>>>>
        recipes_by_input_;
    std::vector<gsl::not_null<Recipe *>> unlockable_recipes_;
    std::vector<std::pair<std::weak_ptr<bool>, ::std::function<void()>>> listeners_;
    Level *level_;

public:
    [[nodiscard]] ItemInstance getFurnaceRecipeResult(::ItemStackBase const &item, ::HashedString const &tag) const;
};
