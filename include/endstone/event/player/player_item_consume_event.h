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
class PlayerItemConsumeEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerItemConsumeEvent)

    explicit PlayerItemConsumeEvent(Player &player, const ItemStack &item, EquipmentSlot hand)
        : Cancellable(player), item_(item.clone()), hand_(hand)
    {
    }

    /**
     * @brief Gets the item that is being consumed.
     *
     * @note Modifying the returned item will have no effect, you must use setItem(ItemStack) instead.
     *
     * @return an ItemStack for the item being consumed
     */
    [[nodiscard]] std::unique_ptr<ItemStack> getItem() const
    {
        return item_->clone();
    }

    /**
     * @brief Set the item being consumed
     *
     * @param item the item being consumed
     */
    void setItem(std::unique_ptr<ItemStack> item)
    {
        if (item == nullptr) {
            item_ = std::make_unique<ItemStack>("minecraft:air");
        }
        else {
            item_ = std::move(item);
        }
    }

    /**
     * @brief Get the hand used to consume the item.
     *
     * @return the hand
     */
    [[nodiscard]] EquipmentSlot getHand() const
    {
        return hand_;
    }

private:
    std::unique_ptr<ItemStack> item_;
    EquipmentSlot hand_;
};
}  // namespace endstone
