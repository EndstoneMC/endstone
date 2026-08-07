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
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/world/inventory/network/item_stack_request_action_type.h"
#include "bedrock/world/item/crafting/recipe.h"

class Player;
class ItemStackRequestActionHandlerContext;

class ItemStackRequestAction {
public:
    virtual ~ItemStackRequestAction() = default;

    // Endstone: private -> public
    ItemStackRequestActionType type_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackRequestAction, 16, 16);

class ItemStackRequestActionCraftBase : public ItemStackRequestAction {
public:
    // Endstone: private -> public
    std::uint8_t craft_count_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackRequestActionCraftBase, 24, 16);

class ItemStackRequestActionCraftRecipe : public ItemStackRequestActionCraftBase {
public:
    // Endstone: private -> public
    RecipeNetId recipe_net_id_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackRequestActionCraftRecipe, 32, 16);

class ItemStackRequestActionCraftRecipeAuto : public ItemStackRequestActionCraftRecipe {
public:
    // Endstone: private -> public
    std::vector<RecipeIngredient> ingredients_;

    // Endstone: private -> public
    std::uint8_t ingredient_count_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackRequestActionCraftRecipeAuto, 64, 48);

class ItemStackRequestActionHandler {
public:
    virtual ~ItemStackRequestActionHandler() = default;

    // Endstone
    [[nodiscard]] Player *getPlayer() const { return player_; }

private:
    ItemStackRequestActionHandlerContext *context_;
    Player *player_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackRequestActionHandler, 24, 24);
