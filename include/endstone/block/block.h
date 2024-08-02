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

#include "endstone/block/block_data.h"

namespace endstone {

/**
 * @brief Represents a block.
 *
 * <p>
 * This is a live object, and only one Block may exist for any given location in a world.
 * The state of the block may change concurrently to your own handling of it.
 * Use block.getState() to get a snapshot state of a block which will not be modified.
 */
class Block {
public:
    virtual ~Block() = default;

    /**
     * @brief Gets the complete block data for this block
     *
     * @return block specific data
     */
    virtual std::unique_ptr<BlockData> getData();
};

}  // namespace endstone
