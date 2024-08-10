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

#include <string>

namespace endstone {

/**
 * @brief Represents a block.
 *
 * <p>
 * This is a live object, and only one Block may exist for any given location in a world.
 */
class Block {
public:
    virtual ~Block() = default;

    /**
     * @brief Get the type of the block.
     *
     * This method returns the type of the block as a string, for example, minecraft:acacia_stairs.
     *
     * @return The type of the block.
     */
    // [[nodiscard]] virtual std::string getType() const = 0;
};

}  // namespace endstone
