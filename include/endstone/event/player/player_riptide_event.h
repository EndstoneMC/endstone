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

#include "endstone/event/player/player_event.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

/**
 * Called when a player activates the riptide enchantment, using their trident to propel them through the air.
 *
 * @note The riptide action is currently performed client side, so manipulating the player in this event may have
 * undesired effects.
 */
class PlayerRiptideEvent final : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerRiptideEvent);

    explicit PlayerRiptideEvent(const NotNull<Player> &player, ItemStack item)
        : PlayerEvent(player), item_(std::move(item))
    {
    }

    /**
     * Gets the item containing the used enchantment.
     *
     * @return an ItemStack for the trident being used
     */
    [[nodiscard]] const ItemStack &getItem() const { return item_; }

private:
    ItemStack item_;
};

}  // namespace endstone
