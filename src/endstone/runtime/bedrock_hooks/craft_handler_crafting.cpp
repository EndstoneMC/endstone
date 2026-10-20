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

#include "bedrock/world/inventory/network/crafting/craft_handler_crafting.h"

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/inventory/network/crafting/item_stack_request_action_craft.h"
#include "bedrock/world/inventory/network/crafting/item_stack_request_action_craft_handler.h"
#include "bedrock/world/item/crafting/recipes.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/inventory/recipe_data.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_craft_item_event.h"
#include "endstone/runtime/hook.h"

ItemStackNetResult CraftHandlerCrafting::_handleCraftAction(const ItemStackRequestActionCraftBase &request_action)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::PlayerCraftItemEvent>()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerCrafting::_handleCraftAction, this, request_action);
    }

    std::vector<endstone::ItemStack> ingredients;
    const Recipe *recipe = nullptr;

    switch (request_action.getActionType()) {
    case ItemStackRequestActionType::CraftRecipe: {
        const auto &net_id = static_cast<const ItemStackRequestActionCraftRecipe &>(request_action).getRecipeNetId();
        recipe = player_.getLevel().getRecipes().getRecipeByNetId(net_id);
        if (recipe == nullptr) {
            break;
        }

        const auto first_slot = 28 + workbench_ * 4;
        const auto slot_count = workbench_ * 5 + 4;
        const auto &player_ui = player_.getPlayerUIContainer();
        for (auto i = 0; i < slot_count; ++i) {
            const auto &item = player_ui.getItem(first_slot + i);
            if (!item.isNull()) {
                ingredients.push_back(endstone::core::EndstoneItemStack::fromMinecraft(item));
            }
        }
        break;
    }
    case ItemStackRequestActionType::CraftRecipeAuto: {
        const auto &net_id =
            static_cast<const ItemStackRequestActionCraftRecipeAuto &>(request_action).getRecipeNetId();
        recipe = player_.getLevel().getRecipes().getRecipeByNetId(net_id);
        if (recipe == nullptr) {
            break;
        }

        for (auto y = 0; y < recipe->getHeight(); ++y) {
            for (auto x = 0; x < recipe->getWidth(); ++x) {
                const auto &ingredient = recipe->getIngredient(x, y);
                const auto *item = ingredient.getItem();
                if (item == nullptr || ingredient.getStackSize() == 0) {
                    continue;
                }
                ingredients.push_back(endstone::core::EndstoneItemStack::fromMinecraft(
                    ItemStack(*item, ingredient.getStackSize(), ingredient.getAuxValue())));
            }
        }
        break;
    }
    default:
        break;
    }

    if (recipe == nullptr) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerCrafting::_handleCraftAction, this, request_action);
    }

    auto crafted = endstone::core::EndstoneRecipeData::fromMinecraft(player_.getLevel().getRecipes(), *recipe);
    if (crafted == nullptr) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerCrafting::_handleCraftAction, this, request_action);
    }

    std::vector<endstone::ItemStack> results;
    results.reserve(recipe->getResultItems().size());
    for (const auto &result : recipe->getResultItems()) {
        results.push_back(endstone::core::EndstoneItemStack::fromMinecraft(ItemStack(result)));
    }

    endstone::PlayerCraftItemEvent e{player_.getEndstoneActor<endstone::core::EndstonePlayer>(),
                                     std::move(crafted), std::move(ingredients), results,
                                     request_action.getNumCrafts()};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return ItemStackNetResult::ActionRequestNotAllowed;
    }

    const auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerCrafting::_handleCraftAction, this, request_action);
    if (result == ItemStackNetResult::Success && e.getResults() != results) {
        auto &craft_results = craft_request_handler_.getCraftResults();
        if (craft_results.empty() && deferred_craft_count_ != 0) {
            std::vector<ItemInstance> replacements;
            replacements.reserve(e.getResults().size());
            for (const auto &replacement : e.getResults()) {
                replacements.emplace_back(endstone::core::EndstoneItemStack::toMinecraft(replacement));
            }
            const auto initialized = craft_request_handler_._initCraftResults(replacements, deferred_craft_count_);
            deferred_craft_count_ = 0;
            if (initialized != ItemStackNetResult::Success) {
                return initialized;
            }
        }
        else {
            const auto replaced = std::min(e.getResults().size(), craft_results.size());
            std::vector<ItemInstance> replacements;
            replacements.reserve(replaced);
            for (std::size_t i = 0; i < replaced; ++i) {
                ItemInstance replacement(endstone::core::EndstoneItemStack::toMinecraft(e.getResults()[i]));
                if (!replacement) {
                    return ItemStackNetResult::InvalidCraftResultItem;
                }
                replacements.push_back(std::move(replacement));
            }
            for (std::size_t i = 0; i < replaced; ++i) {
                craft_results[i].first = replacements[i];
            }
        }
        player_.addOrRemoveComponent<endstone::core::InternalCraftResultReplacedFlagComponent>(true);
    }
    return result;
}
