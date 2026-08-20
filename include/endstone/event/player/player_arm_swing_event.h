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

#include "endstone/event/player/player_event.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

/**
 * Called when a player swings their arm.
 */
class PlayerArmSwingEvent final : public PlayerEvent {
public:
    /**
     * An enum to specify what the player was doing when they swung their arm.
     */
    enum class SwingSource {
        /**
         * No particular source.
         */
        None,
        /**
         * Placing a block.
         */
        Build,
        /**
         * Mining a block.
         */
        Mine,
        /**
         * Interacting with a block or an actor.
         */
        Interact,
        /**
         * Attacking a target.
         */
        Attack,
        /**
         * Using the held item.
         */
        UseItem,
        /**
         * Throwing the held item.
         */
        ThrowItem,
        /**
         * Dropping an item.
         */
        DropItem,
        /**
         * Requested by the server, e.g. by a script or an add-on.
         */
        Event,
    };

    ENDSTONE_EVENT(PlayerArmSwingEvent);

    PlayerArmSwingEvent(const NotNull<Player> &player, std::optional<ItemStack> item, SwingSource swing_source)
        : PlayerEvent(player), item_(std::move(item)), swing_source_(swing_source)
    {
    }

    /**
     * Gets the item the player was holding when they swung their arm.
     *
     * @return the item in the player's hand, or std::nullopt if the hand was empty.
     */
    [[nodiscard]] const std::optional<ItemStack> &getItem() const { return item_; }

    /**
     * Gets what the player was doing when they swung their arm.
     *
     * @return the source of this swing.
     */
    [[nodiscard]] SwingSource getSwingSource() const { return swing_source_; }

private:
    std::optional<ItemStack> item_;
    SwingSource swing_source_;
};

}  // namespace endstone
