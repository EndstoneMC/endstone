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

#include <utility>

#include "endstone/event/player/player_interact_actor_event.h"
#include "endstone/inventory/equipment_slot.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

/**
 * Called when a player interacts with an armor stand and will either swap, retrieve or place an item.
 */
class PlayerArmorStandManipulateEvent final : public PlayerInteractActorEvent {
public:
    ENDSTONE_EVENT(PlayerArmorStandManipulateEvent);

    PlayerArmorStandManipulateEvent(const NotNull<Player> &player, const NotNull<Actor> &right_clicked,
                                    ItemStack armor_stand_item, ItemStack player_item, EquipmentSlot slot)
        : PlayerInteractActorEvent(player, right_clicked), armor_stand_item_(std::move(armor_stand_item)),
          player_item_(std::move(player_item)), slot_(slot)
    {
    }

    /**
     * Gets the item held by the armor stand in the affected slot.
     *
     * @return the item held by the armor stand
     */
    [[nodiscard]] const ItemStack &getArmorStandItem() const { return armor_stand_item_; }

    /**
     * Gets the item held by the player during the interaction.
     *
     * @return the item held by the player
     */
    [[nodiscard]] const ItemStack &getPlayerItem() const { return player_item_; }

    /**
     * Gets the armor stand equipment slot affected by the interaction.
     *
     * @return the affected armor stand slot
     */
    [[nodiscard]] EquipmentSlot getSlot() const { return slot_; }

private:
    ItemStack armor_stand_item_;
    ItemStack player_item_;
    EquipmentSlot slot_;
};

}  // namespace endstone
