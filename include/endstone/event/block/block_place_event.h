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
#include <string>
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
    explicit BlockPlaceEvent(std::unique_ptr<BlockState> placed_block, std::unique_ptr<Block> replaced_block,
                             std::unique_ptr<Block> placed_against, Player &player)
        : Cancellable(std::move(replaced_block)), placed_block_(std::move(placed_block)),
          placed_against_(std::move(placed_against)), player_(player)
    {
    }
    ~BlockPlaceEvent() override = default;

    inline static const std::string NAME = "BlockPlaceEvent";
    [[nodiscard]] std::string getEventName() const override { return NAME; }

    /**
     * @brief Gets the player who placed the block involved in this event.
     *
     * @return The Player who placed the block involved in this event
     */
    [[nodiscard]] Player &getPlayer() const { return player_; }

    /**
     * @brief Gets the BlockState for the block which was placed.
     *
     * @return The BlockState for the block which was placed.
     */
    [[nodiscard]] BlockState &getBlockPlacedState() const { return *placed_block_; }

    /**
     * @brief Gets the block which was replaced.
     *
     * @return The Block which was replaced.
     */
    [[nodiscard]] Block &getBlockReplaced() const { return getBlock(); }

    /**
     * @brief Gets the block that this block was placed against
     *
     * @return Block the block that the new block was placed against
     */
    [[nodiscard]] Block &getBlockAgainst() const { return *placed_against_; }

private:
    std::unique_ptr<BlockState> placed_block_;
    std::unique_ptr<Block> placed_against_;
    Player &player_;
    // TODO(event): add ItemStack item
    // TODO(event): add BlockState placedBlockState
};

}  // namespace endstone
