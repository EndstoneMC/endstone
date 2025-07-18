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

#include "endstone/actor/item.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * @brief Called when a player picks an item up from the ground.
 */
class PlayerPickupItemEvent : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerPickupItemEvent);

    explicit PlayerPickupItemEvent(Player &player, Item &item) : Cancellable(player), item_(item){};

    /**
     * @brief Gets the Item picked up by the entity.
     *
     * @return Item
     */
    Item &getItem() const
    {
        return item_;
    }

private:
    Item &item_;
};

}  // namespace endstone
