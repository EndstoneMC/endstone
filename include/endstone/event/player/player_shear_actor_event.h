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

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/equipment_slot.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

class Actor;

/**
 * Called when a player shears an Actor.
 */
class PlayerShearActorEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerShearActorEvent);

    PlayerShearActorEvent(const NotNull<Player> &player, const NotNull<Actor> &actor, ItemStack item,
                          EquipmentSlot hand)
        : Cancellable(player), actor_(actor), item_(std::move(item)), hand_(hand)
    {
    }

    /**
     * Gets the Actor the player is shearing.
     *
     * @return the Actor the player is shearing
     */
    [[nodiscard]] const NotNull<Actor> &getActor() const { return actor_; }

    /**
     * Gets the item used to shear the Actor.
     *
     * @return the shears
     */
    [[nodiscard]] const ItemStack &getItem() const { return item_; }

    /**
     * Gets the hand used to shear the Actor.
     *
     * @return the hand
     */
    [[nodiscard]] EquipmentSlot getHand() const { return hand_; }

private:
    NotNull<Actor> actor_;
    ItemStack item_;
    EquipmentSlot hand_;
};

}  // namespace endstone
