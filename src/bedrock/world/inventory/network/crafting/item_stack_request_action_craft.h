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

#include <cstdint>

#include "bedrock/bedrock.h"
#include "bedrock/world/inventory/network/item_stack_request_action.h"
#include "bedrock/world/item/crafting/recipe.h"

class ItemStackRequestActionCraftBase : public ItemStackRequestAction {
public:
    [[nodiscard]] std::uint8_t getNumCrafts() const { return num_crafts_; }

protected:
    std::uint8_t num_crafts_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackRequestActionCraftBase, 24, 16);

template <typename TRecipeNetId, ItemStackRequestActionType ActionType>
class ItemStackRequestActionCraft : public ItemStackRequestActionCraftBase {
public:
    [[nodiscard]] virtual const TRecipeNetId &getRecipeNetId() const { return recipe_net_id_; }

private:
    TRecipeNetId recipe_net_id_;
};

using ItemStackRequestActionCraftRecipe =
    ItemStackRequestActionCraft<RecipeNetId, ItemStackRequestActionType::CraftRecipe>;
BEDROCK_STATIC_ASSERT_SIZE(ItemStackRequestActionCraftRecipe, 32, 16);

class ItemStackRequestActionCraftRecipeAuto
    : public ItemStackRequestActionCraft<RecipeNetId, ItemStackRequestActionType::CraftRecipeAuto> {
private:
    Recipe::Ingredients ingredients_;
    std::uint8_t num_ingredients_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackRequestActionCraftRecipeAuto, 64, 48);
