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

#include "endstone/block/tile_state.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/rotation.h"

namespace endstone {

/**
 * Represents a captured state of an item frame.
 */
class ItemFrame : public TileState {
public:
    /**
     * Gets the item in this frame.
     *
     * @return a defensive copy of the item in this item frame, or `std::nullopt` if the frame is empty
     */
    [[nodiscard]] virtual std::optional<ItemStack> getItem() const = 0;

    /**
     * Sets the item in this frame.
     *
     * <p>
     * This resets the rotation of the frame, as placing an item in a frame does in-game.
     *
     * @param item the new item, or `std::nullopt` to empty the frame
     */
    virtual void setItem(const std::optional<ItemStack> &item) = 0;

    /**
     * Gets the chance of the item being dropped upon this frame's destruction.
     *
     * <ul>
     * <li>A drop chance of 0.0F will never drop
     * <li>A drop chance of 1.0F will always drop
     * </ul>
     *
     * @return chance of the item being dropped
     */
    [[nodiscard]] virtual float getItemDropChance() const = 0;

    /**
     * Sets the chance of the item being dropped upon this frame's destruction.
     *
     * <ul>
     * <li>A drop chance of 0.0F will never drop
     * <li>A drop chance of 1.0F will always drop
     * </ul>
     *
     * @param chance the chance of the item being dropped
     */
    virtual void setItemDropChance(float chance) = 0;

    /**
     * Gets the rotation of the frame's item.
     *
     * @return the rotation
     */
    [[nodiscard]] virtual Rotation getRotation() const = 0;

    /**
     * Sets the rotation of the frame's item.
     *
     * @param rotation the new rotation
     */
    virtual void setRotation(Rotation rotation) = 0;
};

}  // namespace endstone
