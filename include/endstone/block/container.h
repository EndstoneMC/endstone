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
#include <string>

#include "endstone/block/block_actor_state.h"
#include "endstone/inventory/inventory.h"

namespace endstone {

/**
 * Represents a captured state of a container block, such as a chest.
 */
class Container : public BlockActorState {
public:
    /**
     * Gets the inventory of the block represented by this block state.
     *
     * <p>
     * If the block was changed to a different type in the meantime, the returned inventory might no longer be valid.
     *
     * <p>
     * Modifications to the returned inventory apply to the block at once, without a call to update().
     *
     * @return the inventory of the block
     */
    [[nodiscard]] virtual Inventory &getInventory() const = 0;

    /**
     * Gets the captured inventory snapshot of this container.
     *
     * <p>
     * The returned inventory is not linked to the block. Modifications to it do not apply to the block until update()
     * is called.
     *
     * @return the captured inventory snapshot
     */
    [[nodiscard]] virtual Inventory &getSnapshotInventory() const = 0;

    /**
     * Gets the custom name of this container.
     *
     * @return the custom name, or `std::nullopt` if the container has none
     */
    [[nodiscard]] virtual std::optional<std::string> getCustomName() const = 0;

    /**
     * Sets the custom name of this container.
     *
     * <p>
     * An empty name clears the custom name.
     *
     * @param name the new custom name, or `std::nullopt` to clear it
     */
    virtual void setCustomName(std::optional<std::string> name) = 0;
};

}  // namespace endstone
