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
 * Called when a block grows naturally in the world.
 *
 * If a Block Grow event is cancelled, the block will not grow.
 */
class BlockGrowEvent : public Cancellable<BlockEvent> {
public:
    ENDSTONE_EVENT(BlockGrowEvent);
    explicit BlockGrowEvent(const NotNull<Block> &block, const NotNull<BlockState> &new_state)
        : Cancellable(block), new_state_(new_state)
    {
    }

    /**
     * Gets the state of the block after it has grown.
     *
     * @return The new state of the block.
     */
    [[nodiscard]] const NotNull<BlockState> &getNewState() const { return new_state_; }

private:
    NotNull<BlockState> new_state_;
};

}  // namespace endstone
