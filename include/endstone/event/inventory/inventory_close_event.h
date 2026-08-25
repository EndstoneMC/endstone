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

#include "endstone/event/inventory/inventory_event.h"
#include "endstone/player.h"
#include "endstone/util/pointers.h"

namespace endstone {

/**
 * Called when a player closes an inventory.
 */
class InventoryCloseEvent : public InventoryEvent {
public:
    ENDSTONE_EVENT(InventoryCloseEvent);

    InventoryCloseEvent(Inventory &inventory, const NotNull<Player> &player)
        : InventoryEvent(inventory), player_(player) {};

    /**
     * Returns the player involved in this event.
     *
     * @return Player who is closing the inventory
     */
    [[nodiscard]] const NotNull<Player> &getPlayer() const { return player_; }

private:
    NotNull<Player> player_;
};

}  // namespace endstone
