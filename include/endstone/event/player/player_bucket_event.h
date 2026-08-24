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
#include "endstone/inventory/equipment_slot.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/util/pointers.h"

namespace endstone {

class Block;
class ItemType;

/**
 * Provides common data for events involving a player's bucket interaction.
 */
class PlayerBucketEvent : public Cancellable<PlayerEvent> {
public:
    PlayerBucketEvent(const NotNull<Player> &player, const Nullable<Block> &block, const NotNull<Block> &block_clicked,
                      BlockFace block_face, const ItemType &bucket, std::optional<ItemStack> item_stack,
                      EquipmentSlot hand)
        : Cancellable(player), block_(block), block_clicked_(block_clicked), block_face_(block_face), bucket_(bucket),
          item_stack_(std::move(item_stack)), hand_(hand)
    {
    }
    ~PlayerBucketEvent() override = default;

    /**
     * Gets the block involved in this event.
     *
     * @return the block involved in this event, or a null handle if unavailable
     */
    [[nodiscard]] const Nullable<Block> &getBlock() const { return block_; }

    /**
     * Gets the block clicked by the player.
     *
     * @return the clicked block
     */
    [[nodiscard]] const NotNull<Block> &getBlockClicked() const { return block_clicked_; }

    /**
     * Gets the face on the clicked block.
     *
     * @return the clicked block face
     */
    [[nodiscard]] BlockFace getBlockFace() const { return block_face_; }

    /**
     * Gets the bucket used in this event.
     *
     * @return the bucket item type
     */
    [[nodiscard]] const ItemType &getBucket() const { return bucket_; }

    /**
     * Gets the hand used in this event.
     *
     * @note This is always EquipmentSlot::Hand, because Bedrock does not report which hand was used for this
     * interaction.
     *
     * @return the hand
     */
    [[nodiscard]] EquipmentSlot getHand() const { return hand_; }

    /**
     * Gets the resulting item in the player's hand.
     *
     * @return the resulting item stack, or std::nullopt if unavailable
     */
    [[nodiscard]] const std::optional<ItemStack> &getItemStack() const { return item_stack_; }

    /**
     * Sets the resulting item in the player's hand.
     *
     * @param item_stack the item stack to write back after the event, or std::nullopt
     */
    void setItemStack(std::optional<ItemStack> item_stack) { item_stack_ = std::move(item_stack); }

private:
    Nullable<Block> block_;
    NotNull<Block> block_clicked_;
    BlockFace block_face_;
    const ItemType &bucket_;
    std::optional<ItemStack> item_stack_;
    EquipmentSlot hand_;
};

}  // namespace endstone
