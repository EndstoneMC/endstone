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

#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/containers/container_enum.h"
#include "bedrock/world/containers/managers/enchanting_container_manager_model.h"
#include "bedrock/world/inventory/network/crafting/item_stack_request_action_craft.h"
#include "bedrock/world/inventory/network/crafting/item_stack_request_action_craft_handler.h"
#include "bedrock/world/inventory/network/item_stack_request_action_handler.h"
#include "bedrock/world/simple_sparse_container.h"
#include "endstone/core/block/block.h"
#include "endstone/core/enchantments/enchantment.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/enchantment/enchant_item_event.h"
#include "endstone/runtime/hook.h"

namespace {
constexpr int kEnchantingInputSlot = 14;

const endstone::Enchantment *getEndstoneEnchantment(const EnchantmentInstance &instance)
{
    const auto *enchant = Enchant::getEnchant(instance.getEnchantType());
    if (enchant == nullptr) {
        return nullptr;
    }
    return endstone::Enchantment::get(endstone::EnchantmentId::minecraft(enchant->getStringId().getString()));
}

endstone::EnchantItemEvent::Enchantments getEnchantments(const ItemEnchants &item_enchants)
{
    endstone::EnchantItemEvent::Enchantments result;
    for (const auto &instance : item_enchants.getAllEnchants()) {
        if (const auto *enchantment = getEndstoneEnchantment(instance)) {
            result.insert_or_assign(enchantment, instance.getEnchantLevel());
        }
    }
    return result;
}

ItemEnchants::Enchantments getMinecraftEnchantments(const endstone::EnchantItemEvent::Enchantments &enchants,
                                                    const ItemEnchants::Enchantments &original_enchants)
{
    ItemEnchants::Enchantments result;
    for (const auto &[enchantment, level] : enchants) {
        if (enchantment == nullptr) {
            continue;
        }

        const auto type =
            static_cast<const endstone::core::EndstoneEnchantment &>(*enchantment).getHandle().getEnchantType();
        auto activation = std::size_t{0};
        for (std::size_t i = 0; i < original_enchants.size(); ++i) {
            if (std::ranges::any_of(original_enchants[i],
                                    [type](const auto &instance) { return instance.getEnchantType() == type; })) {
                activation = i;
                break;
            }
        }
        result[activation].emplace_back(type, level);
    }
    return result;
}

bool wasItemModified(const endstone::ItemStack &item, const endstone::ItemStack &original_item)
{
    return item.getType() != original_item.getType() || item.getAmount() != original_item.getAmount() ||
           item.getData() != original_item.getData() || item.getNbt() != original_item.getNbt();
}
}  // namespace

ItemStackNetResult CraftHandlerEnchant::_handleCraftAction(const ItemStackRequestActionCraftBase &request_action)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (request_action.getActionType() != ItemStackRequestActionType::CraftRecipe ||
        !server.getEndstonePluginManager().isEventRegistered<endstone::EnchantItemEvent>()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
    }

    const auto &container_manager = getPlayer().getContainerManagerModel();
    if (container_manager == nullptr) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
    }

    auto &model = static_cast<EnchantingContainerManagerModel &>(*container_manager);
    auto &options = model.getEnchantOptions();
    const auto &recipe_net_id = static_cast<const ItemStackRequestActionCraftRecipe &>(request_action).getRecipeNetId();
    const auto option_it = std::ranges::find(options, recipe_net_id, &ItemEnchantOption::getEnchantNetId);
    if (option_it == options.end()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
    }

    const auto input_container = craft_request_handler_.getRequestActionHandler()._getOrInitSparseContainer(
        FullContainerName{ContainerEnumName::EnchantingInputContainer, std::nullopt});
    if (input_container == nullptr) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
    }

    const auto &input = input_container->getItem(kEnchantingInputSlot);
    if (input.isNull() || input.getCount() == 0) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
    }

    const auto original_exp_level_cost = option_it->getCost();
    const auto original_enchants_to_add = getEnchantments(option_it->getEnchants());
    const auto original_item = endstone::core::EndstoneItemStack::fromMinecraft(input);
    const auto all_enchants = option_it->getEnchants().getAllEnchants();
    const auto *enchantment_hint = all_enchants.empty() ? nullptr : getEndstoneEnchantment(all_enchants.front());
    const auto level_hint = all_enchants.empty() ? 0 : all_enchants.front().getEnchantLevel();
    endstone::EnchantItemEvent event{
        getPlayer().getEndstoneActor<endstone::core::EndstonePlayer>(),
        endstone::core::EndstoneBlock::at(getPlayer().getDimensionBlockSource(), model.getBlockPos()),
        original_item,
        original_exp_level_cost,
        original_enchants_to_add,
        enchantment_hint,
        level_hint,
        static_cast<int>(std::distance(options.begin(), option_it)),
    };
    server.getPluginManager().callEvent(event);
    if (event.isCancelled() || event.getEnchantsToAdd().empty()) {
        return ItemStackNetResult::FailedToEnchant;
    }

    if (event.getExpLevelCost() != original_exp_level_cost) {
        option_it->setCost(event.getExpLevelCost());
    }
    if (event.getEnchantsToAdd() != original_enchants_to_add) {
        option_it->getEnchants().setEnchantInstances(
            getMinecraftEnchantments(event.getEnchantsToAdd(), option_it->getEnchants().getEnchantInstances()));
    }

    // The Endstone ItemStack round-trip does not carry ItemStack's network id. Leave an untouched stack in place;
    // when a listener changes it, preserve the id that BDS uses to validate the request.
    if (wasItemModified(event.getItem(), original_item)) {
        auto enchanted_item = endstone::core::EndstoneItemStack::toMinecraft(event.getItem());
        enchanted_item.copyItemStackNetIdVariant(input);
        input_container->setItem(kEnchantingInputSlot, enchanted_item);
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
}
