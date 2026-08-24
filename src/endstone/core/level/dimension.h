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

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "bedrock/platform/uuid.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/actor/actor.h"
#include "endstone/actor/item.h"
#include "endstone/core/server.h"
#include "endstone/level/dimension.h"

class LevelChunk;

namespace endstone::core {
class EndstoneDimension : public Dimension, public std::enable_shared_from_this<EndstoneDimension> {
public:
    explicit EndstoneDimension(WeakRef<::Dimension> dimension, EndstoneLevel &level);
    ~EndstoneDimension() override = default;
    [[nodiscard]] DimensionId getId() const override;
    [[nodiscard]] std::string getTranslationKey() const override;
    [[nodiscard]] Level &getLevel() const override;
    [[nodiscard]] bool isValid() const override;
    [[nodiscard]] NotNull<Block> getBlockAt(int x, int y, int z) const override;
    [[nodiscard]] NotNull<Block> getBlockAt(Location location) const override;
    [[nodiscard]] int getHighestBlockYAt(int x, int z) const override;
    [[nodiscard]] NotNull<Block> getHighestBlockAt(int x, int z) const override;
    [[nodiscard]] NotNull<Block> getHighestBlockAt(Location location) const override;
    [[nodiscard]] std::vector<NotNull<Chunk>> getLoadedChunks() override;
    [[nodiscard]] bool isChunkLoaded(int x, int z) const override;
    [[nodiscard]] bool isChunkGenerated(int x, int z) const override;
    bool loadChunk(int x, int z) override;
    bool loadChunk(int x, int z, bool generate) override;
    bool unloadChunk(int x, int z) override;
    bool unloadChunkRequest(int x, int z) override;
    bool addPluginChunkTicket(int x, int z, Plugin &plugin) override;
    bool removePluginChunkTicket(int x, int z, Plugin &plugin) override;
    void removePluginChunkTickets(Plugin &plugin) override;
    [[nodiscard]] std::vector<Plugin *> getPluginChunkTickets(int x, int z) const override;
    [[nodiscard]] std::unordered_map<Plugin *, std::vector<NotNull<Chunk>>> getPluginChunkTickets() const override;
    [[nodiscard]] NotNull<Item> dropItem(Location location, const ItemStack &item) override;
    [[nodiscard]] Nullable<Actor> spawnActor(Location location, ActorTypeId type) override;
    [[nodiscard]] std::vector<NotNull<Actor>> getActors() const override;
    [[nodiscard]] std::vector<NotNull<Mob>> getMobs() const override;
    [[nodiscard]] std::vector<NotNull<Player>> getPlayers() const override;

    [[nodiscard]] ::Dimension &getHandle() const;
    [[nodiscard]] NotNull<EndstoneDimension> self() const;

private:
    struct ChunkTicket {
        mce::UUID area_id;
        std::vector<Plugin *> plugins;
        bool server_owned = false;
    };

    void checkServerThread() const;
    mce::UUID pinChunk(int x, int z, bool generate);
    void releaseTicket(std::unordered_map<std::uint64_t, ChunkTicket>::iterator it);

    WeakRef<::Dimension> dimension_;
    EndstoneLevel &level_;
    std::unordered_map<std::uint64_t, ChunkTicket> chunk_tickets_;
};
}  // namespace endstone::core
