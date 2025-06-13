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

#include "endstone/block/block.h"
#include "endstone/level/chunk.h"
#include "endstone/util/result.h"

namespace endstone {

/**
 * @brief Represents a dimension within a Level.
 */
class Dimension {
public:
    /**
     * @brief Represents various dimension types.
     */
    enum class Type {
        Overworld = 0,
        Nether = 1,
        TheEnd = 2,
        Custom = 999
    };

    virtual ~Dimension() = default;

    /**
     * @brief Gets the name of this dimension
     *
     * @return Name of this dimension
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * @brief Gets the type of this dimension
     *
     * @return Type of this dimension
     */
    [[nodiscard]] virtual Type getType() const = 0;

    /**
     * @brief Gets the level to which this dimension belongs
     *
     * @return Level containing this dimension.
     */
    [[nodiscard]] virtual Level &getLevel() const = 0;

    /**
     * @brief Gets the Block at the given coordinates
     *
     * @param x X-coordinate of the block
     * @param y Y-coordinate of the block
     * @param z Z-coordinate of the block
     * @return Block at the given coordinates
     */
    [[nodiscard]] virtual std::unique_ptr<Block> getBlockAt(int x, int y, int z) const = 0;

    /**
     * @brief Gets the Block at the given Location.
     *
     * @param location Location of the block
     * @return Block at the given coordinates
     */
    [[nodiscard]] virtual std::unique_ptr<Block> getBlockAt(Location location) const = 0;

    /**
     * @brief Gets the highest non-empty (impassable) coordinate at the given coordinates.
     *
     * @param x X-coordinate of the blocks
     * @param z Z-coordinate of the blocks
     * @return Y-coordinate of the highest non-empty block
     */
    [[nodiscard]] virtual int getHighestBlockYAt(int x, int z) const = 0;

    /**
     * @brief Gets the highest non-empty (impassable) block at the given coordinates.
     *
     * @param x X-coordinate of the block
     * @param z Z-coordinate of the block
     * @return Highest non-empty block
     */
    [[nodiscard]] virtual std::unique_ptr<Block> getHighestBlockAt(int x, int z) const = 0;

    /**
     * @brief Gets the highest non-empty (impassable) block at the given Location.
     *
     * @param location Coordinates to get the highest block
     * @return Highest non-empty block
     */
    [[nodiscard]] virtual std::unique_ptr<Block> getHighestBlockAt(Location location) const = 0;

    /**
     * @brief Gets a list of all loaded Chunks
     *
     * @return All loaded chunks
     */
    [[nodiscard]] virtual std::vector<std::unique_ptr<Chunk>> getLoadedChunks() = 0;
};
}  // namespace endstone

template <>
struct fmt::formatter<endstone::Dimension> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const endstone::Dimension &self, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "Dimension(name={})", self.getName());
    }
};
