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

#include "endstone/registry.h"

namespace endstone {

class BlockData;

/**
 * @brief Represents a block type.
 */
class BlockType;
using BlockTypeId = Identifier<BlockType>;
class BlockType : public Registry<BlockType>::Type {
public:
    static constexpr auto Air = BlockTypeId::minecraft("air");

    /**
     * @brief Returns true if this BlockType has a corresponding ItemType.
     *
     * @return true if there is a corresponding ItemType, otherwise false
     */
    [[nodiscard]] virtual bool hasItemType() const = 0;

    /**
     * @brief Creates a new BlockData instance for this block type, with all
     * properties initialized to unspecified defaults.
     *
     * @return new data instance
     */
    [[nodiscard]] virtual std::unique_ptr<BlockData> createBlockData() const = 0;
};
}  // namespace endstone
