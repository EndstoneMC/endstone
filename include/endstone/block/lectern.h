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

#include "endstone/block/block_actor_state.h"
#include "endstone/inventory/inventory.h"

namespace endstone {

/**
 * Represents a captured state of a lectern.
 */
class Lectern : public BlockActorState {
public:
    /**
     * Gets the page currently displayed on the lectern.
     *
     * <p>
     * The page is 0-indexed.
     *
     * @return the page
     */
    [[nodiscard]] virtual int getPage() const = 0;

    /**
     * Sets the page currently displayed on the lectern.
     *
     * <p>
     * The page is 0-indexed, and is clamped to the number of pages in the book the lectern holds.
     *
     * @param page the page
     */
    virtual void setPage(int page) = 0;

    /**
     * Gets the inventory of the block represented by this block state.
     *
     * If the block was changed to a different type in the meantime, the returned inventory might no longer be valid.
     *
     * @return the inventory of the block
     */
    [[nodiscard]] virtual Inventory &getInventory() const = 0;
};

}  // namespace endstone
