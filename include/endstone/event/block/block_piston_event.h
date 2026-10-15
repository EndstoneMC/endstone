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
#include <vector>

#include "endstone/block/block_face.h"
#include "endstone/event/block/block_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

/**
 * Called when a piston block is triggered.
 */
class BlockPistonEvent : public Cancellable<BlockEvent> {
public:
    using BlockList = std::vector<NotNull<Block>>;

    explicit BlockPistonEvent(const NotNull<Block> &block, BlockFace direction, BlockList blocks = {})
        : Cancellable(block), direction_(direction), blocks_(std::move(blocks))
    {
    }

    /**
     * Return the direction in which the piston will operate.
     *
     * @return direction of the piston
     */
    [[nodiscard]] BlockFace getDirection() const { return direction_; }

    /**
     * Gets the blocks that will be moved or broken by the piston, at their original positions.
     *
     * @return The affected blocks, excluding the piston and its arm.
     */
    [[nodiscard]] const BlockList &getBlocks() const { return blocks_; }

private:
    BlockFace direction_;
    BlockList blocks_;
};

}  // namespace endstone
