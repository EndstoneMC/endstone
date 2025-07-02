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

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

/**
 * @brief Represents an event that is called when a player right-clicks a block.
 */
class PlayerInteractEvent : public Cancellable<PlayerEvent> {
public:
    enum class Action {
        /**
         * Left-clicking a block
         */
        LeftClickBlock,
        /**
         * Right-clicking a block
         */
        RightClickBlock,
        /**
         * Left-clicking the air
         */
        LeftClickAir,
        /**
         * Right-clicking the air
         */
        RightClickAir,
    };

    ENDSTONE_EVENT(PlayerInteractEvent);

    PlayerInteractEvent(Player &player, Action action, ItemStack *item, Block *block_clicked, BlockFace block_face,
                        const std::optional<Vector<float>> &clicked_position)
        : Cancellable(player), action_(action), item_(item), block_clicked_(block_clicked), block_face_(block_face),
          clicked_position_(clicked_position)
    {
    }

    /**
     * @brief Returns the action type
     *
     * @return Action returns the type of interaction
     */
    [[nodiscard]] Action getAction() const
    {
        return action_;
    }

    /**
     * @brief Check if this event involved an item
     *
     * @return boolean true if it did
     */
    [[nodiscard]] bool hasItem() const
    {
        return item_ != nullptr;
    }

    /**
     * @brief Returns the item in hand represented by this event
     *
     * @return ItemStack the item used
     */
    [[nodiscard]] ItemStack *getItem() const
    {
        return item_;
    }

    /**
     * @brief Check if this event involved a block
     *
     * @return boolean true if it did
     */
    [[nodiscard]] bool hasBlock() const
    {
        return block_clicked_ != nullptr;
    }

    /**
     * @brief Returns the clicked block
     *
     * @return Block returns the block clicked with this item.
     */
    [[nodiscard]] Block *getBlock() const
    {
        return block_clicked_;
    }

    /**
     * @brief Returns the face of the block that was clicked
     *
     * @return BlockFace returns the face of the block that was clicked
     */
    [[nodiscard]] BlockFace getBlockFace() const
    {
        return block_face_;
    }

    /**
     * @brief Gets the exact position on the block the player interacted with.
     *
     * @note This will be std::nullopt outside of Action.RightClickBlock
     * @note All vector components are between 0.0 and 1.0 inclusive.
     *
     * @return the clicked position.
     */
    [[nodiscard]] std::optional<Vector<float>> getClickedPosition() const
    {
        return clicked_position_;
    }

private:
    ItemStack *item_;
    Action action_;
    Block *block_clicked_;
    BlockFace block_face_;
    std::optional<Vector<float>> clicked_position_;
};

}  // namespace endstone
