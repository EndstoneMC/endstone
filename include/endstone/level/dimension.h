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

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "endstone/actor/actor.h"
#include "endstone/actor/item.h"
#include "endstone/block/block.h"
#include "endstone/identifier.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/level/chunk.h"

namespace endstone {

class Dimension;
using DimensionId = Identifier<Dimension>;

/**
 * Represents a dimension within a Level.
 */
class Dimension : public std::enable_shared_from_this<Dimension> {
public:
    static constexpr auto Overworld = DimensionId::minecraft("overworld");
    static constexpr auto Nether = DimensionId::minecraft("nether");
    static constexpr auto TheEnd = DimensionId::minecraft("the_end");

    virtual ~Dimension() = default;

    /**
     * Return the identifier of this dimension.
     *
     * @return this dimension's identifier
     */
    [[nodiscard]] virtual DimensionId getId() const = 0;

    /**
     * Get the translation key, suitable for use in a translation component.
     *
     * @return the translation key
     */
    [[nodiscard]] virtual std::string getTranslationKey() const = 0;

    /**
     * Gets the level to which this dimension belongs.
     *
     * @return Level containing this dimension.
     */
    [[nodiscard]] virtual Level &getLevel() const = 0;

    /**
     * Checks whether this dimension is still valid (loaded).
     *
     * A Dimension handle can outlive the underlying dimension if it is unloaded; this returns false once that
     * happens, after which accessing the dimension throws.
     *
     * @return `true` if the dimension is still loaded, `false` otherwise
     */
    [[nodiscard]] virtual bool isValid() const = 0;

    /**
     * Gets the Block at the given coordinates.
     *
     * @param x X-coordinate of the block
     * @param y Y-coordinate of the block
     * @param z Z-coordinate of the block
     * @return Block at the given coordinates
     */
    [[nodiscard]] virtual std::unique_ptr<Block> getBlockAt(int x, int y, int z) const = 0;

    /**
     * Gets the Block at the given Location.
     *
     * @param location Location of the block
     * @return Block at the given coordinates
     */
    [[nodiscard]] virtual std::unique_ptr<Block> getBlockAt(Location location) const = 0;

    /**
     * Gets the highest non-empty (impassable) coordinate at the given coordinates.
     *
     * @param x X-coordinate of the blocks
     * @param z Z-coordinate of the blocks
     * @return Y-coordinate of the highest non-empty block
     */
    [[nodiscard]] virtual int getHighestBlockYAt(int x, int z) const = 0;

    /**
     * Gets the highest non-empty (impassable) block at the given coordinates.
     *
     * @param x X-coordinate of the block
     * @param z Z-coordinate of the block
     * @return Highest non-empty block
     */
    [[nodiscard]] virtual std::unique_ptr<Block> getHighestBlockAt(int x, int z) const = 0;

    /**
     * Gets the highest non-empty (impassable) block at the given Location.
     *
     * @param location Coordinates to get the highest block
     * @return Highest non-empty block
     */
    [[nodiscard]] virtual std::unique_ptr<Block> getHighestBlockAt(Location location) const = 0;

    /**
     * Gets a list of all loaded Chunks.
     *
     * @return All loaded chunks
     */
    [[nodiscard]] virtual std::vector<std::unique_ptr<Chunk>> getLoadedChunks() = 0;

    /**
     * Drops an item at the specified Location.
     *
     * @param location Location to drop the item
     * @param item ItemStack to drop
     *
     * @return Item entity created as a result of this method
     */
    [[nodiscard]] virtual Item &dropItem(Location location, const ItemStack &item) = 0;

    /**
     * Creates an actor at the given Location.
     *
     * @param location The location to spawn the actor
     * @param type The actor type to spawn
     * @return Resulting Actor of this method
     */
    [[nodiscard]] virtual Actor *spawnActor(Location location, ActorTypeId type) = 0;

    /**
     * Get a list of all actors in this dimension.
     *
     * @return A List of all actors currently residing in this dimension
     */
    [[nodiscard]] virtual std::vector<Actor *> getActors() const = 0;
};

inline Nullable<Dimension> Location::getDimension() const
{
    auto dimension = dimension_.lock();
    if (!dimension) {
        // The wrapper is gone: tell "never set" (empty weak_ptr) apart from a wrapper that has expired.
        const std::weak_ptr<Dimension> unset;
        const bool was_set = dimension_.owner_before(unset) || unset.owner_before(dimension_);
        Preconditions::checkArgument(!was_set, "Dimension unloaded");
        return nullptr;
    }
    // The wrapper is alive, but the underlying dimension it points to may have been unloaded.
    Preconditions::checkArgument(dimension->isValid(), "Dimension unloaded");
    return dimension;
}

inline bool Location::isDimensionLoaded() const
{
    const auto dimension = dimension_.lock();
    return dimension && dimension->isValid();
}

inline std::unique_ptr<Block> Location::getBlock() const
{
    return getDimension().value().getBlockAt(*this);
}

inline float Location::distanceSquared(const Location &other) const
{
    const auto dimension = getDimension();
    const auto other_dimension = other.getDimension();
    Preconditions::checkArgument(dimension != nullptr && other_dimension != nullptr,
                                 "Cannot measure distance to a null dimension.");
    Preconditions::checkArgument(dimension == other_dimension, "Cannot measure distance between {} and {}.",
                                 dimension.value().getId(), other_dimension.value().getId());
    return ((x_ - other.x_) * (x_ - other.x_)) + ((y_ - other.y_) * (y_ - other.y_)) +
           ((z_ - other.z_) * (z_ - other.z_));
}
}  // namespace endstone

template <>
struct std::formatter<endstone::Dimension> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::Dimension &self, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "Dimension(id={})", self.getId());
    }
};
