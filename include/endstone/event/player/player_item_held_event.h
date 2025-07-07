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
 * @brief Called when a player changes their currently held item.
 *
 */
class PlayerItemHeldEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerItemHeldEvent)

    explicit PlayerItemHeldEvent(Player &player, int new_slot, int previous_slot)
        : Cancellable(player), new_slot_(new_slot), previous_slot_(previous_slot)
    {
    }

    /**
     * @brief Gets the new held slot index.
     */
    [[nodiscard]] int getNewSlot() const
    {
        return new_slot_;
    }

    /**
     * @brief Gets the previous held slot index.
     */
    [[nodiscard]] int getPreviousSlot() const
    {
        return previous_slot_;
    }

private:
    int new_slot_;
    int previous_slot_;
};
}  // namespace endstone
