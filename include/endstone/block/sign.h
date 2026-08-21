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

#include "endstone/block/block_state.h"
#include "endstone/block/sign/sign_side.h"

namespace endstone {

/**
 * Represents a captured state of a sign.
 */
class Sign : public BlockState {
public:
    /**
     * Represents a side of a sign.
     */
    enum class Side {
        /**
         * The front side of the sign.
         */
        Front = 0,

        /**
         * The back side of the sign.
         */
        Back = 1,
    };

    /**
     * Gets the side of this sign.
     *
     * @param side the side of the sign
     * @return the given side of the sign
     */
    [[nodiscard]] virtual SignSide &getSide(Side side) const = 0;

    /**
     * Gets whether this sign is waxed.
     *
     * <p>
     * A waxed sign cannot be edited by players.
     *
     * @return `true` if this sign is waxed
     */
    [[nodiscard]] virtual bool isWaxed() const = 0;

    /**
     * Sets whether this sign is waxed.
     *
     * @param waxed whether this sign is waxed
     */
    virtual void setWaxed(bool waxed) = 0;
};

}  // namespace endstone
