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

#include <iostream>

#include <entt/entt.hpp>

#include "bedrock/world/actor/item/item_actor.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_drop_item_event.h"
#include "endstone/event/player/player_item_consume_event.h"
#include "endstone/event/player/player_pickup_item_event.h"
#include "endstone/event/player/player_teleport_event.h"
#include "endstone/runtime/hook.h"

void Player::teleportTo(const Vec3 &pos, bool should_stop_riding, int cause, int entity_type, bool keep_velocity)
{
    // Do not call PlayerTeleportEvent if EndstonePlayer::teleport was called internally
    // - (for example, when PlayerMoveEvent is cancelled).
    if (hasComponent<endstone::core::InternalTeleportFlagComponent>()) {
        addOrRemoveComponent<endstone::core::InternalTeleportFlagComponent>(false);
        ENDSTONE_HOOK_CALL_ORIGINAL(&Player::teleportTo, this, pos, should_stop_riding, cause, entity_type,
                                    keep_velocity);
        return;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();
    const endstone::Location to{&player.getDimension(), pos.x, pos.y, pos.z, getRotation().x, getRotation().y};
    endstone::PlayerTeleportEvent e{player, player.getLocation(), to};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return;
    }

    auto final_pos = Vec3(e.getTo().getX(), e.getTo().getY(), e.getTo().getZ());
    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::teleportTo, this, final_pos, should_stop_riding, cause, entity_type,
                                keep_velocity);
}

void Player::completeUsingItem()
{
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    const auto item = endstone::core::EndstoneItemStack::fromMinecraft(item_in_use_.getItemInUse());
    const auto hand = inventory_->getSelectedSlot().container_id == CONTAINER_ID_INVENTORY
                        ? endstone::EquipmentSlot::Hand
                        : endstone::EquipmentSlot::OffHand;
    endstone::PlayerItemConsumeEvent e{getEndstoneActor<endstone::core::EndstonePlayer>(), *item, hand};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        setStatusFlag(ActorFlags::USINGITEM, false);
        item_in_use_.clearItemInUse(getEntity());
        return;
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::completeUsingItem, this);
}

bool Player::drop(const ItemStack &item, bool randomly)
{
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();
    if (isAlive()) {
        const auto drop = endstone::core::EndstoneItemStack::fromMinecraft(item);
        endstone::PlayerDropItemEvent e(player, *drop);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::drop, this, item, randomly);
}

bool Player::take(Actor &actor, int unknown, int favored_slot)
{
    if (isClientSide() || !canInteractWithOtherEntitiesInGame()) {
        return false;
    }

    if (actor.hasCategory(ActorCategory::Item)) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();
        const auto &item_stack = static_cast<ItemActor &>(actor).getItemStack();
        const auto item = endstone::core::EndstoneItemStack::fromMinecraft(item_stack);
        endstone::PlayerPickupItemEvent e(player, *item);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::take, this, actor, unknown, favored_slot);
}
