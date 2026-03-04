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
 * @brief Called when a block is placed by a player.
 * <p>
 * If a BlockPlaceEvent is cancelled, the block will not be placed.
 */
class BlockPlaceEvent : public Cancellable<BlockEvent> {
public:
    ENDSTONE_EVENT(BlockPlaceEvent);
    explicit BlockPlaceEvent(std::unique_ptr<Block> placed_block, std::unique_ptr<BlockState> replaced_state,
                             std::unique_ptr<Block> placed_against, Player &player)
        : Cancellable(std::move(placed_block)), replaced_state_(std::move(replaced_state)),
          placed_against_(std::move(placed_against)), player_(player)
    {
    }
    ~BlockPlaceEvent() override = default;

    /**
     * @brief Gets the player who placed the block involved in this event.
     *
     * @return The Player who placed the block involved in this event
     */
    [[nodiscard]] Player &getPlayer() const { return player_; }

    /**
     * @brief Gets the block placed.
     *
     * @return The Block that was placed.
     */
    [[nodiscard]] Block &getBlockPlaced() const { return getBlock(); }

    /**
     * @brief Gets the BlockState for the block which was replaced.
     *
     * @return The BlockState of the block that was replaced.
     */
    [[nodiscard]] BlockState &getBlockReplacedState() const { return *replaced_state_; }

    /**
     * @brief Gets the block that this block was placed against
     *
     * @return Block the block that the new block was placed against
     */
    [[nodiscard]] Block &getBlockAgainst() const { return *placed_against_; }

private:
    std::unique_ptr<BlockState> replaced_state_;
    std::unique_ptr<Block> placed_against_;
    Player &player_;
};

}  // namespace endstone
