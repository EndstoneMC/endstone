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

#include "bedrock/world/actor/player/player.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/enchantment/enchant_item_event.h"
#include "endstone/runtime/hook.h"

ItemStackNetResult CraftHandlerEnchant::_handleCraftAction(const ItemStackRequestActionCraftBase &request_action)
{
    const auto player = getPlayer().getEndstoneActor<endstone::core::EndstonePlayer>();
    if (player) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        endstone::EnchantItemEvent e{player};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return ItemStackNetResult::FailedToEnchant;
        }
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CraftHandlerEnchant::_handleCraftAction, this, request_action);
}
