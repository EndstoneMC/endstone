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

namespace endstone {

/**
 * Represents a captured state of a campfire.
 */
class Campfire : public TileState {
public:
    /**
     * Gets the number of items this campfire can cook at once.
     *
     * @return the number of slots
     */
    [[nodiscard]] virtual int getSize() const = 0;

    /**
     * Gets the item currently cooking in the given slot.
     *
     * @param index the slot, between 0 and `getSize()` - 1
     * @return the item, or `std::nullopt` if the slot is empty
     */
    [[nodiscard]] virtual std::optional<ItemStack> getItem(int index) const = 0;

    /**
     * Sets the item currently cooking in the given slot.
     *
     * @param index the slot, between 0 and `getSize()` - 1
     * @param item the item, or `std::nullopt` to empty the slot
     */
    virtual void setItem(int index, const std::optional<ItemStack> &item) = 0;

    /**
     * Gets how long the item in the given slot has been cooking for.
     *
     * @param index the slot, between 0 and `getSize()` - 1
     * @return the cook time, in ticks
     */
    [[nodiscard]] virtual int getCookTime(int index) const = 0;

    /**
     * Sets how long the item in the given slot has been cooking for.
     *
     * @param index the slot, between 0 and `getSize()` - 1
     * @param cook_time the cook time, in ticks
     */
    virtual void setCookTime(int index, int cook_time) = 0;
};

}  // namespace endstone
