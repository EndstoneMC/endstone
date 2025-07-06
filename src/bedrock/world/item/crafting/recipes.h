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

#include <functional>
#include <memory>

#include "bedrock/core/string/string_hash.h"
#include "bedrock/resources/resource_pack_manager.h"
#include "bedrock/world/item/crafting/recipe.h"

using RecipeListenerList = std::vector<std::pair<std::weak_ptr<bool>, std::function<void()>>>;

class ExternalRecipeStore {
private:
    BlockReducer *block_reducer_ = nullptr;
};

class Level;
class Recipes {
public:
    static constexpr int RECIPE_MAXIMUM_WIDTH = 3;
    static constexpr int RECIPE_MAXIMUM_HEIGHT = 3;
    static constexpr int RECIPE_TOP_PRIORITY = 0;
    static constexpr int DEFAULT_PRIORITY = 50;

    struct FurnaceRecipeKey {
        FurnaceRecipeKey(int id, const HashedString &tag) : id(id), tag(tag) {}
        bool operator<(const FurnaceRecipeKey &) const;
        int id;
        HashedString tag;
    };

    Recipes();
    Recipes(Level *);
    [[nodiscard]] ItemInstance getFurnaceRecipeResult(const ItemStackBase &, const HashedString &) const;

private:
    ResourcePackManager *resource_pack_manager_;
    ExternalRecipeStore external_recipe_store_;
    std::map<HashedString, std::map<std::string, std::shared_ptr<Recipe>>> recipes_;
    std::map<FurnaceRecipeKey, ItemInstance> furnace_recipes_;
    bool initializing_;
    std::map<ItemInstance, std::unordered_map<std::string, Recipe *>, SortItemInstanceIdAux> recipes_by_output_;
    std::unordered_map<TypedServerNetId<RecipeNetIdTag, std::uint32_t, 0U>, Recipe *> recipes_by_net_id_;
    std::unordered_map<std::uint64_t, std::unordered_map<std::uint64_t, std::shared_ptr<std::vector<ItemInstance>>>>
        recipes_by_input_;
    std::vector<gsl::not_null<Recipe *>> unlockable_recipes_;
    RecipeListenerList listeners_;
    Level *level_;
};
