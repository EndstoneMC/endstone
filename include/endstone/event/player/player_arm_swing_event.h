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

#include <optional>
#include <utility>

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

/**
 * Called when a player swings their arm.
 *
 * Cancelling stops the server acting on the swing at all. The swing is neither recorded nor shown to the other
 * players in the dimension.
 *
 * @note The swinging player still sees their own arm move, because their client plays the animation without waiting
 * for the server. The event covers swings the player starts. Swings the server drives itself, such as dropping an
 * item, do not fire it.
 */
class PlayerArmSwingEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerArmSwingEvent);

    PlayerArmSwingEvent(const NotNull<Player> &player, std::optional<ItemStack> item)
        : Cancellable(player), item_(std::move(item))
    {
    }

    /**
     * Gets the item the player was holding when they swung their arm.
     *
     * @return the item in the player's hand, or std::nullopt if the hand was empty.
     */
    [[nodiscard]] const std::optional<ItemStack> &getItem() const { return item_; }

private:
    std::optional<ItemStack> item_;
};

}  // namespace endstone
