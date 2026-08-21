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

#include "endstone/block/block.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

/**
 * Called when an Actor changes a block as part of its own behaviour, such as a creeper exploding, an enderman
 * picking a block up, a ravager trampling crops or a zombie breaking a door.
 *
 * Unlike Bukkit's equivalent, this covers only the mob griefing paths. It is not called for falling blocks landing
 * or for sheep eating grass, and the resulting block state is not available. Blocks broken by a player are covered
 * by BlockBreakEvent.
 */
class ActorChangeBlockEvent : public Cancellable<ActorEvent<Actor>> {
public:
    ENDSTONE_EVENT(ActorChangeBlockEvent);

    explicit ActorChangeBlockEvent(const NotNull<Actor> &actor, const NotNull<Block> &block)
        : Cancellable(actor), block_(block)
    {
    }

    /**
     * Gets the block that will be destroyed or changed.
     *
     * @return the affected block
     */
    [[nodiscard]] const NotNull<Block> &getBlock() const { return block_; }

private:
    NotNull<Block> block_;
};

}  // namespace endstone
