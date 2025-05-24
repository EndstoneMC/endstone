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

#include "bedrock/world/actor/player/player.h"

#include <entt/entt.hpp>

#include "bedrock/entity/components/abilities_component.h"
#include "bedrock/network/packet/available_commands_packet.h"
#include "bedrock/world/item/item.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_item_consume_event.h"
#include "endstone/event/player/player_teleport_event.h"
#include "endstone/runtime/hook.h"
void Player::teleportTo(const Vec3 &pos, bool should_stop_riding, int cause, int entity_type, bool keep_velocity)
{
    Vec3 position = pos;
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();
    const endstone::Location to{&player.getDimension(), pos.x, pos.y, pos.z, getRotation().x, getRotation().y};
    endstone::PlayerTeleportEvent e{player, player.getLocation(), to};
    server.getPluginManager().callEvent(e);

    if (e.isCancelled()) {
        return;
    }
    position = {e.getTo().getX(), e.getTo().getY(), e.getTo().getZ()};
    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::teleportTo, this, position, should_stop_riding, cause, entity_type,
                                keep_velocity);
}

void Player::completeUsingItem()
{
    auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    const auto *item = item_in_use_.item.getItem();
    const std::set<std::string> item_names{"minecraft:potion", "minecraft:milk_bucket", "minecraft:medicine"};
    if (!item->isFood() && !item_names.contains(item->getFullItemName())) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&Player::completeUsingItem, this);
        return;
    }
    const auto item_stack = endstone::core::EndstoneItemStack::fromMinecraft(item_in_use_.item);
    endstone::PlayerItemConsumeEvent e{*endstone::core::EndstonePlayer::create(server, *this), item_stack.get()};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        stopUsingItem();
    }
    else {
        ENDSTONE_HOOK_CALL_ORIGINAL(&Player::completeUsingItem, this);
        return;
    }
}
