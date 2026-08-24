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
