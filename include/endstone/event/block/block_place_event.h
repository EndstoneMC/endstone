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

#include "endstone/event/block/block_event.h"
#include "endstone/player.h"

namespace endstone {

/**
 * @brief Called when a block is placed by a player.
 * <p>
 * If a BlockPlaceEvent is cancelled, the block will not be placed.
 */
class BlockPlaceEvent : public BlockEvent {
public:
    explicit BlockPlaceEvent(Block &replaced_block, Block &placed_against, Player &player)
        : BlockEvent(replaced_block), placed_against_(placed_against), player_(player)
    {
    }
    ~BlockPlaceEvent() override = default;

    inline static const std::string NAME = "BlockPlaceEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    /**
     * @brief Gets the player who placed the block involved in this event.
     *
     * @return The Player who placed the block involved in this event
     */
    [[nodiscard]] Player &getPlayer() const
    {
        return player_;
    }

    /**
     * @brief Gets the block which was replaced.
     *
     * @return The Block which was replaced.
     */
    [[nodiscard]] Block &getBlockReplaced() const
    {
        return getBlock();
    }

    /**
     * @brief Gets the block that this block was placed against
     *
     * @return Block the block that the new block was placed against
     */
    [[nodiscard]] Block &getBlockAgainst() const
    {
        return placed_against_;
    }

private:
    Block &placed_against_;
    Player &player_;
    // TODO(event): add ItemStack item
    // TODO(event): add BlockState placedBlockState
};

}  // namespace endstone
