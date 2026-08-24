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
#include <unordered_map>
#include <vector>

#include "endstone/actor/actor.h"
#include "endstone/actor/item.h"
#include "endstone/block/block.h"
#include "endstone/identifier.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/level/chunk.h"

namespace endstone {

class Mob;
class Player;
class Plugin;

class Dimension;
using DimensionId = Identifier<Dimension>;

/**
 * Represents a dimension within a Level.
 */
class Dimension {
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
    [[nodiscard]] virtual NotNull<Block> getBlockAt(int x, int y, int z) const = 0;

    /**
     * Gets the Block at the given Location.
     *
     * @param location Location of the block
     * @return Block at the given coordinates
     */
    [[nodiscard]] virtual NotNull<Block> getBlockAt(Location location) const = 0;

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
    [[nodiscard]] virtual NotNull<Block> getHighestBlockAt(int x, int z) const = 0;

    /**
     * Gets the highest non-empty (impassable) block at the given Location.
     *
     * @param location Coordinates to get the highest block
     * @return Highest non-empty block
     */
    [[nodiscard]] virtual NotNull<Block> getHighestBlockAt(Location location) const = 0;

    /**
     * Gets a list of all loaded Chunks.
     *
     * @return All loaded chunks
     */
    [[nodiscard]] virtual std::vector<NotNull<Chunk>> getLoadedChunks() = 0;

    /**
     * Checks if the Chunk at the given coordinates is loaded.
     *
     * @param x X-coordinate of the chunk
     * @param z Z-coordinate of the chunk
     * @return `true` if the chunk is loaded, otherwise `false`
     */
    [[nodiscard]] virtual bool isChunkLoaded(int x, int z) const = 0;

    /**
     * Checks if the Chunk at the given coordinates has been generated.
     *
     * A chunk counts as generated once it is loaded or has been written to the level's chunk storage.
     *
     * @param x X-coordinate of the chunk
     * @param z Z-coordinate of the chunk
     * @return `true` if the chunk has been generated, otherwise `false`
     */
    [[nodiscard]] virtual bool isChunkGenerated(int x, int z) const = 0;

    /**
     * Requests the Chunk at the given coordinates to be loaded, generating it if it does not exist yet.
     *
     * Equivalent to `loadChunk(x, z, true)`.
     *
     * @param x X-coordinate of the chunk
     * @param z Z-coordinate of the chunk
     * @return `true` if the chunk is now held resident, otherwise `false`
     */
    virtual bool loadChunk(int x, int z) = 0;

    /**
     * Requests the Chunk at the given coordinates to be loaded, and keeps it resident until it is released again.
     *
     * The chunk is held from the moment this returns until `unloadChunk()` or `unloadChunkRequest()` releases it, or
     * the server restarts. Bedrock has no synchronous chunk load, so unless the chunk was already resident the load
     * finishes on a later tick and `isChunkLoaded()` may still report `false` right afterwards. A chunk held this way
     * stays in memory but is not ticked, and the hold never expires on its own. Intended for keeping a handful of
     * chunks resident; it is not suited to loading large regions.
     *
     * The hold is not attributed to any plugin and survives that plugin being disabled. Use
     * `addPluginChunkTicket()` for a hold that is released automatically.
     *
     * @param x X-coordinate of the chunk
     * @param z Z-coordinate of the chunk
     * @param generate Whether to generate the chunk if it does not exist yet
     * @return `false` if `generate` is `false` and the chunk has not been generated, or if the coordinates lie outside
     *         the world limit, otherwise `true`
     */
    virtual bool loadChunk(int x, int z, bool generate) = 0;

    /**
     * Releases the hold that `loadChunk()` placed on the Chunk at the given coordinates, and unloads it if nothing else
     * keeps it resident.
     *
     * A chunk kept alive by a nearby player, the spawn area, a `/tickingarea` or a plugin chunk ticket stays loaded,
     * and this reports `false`. Unloading a chunk saves it and fires a ChunkUnloadEvent, which handlers observe before
     * this returns.
     *
     * @note This also completes any chunk unloads the dimension had pending, so calling it once per chunk over a large
     *       area is expensive. Use `unloadChunkRequest()` when releasing many chunks at once.
     *
     * @param x X-coordinate of the chunk
     * @param z Z-coordinate of the chunk
     * @return `true` if the chunk is no longer loaded, otherwise `false`
     */
    virtual bool unloadChunk(int x, int z) = 0;

    /**
     * Releases the hold that `loadChunk()` placed on the Chunk at the given coordinates, without unloading it now.
     *
     * The chunk is unloaded on a later tick once nothing else keeps it resident.
     *
     * @param x X-coordinate of the chunk
     * @param z Z-coordinate of the chunk
     * @return `true`
     */
    virtual bool unloadChunkRequest(int x, int z) = 0;

    /**
     * Adds a plugin ticket for the Chunk at the given coordinates, loading it if it is not already loaded.
     *
     * A plugin ticket keeps the chunk resident until it is explicitly removed or the owning plugin is disabled. A
     * plugin may only have one ticket per chunk, but each chunk can have multiple plugin tickets. `unloadChunk()` does
     * not remove plugin tickets.
     *
     * @param x X-coordinate of the chunk
     * @param z Z-coordinate of the chunk
     * @param plugin Plugin taking the ticket
     * @return `true` if a plugin ticket was added, `false` if the plugin already holds one for this chunk
     */
    virtual bool addPluginChunkTicket(int x, int z, Plugin &plugin) = 0;

    /**
     * Removes the given plugin's ticket for the Chunk at the given coordinates.
     *
     * @param x X-coordinate of the chunk
     * @param z Z-coordinate of the chunk
     * @param plugin Plugin whose ticket to remove
     * @return `true` if a plugin ticket was removed, `false` if the plugin holds none for this chunk
     */
    virtual bool removePluginChunkTicket(int x, int z, Plugin &plugin) = 0;

    /**
     * Removes every ticket the given plugin holds in this dimension.
     *
     * @param plugin Plugin whose tickets to remove
     */
    virtual void removePluginChunkTickets(Plugin &plugin) = 0;

    /**
     * Gets which plugins hold a ticket for the Chunk at the given coordinates.
     *
     * The returned list is a snapshot; it does not track tickets added or removed afterwards.
     *
     * @param x X-coordinate of the chunk
     * @param z Z-coordinate of the chunk
     * @return Plugins holding a ticket for the chunk
     */
    [[nodiscard]] virtual std::vector<Plugin *> getPluginChunkTickets(int x, int z) const = 0;

    /**
     * Gets which plugins hold tickets for which Chunks in this dimension.
     *
     * The returned map is a snapshot; it does not track tickets added or removed afterwards. A plugin holding no
     * tickets is absent from it.
     *
     * @return Chunks each plugin holds a ticket for
     */
    [[nodiscard]] virtual std::unordered_map<Plugin *, std::vector<NotNull<Chunk>>> getPluginChunkTickets() const = 0;

    /**
     * Drops an item at the specified Location.
     *
     * @param location Location to drop the item
     * @param item ItemStack to drop
     *
     * @return Item entity created as a result of this method
     */
    [[nodiscard]] virtual NotNull<Item> dropItem(Location location, const ItemStack &item) = 0;

    /**
     * Creates an actor at the given Location.
     *
     * @param location The location to spawn the actor
     * @param type The actor type to spawn
     * @return Resulting Actor of this method
     */
    [[nodiscard]] virtual Nullable<Actor> spawnActor(Location location, ActorTypeId type) = 0;

    /**
     * Get a list of all actors in this dimension.
     *
     * @return A List of all actors currently residing in this dimension
     */
    [[nodiscard]] virtual std::vector<NotNull<Actor>> getActors() const = 0;

    /**
     * Get a list of all mobs in this dimension.
     *
     * @return A List of all mobs currently residing in this dimension
     */
    [[nodiscard]] virtual std::vector<NotNull<Mob>> getMobs() const = 0;

    /**
     * Get a list of all players in this dimension.
     *
     * @return A List of all players currently residing in this dimension
     */
    [[nodiscard]] virtual std::vector<NotNull<Player>> getPlayers() const = 0;
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

inline NotNull<Block> Location::getBlock() const
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
    auto format(const endstone::Dimension &self, FormatContext &ctx) const
    {
        return std::format_to(ctx.out(), "Dimension(id={})", self.getId());
    }
};
