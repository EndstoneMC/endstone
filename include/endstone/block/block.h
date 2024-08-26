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
#include <string>

#include "endstone/block/block_face.h"
#include "endstone/level/location.h"

namespace endstone {

class Dimension;

/**
 * @brief Represents a block.
 *
 * <p>
 * This is a live object, and only one Block may exist for any given location in a dimension.
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
    [[nodiscard]] virtual std::string getType() const = 0;

    /**
     * @brief Gets the block at the given offsets
     *
     * @param offset_x X-coordinate offset
     * @param offset_y Y-coordinate offset
     * @param offset_z Z-coordinate offset
     * @return Block at the given offsets
     */
    virtual std::unique_ptr<Block> getRelative(int offset_x, int offset_y, int offset_z) = 0;

    /**
     * @brief Gets the block at the given face
     * <p>
     * This method is equal to getRelative(face, 1)
     *
     * @param face Face of this block to return
     * @return Block at the given face
     */
    virtual std::unique_ptr<Block> getRelative(BlockFace face) = 0;

    /**
     * @brief Gets the block at the given distance of the given face
     *
     * @param face Face of this block to return
     * @param distance Distance to get the block at
     * @return Block at the given face
     */
    virtual std::unique_ptr<Block> getRelative(BlockFace face, int distance) = 0;

    /**
     * @brief Gets the dimension which contains this Block
     *
     * @return Dimension containing this block
     */
    [[nodiscard]] virtual Dimension &getDimension() const = 0;

    /**
     * @brief Gets the x-coordinate of this block
     *
     * @return x-coordinate
     */
    [[nodiscard]] virtual int getX() const = 0;

    /**
     * @brief Gets the y-coordinate of this block
     *
     * @return x-coordinate
     */
    [[nodiscard]] virtual int getY() const = 0;

    /**
     * @brief Gets the z-coordinate of this block
     *
     * @return x-coordinate
     */
    [[nodiscard]] virtual int getZ() const = 0;

    /**
     * @brief Gets the Location of the block
     *
     * @return Location of block
     */
    [[nodiscard]] virtual Location getLocation() const = 0;
};

}  // namespace endstone
