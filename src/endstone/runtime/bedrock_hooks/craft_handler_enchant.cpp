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

#include "bedrock/world/inventory/network/crafting/craft_handler_enchant.h"

#include <algorithm>
#include <iterator>
#include <utility>

#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/containers/container_enum.h"
#include "bedrock/world/containers/managers/enchanting_container_manager_model.h"
#include "bedrock/world/inventory/network/crafting/item_stack_request_action_craft.h"
#include "bedrock/world/inventory/network/crafting/item_stack_request_action_craft_handler.h"
#include "bedrock/world/inventory/network/item_stack_request_action_handler.h"
#include "bedrock/world/simple_sparse_container.h"
#include "endstone/core/block/block.h"
#include "endstone/core/enchantments/enchantment.h"
#include "endstone/core/inventory/inventory.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/enchantment/enchant_item_event.h"
#include "endstone/runtime/hook.h"

ItemStackNetResult CraftHandlerEnchant::_handleCraftAction(const ItemStackRequestActionCraftBase &request_action)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (request_action.getActionType() != ItemStackRequestActionType::CraftRecipe ||
        !server.getEndstonePluginManager().isEventRegistered<endstone::EnchantItemEvent>()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
    }

    const auto &container_manager = player_.getContainerManagerModelRef();
    if (container_manager == nullptr) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
    }

    auto &model = static_cast<EnchantingContainerManagerModel &>(*container_manager);
    auto &options = model.getEnchantOptions();
    const auto &recipe_net_id = static_cast<const ItemStackRequestActionCraftRecipe &>(request_action).getRecipeNetId();
    const auto option = std::ranges::find(options, recipe_net_id, &ItemEnchantOption::enchant_net_id);
    if (option == options.end()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
    }

    const auto input_container = craft_request_handler_.getActionHandler()._getOrInitSparseContainer(
        FullContainerName{ContainerEnumName::EnchantingInputContainer, std::nullopt});
    if (input_container == nullptr) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
    }

    const auto &input = input_container->getItem(14);
    if (!input) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
    }

    const auto which_button = static_cast<int>(std::distance(options.begin(), option));
    if (!player_.isCreative()) {
        if (option->cost > player_.getPlayerLevel()) {
            return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
        }
        const auto &lapis = input_container->getItem(15);
        if (!lapis || which_button + 1 > lapis.getCount()) {
            return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
        }
    }

    endstone::EnchantItemEvent::Enchantments enchants_to_add;
    for (const auto &instance : option->enchants.getAllEnchants()) {
        const auto *enchant = Enchant::getEnchant(instance.getEnchantType());
        if (enchant == nullptr) {
            return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
        }
        const auto *enchantment =
            endstone::Enchantment::get(endstone::EnchantmentId::minecraft(enchant->getStringId().getString()));
        if (enchantment == nullptr) {
            return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
        }
        enchants_to_add.insert_or_assign(enchantment, instance.getEnchantLevel());
    }

    const auto original_item = endstone::core::EndstoneItemStack::fromMinecraft(input);
    const auto original_cost = option->cost;
    const auto original_enchants_to_add = enchants_to_add;

    endstone::core::EndstoneInventory inventory{[&]() -> ::Container & { return *input_container; }};
    endstone::EnchantItemEvent event{
        inventory,
        player_.getEndstoneActor<endstone::core::EndstonePlayer>(),
        endstone::core::EndstoneBlock::at(player_.getDimensionBlockSource(), model.getBlockPos()),
        original_item,
        original_cost,
        std::move(enchants_to_add),
        which_button,
    };
    server.getPluginManager().callEvent(event);
    if (event.isCancelled() || event.getEnchantsToAdd().empty()) {
        return ItemStackNetResult::FailedToEnchant;
    }

    option->cost = event.getExpLevelCost();
    if (event.getEnchantsToAdd() != original_enchants_to_add) {
        ItemEnchants::Enchantments instances;
        for (const auto &[enchantment, level] : event.getEnchantsToAdd()) {
            if (enchantment == nullptr) {
                continue;
            }
            instances[0].emplace_back(
                static_cast<const endstone::core::EndstoneEnchantment &>(*enchantment).getHandle().getEnchantType(),
                level);
        }
        option->enchants.setEnchantInstances(std::move(instances));
    }

    const auto &item = event.getItem();
    if (item.getType() != original_item.getType() || item.getAmount() != original_item.getAmount() ||
        item.getData() != original_item.getData() || item.getNbt() != original_item.getNbt()) {
        auto enchanted_item = endstone::core::EndstoneItemStack::toMinecraft(item);
        enchanted_item.copyItemStackNetIdVariant(input);
        input_container->setItem(14, enchanted_item);
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
}
