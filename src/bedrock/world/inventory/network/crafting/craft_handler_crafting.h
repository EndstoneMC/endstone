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

#include "bedrock/world/inventory/network/crafting/craft_handler_base.h"
#include "bedrock/world/item/crafting/recipe.h"
#include "bedrock/world/item/item_stack.h"

class Player;

class CraftHandlerCrafting : public CraftHandlerBase {
protected:
    CraftHandlerCrafting();

    ENDSTONE_HOOK ItemStackNetResult _handleCraftAction(const ItemStackRequestActionCraftBase &request_action) override;

private:
    bool workbench_;
    Player &player_;
    RecipeNetId recipe_net_id_;
    std::uint8_t deferred_craft_count_;
    std::vector<ItemStack> consumed_items_;
};
BEDROCK_STATIC_ASSERT_SIZE(CraftHandlerCrafting, 152, 144);
