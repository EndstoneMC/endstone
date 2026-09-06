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
#include <vector>

#include "endstone/actor/actor.h"

namespace endstone {

class Block;
class BlockState;
class Plugin;

/**
 * Represents a chunk of blocks.
 */
class Chunk {
public:
    virtual ~Chunk() = default;

    /**
     * Gets the X-coordinate of this chunk.
     *
     * @return X-coordinate
     */
    [[nodiscard]] virtual int getX() const = 0;

    /**
     * Gets the Z-coordinate of this chunk.
     *
     * @return Z-coordinate
     */
    [[nodiscard]] virtual int getZ() const = 0;

    /**
     * Gets the level containing this chunk.
     *
     * @return Parent Level
     */
    [[nodiscard]] virtual Level &getLevel() const = 0;

    /**
     * Gets the dimension containing this chunk.
     *
     * @return Parent Dimension
     */
    [[nodiscard]] virtual NotNull<Dimension> getDimension() const = 0;

    /**
     * Gets a block from this chunk.
     *
     * This does not load the chunk. Use load() to request it before accessing block data.
     *
     * @param x X-coordinate within the chunk, from 0 to 15
     * @param y Y-coordinate in the dimension, from its minimum height (inclusive) to its maximum height (exclusive)
     * @param z Z-coordinate within the chunk, from 0 to 15
     * @return Block at the given coordinates
     * @throws std::invalid_argument if the coordinates are outside these bounds
     */
    [[nodiscard]] virtual NotNull<Block> getBlock(int x, int y, int z) const = 0;

    /**
     * Gets a list of all loaded entities in this chunk, including players.
     *
     * This does not load the chunk or its entities. The returned list is a snapshot.
     *
     * @return Entities whose positions are in this chunk, or an empty list if this chunk is not loaded
     */
    [[nodiscard]] virtual std::vector<NotNull<Actor>> getEntities() const = 0;

    /**
     * Checks if this chunk has been generated.
     *
     * A chunk counts as generated once it is loaded or has been written to the level's chunk storage.
     * This does not load or generate the chunk.
     *
     * @return `true` if the chunk has been generated, otherwise `false`
     */
    [[nodiscard]] virtual bool isGenerated() const = 0;

    /**
     * Checks if this chunk's coordinates qualify for slime spawning outside swamp biomes.
     *
     * Bedrock's slime chunk pattern depends only on chunk coordinates, not the world seed.
     * This does not load the chunk or check other spawning conditions.
     *
     * @return `true` if these coordinates identify a slime chunk, otherwise `false`
     */
    [[nodiscard]] virtual bool isSlimeChunk() const = 0;

    /**
     * Checks if this chunk is loaded.
     *
     * @return `true` if the chunk is loaded, otherwise `false`
     */
    [[nodiscard]] virtual bool isLoaded() const = 0;

    /**
     * Requests this chunk to be loaded, generating it if it does not exist yet.
     *
     * Equivalent to `load(true)`.
     *
     * @return `true` if the chunk is now held resident, otherwise `false`
     */
    virtual bool load() = 0;

    /**
     * Requests this chunk to be loaded, and keeps it resident until it is released again.
     *
     * @see Dimension::loadChunk() for how the hold behaves.
     *
     * @param generate Whether to generate the chunk if it does not exist yet
     * @return `false` if `generate` is `false` and the chunk has not been generated, otherwise `true`
     */
    virtual bool load(bool generate) = 0;

    /**
     * Releases the hold that `load()` placed on this chunk, and unloads it if nothing else keeps it resident.
     *
     * @return `true` if the chunk is no longer loaded, otherwise `false`
     */
    virtual bool unload() = 0;

    /**
     * Checks whether this chunk is force loaded.
     *
     * @return Force-load status, including a load that has not finished yet
     * @throws std::runtime_error If called outside the server thread or the dimension is no longer valid
     * @see Dimension::isChunkForceLoaded()
     */
    [[nodiscard]] virtual bool isForceLoaded() const = 0;

    /**
     * Sets whether this chunk is force loaded.
     *
     * The chunk is kept resident until force loading is disabled or the server restarts. Loading finishes on a later
     * tick and does not make the chunk tick. Disabling force loading leaves other holds and plugin tickets intact.
     *
     * @param forced Whether to force load the chunk
     * @throws std::runtime_error If called outside the server thread, the dimension is no longer valid, or the chunk
     *                           cannot be held resident
     * @see Dimension::setChunkForceLoaded()
     */
    virtual void setForceLoaded(bool forced) = 0;

    /**
     * Adds a plugin ticket for this chunk, loading it if it is not already loaded.
     *
     * @param plugin Plugin taking the ticket
     * @return `true` if a plugin ticket was added, `false` if the plugin already holds one for this chunk
     */
    virtual bool addPluginChunkTicket(Plugin &plugin) = 0;

    /**
     * Removes the given plugin's ticket for this chunk.
     *
     * @param plugin Plugin whose ticket to remove
     * @return `true` if a plugin ticket was removed, `false` if the plugin holds none for this chunk
     */
    virtual bool removePluginChunkTicket(Plugin &plugin) = 0;

    /**
     * Gets a list of all block actors in this chunk.
     *
     * The returned list is a snapshot; it does not track block actors placed or broken afterwards.
     *
     * @return the state of every block actor in this chunk, or an empty list if this chunk is not loaded
     */
    [[nodiscard]] virtual std::vector<NotNull<BlockState>> getBlockActors() const = 0;

    /**
     * Gets which plugins hold a ticket for this chunk.
     *
     * The returned list is a snapshot; it does not track tickets added or removed afterwards.
     *
     * @return Plugins holding a ticket for this chunk
     */
    [[nodiscard]] virtual std::vector<Plugin *> getPluginChunkTickets() const = 0;
};

}  // namespace endstone

template <>
struct std::formatter<endstone::Chunk> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::Chunk &self, FormatContext &ctx) const
    {
        return std::format_to(ctx.out(), "Chunk(x={}, z={})", self.getX(), self.getZ());
    }
};
