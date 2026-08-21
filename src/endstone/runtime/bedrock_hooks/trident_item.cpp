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

#include "bedrock/world/item/trident_item.h"

#include "bedrock/entity/components/riptide_trident_spin_attack_component.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_riptide_event.h"
#include "endstone/runtime/hook.h"

void TridentItem::releaseUsing(ItemStack &item_stack, Player *player, int duration_left) const
{
    const auto item = endstone::core::EndstoneItemStack::fromMinecraft(item_stack);
    const auto was_spin_attacking = player && player->hasComponent<RiptideTridentSpinAttackComponent>();
    ENDSTONE_HOOK_CALL_ORIGINAL(&TridentItem::releaseUsing, this, item_stack, player, duration_left);
    if (was_spin_attacking || !player || !player->hasComponent<RiptideTridentSpinAttackComponent>()) {
        return;
    }
    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::PlayerRiptideEvent e{player->getEndstoneActor<endstone::core::EndstonePlayer>(), item};
    server.getPluginManager().callEvent(e);
}
