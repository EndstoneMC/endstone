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

#include <memory>
#include <utility>

#include "endstone/block/block_state.h"
#include "endstone/event/block/block_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/player.h"

namespace endstone {

/**
 * Called when a block is placed by a player.
 * <p>
 * If a BlockPlaceEvent is cancelled, the block will not be placed.
 */
class BlockPlaceEvent : public Cancellable<BlockEvent> {
public:
    ENDSTONE_EVENT(BlockPlaceEvent);
    explicit BlockPlaceEvent(const NotNull<Block> &placed_block, const NotNull<BlockState> &replaced_state,
                             const NotNull<Block> &placed_against, const NotNull<Player> &player)
        : Cancellable(placed_block), replaced_state_(replaced_state), placed_against_(placed_against), player_(player)
    {
    }
    ~BlockPlaceEvent() override = default;

    /**
     * Gets the player who placed the block involved in this event.
     *
     * @return The Player who placed the block involved in this event
     */
    [[nodiscard]] const NotNull<Player> &getPlayer() const { return player_; }

    /**
     * Gets the block placed.
     *
     * @return The Block that was placed.
     */
    [[nodiscard]] const NotNull<Block> &getBlockPlaced() const { return getBlock(); }

    /**
     * Gets the BlockState for the block which was replaced.
     *
     * @return The BlockState of the block that was replaced.
     */
    [[nodiscard]] const NotNull<BlockState> &getBlockReplacedState() const { return replaced_state_; }

    /**
     * Gets the block that this block was placed against.
     *
     * @return Block the block that the new block was placed against
     */
    [[nodiscard]] const NotNull<Block> &getBlockAgainst() const { return placed_against_; }

private:
    NotNull<BlockState> replaced_state_;
    NotNull<Block> placed_against_;
    NotNull<Player> player_;
};

}  // namespace endstone
