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
 * @brief Represents events with a source block and a destination block, currently only applies to
 * liquid (lava and water) and teleporting dragon eggs.
 *
 * If a Block From To event is cancelled, the block will not move (the liquid will not flow).
 */
class BlockFromToEvent : public Cancellable<BlockEvent> {
public:
    ENDSTONE_EVENT(BlockFromToEvent);
    explicit BlockFromToEvent(std::unique_ptr<Block> block, std::unique_ptr<Block> to_block)
        : Cancellable(std::move(block)), to_(std::move(to_block))
    {
    }

    /**
     * @brief Convenience method for getting the faced Block.
     *
     * @return The faced Block
     */
    [[nodiscard]] Block &getToBlock() const { return *to_; }

protected:
    std::unique_ptr<Block> to_;
};

}  // namespace endstone
