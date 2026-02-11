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

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/equipment_slot.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {
/**
 * @brief Called when a player is finishing consuming an item (food, potion, milk bucket).
 *
 * @note If the ItemStack is modified the server will use the effects of the new item and not remove the original one
 * from the player's inventory.
 *
 * @note If the event is cancelled the effect will not be applied and the item will not be removed from the player's
 * inventory.
 */
class PlayerItemConsumeEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerItemConsumeEvent)

    explicit PlayerItemConsumeEvent(Player &player, ItemStack item, EquipmentSlot hand)
        : Cancellable(player), item_(std::move(item)), hand_(hand)
    {
    }

    /**
     * @brief Gets the item that is being consumed.
     *
     * @return an ItemStack for the item being consumed
     */
    [[nodiscard]] const ItemStack &getItem() const { return item_; }

    /**
     * @brief Get the hand used to consume the item.
     *
     * @return the hand
     */
    [[nodiscard]] EquipmentSlot getHand() const { return hand_; }

private:
    ItemStack item_;
    EquipmentSlot hand_;
};
}  // namespace endstone
