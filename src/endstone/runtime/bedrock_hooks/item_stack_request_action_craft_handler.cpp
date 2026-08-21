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

#include "bedrock/world/inventory/network/crafting/item_stack_request_action_craft_handler.h"

#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/item/crafting/recipes.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_craft_item_event.h"
#include "endstone/runtime/hook.h"

ItemStackNetResult ItemStackRequestActionCraftHandler::handleCraftAction(
    const ItemStackRequestActionCraftBase &request_action)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    const auto action_type = request_action.getActionType();
    if ((action_type == ItemStackRequestActionType::CraftRecipe ||
         action_type == ItemStackRequestActionType::CraftRecipeAuto) &&
        server.getEndstonePluginManager().isEventRegistered<endstone::PlayerCraftItemEvent>()) {
        const auto &net_id =
            action_type == ItemStackRequestActionType::CraftRecipe
                ? static_cast<const ItemStackRequestActionCraftRecipe &>(request_action).getRecipeNetId()
                : static_cast<const ItemStackRequestActionCraftRecipeAuto &>(request_action).getRecipeNetId();
        const auto *recipe = player_.getLevel().getRecipes().getRecipeByNetId(net_id);
        if (recipe != nullptr && !recipe->getResultItems().empty()) {
            const auto result = ItemStack(recipe->getResultItems().front());
            endstone::PlayerCraftItemEvent e{player_.getEndstoneActor<endstone::core::EndstonePlayer>(),
                                             endstone::core::EndstoneItemStack::fromMinecraft(result),
                                             recipe->getRecipeId(), request_action.getNumCrafts()};
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                return ItemStackNetResult::ActionRequestNotAllowed;
            }
        }
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&ItemStackRequestActionCraftHandler::handleCraftAction, this, request_action);
}
