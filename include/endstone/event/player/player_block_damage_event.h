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

#include "endstone/block/block_face.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/util/vector.h"

namespace endstone {

/**
 * Called when a player starts, continues, aborts, predicts, stops, or creatively destroys a block.
 *
 * Cancellation is honored for Start and Continue actions only.
 */
class PlayerBlockDamageEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerBlockDamageEvent)

    /**
     * Represents the block damage action that triggered this event.
     */
    enum class Action {
        /**
         * The player started damaging a block.
         */
        Start,
        /**
         * The player aborted block damage.
         */
        Abort,
        /**
         * The player stopped damaging a block.
         */
        Stop,
        /**
         * The player continued damaging a block.
         */
        Continue,
        /**
         * The player predicted block damage.
         */
        Predict,
        /**
         * The player creatively destroyed a block.
         */
        Creative,
    };

    PlayerBlockDamageEvent(Player &player, Action action, std::optional<ItemStack> item, Block *block,
                           std::optional<BlockFace> block_face, Vector position)
        : Cancellable(player), action_(action), item_(std::move(item)), block_(block), block_face_(block_face),
          position_(std::move(position))
    {
    }

    /**
     * Gets the block damage action that triggered this event.
     *
     * @return the block damage action
     */
    [[nodiscard]] Action getAction() const { return action_; }

    /**
     * Gets the item used to damage the block.
     *
     * @return the item used to damage the block, or std::nullopt if unavailable
     */
    [[nodiscard]] const std::optional<ItemStack> &getItem() const { return item_; }

    /**
     * Gets the block being damaged.
     *
     * @return the block being damaged, or nullptr if unavailable
     */
    [[nodiscard]] Block *getBlock() const { return block_; }

    /**
     * Gets the face being damaged.
     *
     * @return the face being damaged, or std::nullopt if unavailable
     */
    [[nodiscard]] std::optional<BlockFace> getBlockFace() const { return block_face_; }

    /**
     * Gets the block position.
     *
     * @return the block position
     */
    [[nodiscard]] Vector getPosition() const { return position_; }

private:
    Action action_;
    std::optional<ItemStack> item_;
    Block *block_;
    std::optional<BlockFace> block_face_;
    Vector position_;
};

}  // namespace endstone
