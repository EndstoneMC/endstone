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

#include "endstone/event/block/block_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

/**
 * @brief Called when a block is broken by a player.
 *
 * <p>
 * If a BlockBreakEvent is cancelled, the block will not break and experience will not drop.
 */
class BlockBreakEvent : public Cancellable<BlockEvent> {
public:
    ENDSTONE_EVENT(BlockBreakEvent);
    explicit BlockBreakEvent(std::unique_ptr<Block> block, Player &player)
        : Cancellable(std::move(block)), player_(player)
    {
    }
    ~BlockBreakEvent() override = default;

    /**
     * @brief Gets the Player that is breaking the block involved in this event.
     *
     * @return The Player that is breaking the block involved in this event
     */
    [[nodiscard]] Player &getPlayer() const { return player_; }

private:
    Player &player_;
};

}  // namespace endstone
