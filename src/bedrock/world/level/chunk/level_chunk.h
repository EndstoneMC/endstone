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

#include <array>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/common_types.h"
#include "bedrock/forward.h"
#include "bedrock/platform/threading/mutex_details.h"
#include "bedrock/platform/threading/spin_lock.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/chunk/chunk_block_pos.h"
#include "bedrock/world/level/chunk/chunk_state.h"
#include "bedrock/world/level/chunk/sub_chunk_storage.h"
#include "bedrock/world/level/chunk_pos.h"
#include "bedrock/world/level/tick.h"

class Level;
class Dimension;
class ChunkSource;
class Biome;
class BiomeRegistry;
class BlockTickingQueue;
class SubChunk;

class LevelChunkBiomes {
public:
    std::array<BiomeIdType, 256> biomes_2d;
    std::vector<std::unique_ptr<SubChunkStorage<Biome>>> biome_subchunks;
    Bedrock::Threading::SharedMutex biomes_mutex;
    std::uint16_t biome_stack_size_3d;
};
BEDROCK_STATIC_ASSERT_SIZE(LevelChunkBiomes, 552, 600);

struct ColumnCachedData {
    int grass_color;
    int water_color;
};
static_assert(sizeof(ColumnCachedData) == 8);

enum class SubChunkInitMode : int {
    All = 0,
    AllButLast = 1,
    None = 2,
    ClientRequestSystemBlock = 3,
    ReplaceWithAllAir = 4,
};

namespace LevelChunkTicking {
enum class Entity : uint32_t;
}  // namespace LevelChunkTicking

class LevelChunk {
public:
    LevelChunk(Dimension &, const ChunkPos &, bool, SubChunkInitMode, bool);

    [[nodiscard]] const std::atomic<ChunkState> &getState() const;
    [[nodiscard]] Tick getLastTick() const;
    [[nodiscard]] const BlockPos &getMin() const;
    [[nodiscard]] const BlockPos &getMax() const;
    [[nodiscard]] const ChunkPos &getPosition() const;
    [[nodiscard]] ChunkSource *getGenerator() const;
    [[nodiscard]] Dimension &getDimension() const;
    [[nodiscard]] Level &getLevel() const;
    [[nodiscard]] const Biome &getBiome(const ChunkBlockPos &pos) const;

private:
    Bedrock::Threading::Mutex block_entity_access_lock_;
    Level &level_;
    Dimension &dimension_;
    BlockPos min_;
    BlockPos max_;
    ChunkPos position_;
    bool lighting_fixup_done_;
    std::atomic<bool> lighting_task_active_;
    bool read_only_;
    ChunkSource *generator_;
    LevelChunkTicking::Entity tmp_ticking_entity_;
    std::optional<LevelChunkFormat> loaded_format_;
    std::string serialized_entities_buffer_;
    std::string failed_serialized_entities_buffer_;
    bool had_serialized_entities_;
    std::vector<void *> unresolved_actor_links_;
    std::atomic<ChunkState> load_state_;
    ChunkTerrainDataState terrain_data_state_;
    ChunkDebugDisplaySavedState debug_display_saved_state_;
    ChunkCachedDataState cached_data_state_;
    SpinLock cached_data_state_spin_lock_;
    SpinLock client_request_heightmap_adjust_spin_lock_;
    Tick last_tick_;
    std::unique_ptr<BlockTickingQueue> tick_queue_;
    std::unique_ptr<BlockTickingQueue> random_tick_queue_;
    std::vector<SubChunk> sub_chunks_;
    std::vector<std::unique_ptr<SpinLock>> sub_chunk_spin_locks_;
    LevelChunkBiomes biomes_;
    std::array<ColumnCachedData, 256> cached_data_;
    std::array<ChunkLocalHeight, 256> heightmap_;
    std::array<ChunkLocalHeight, 256> render_heightmap_;
    std::unique_ptr<std::vector<std::int16_t>> pre_world_gen_heightmap_;
    ChunkLocalHeight non_air_max_height_;
    std::unordered_map<std::uint16_t, std::uint16_t> biome_states_;  // opaque: <BiomeIdType, BiomeChunkState>
    bool has_cached_temperature_noise_;
    std::array<bool, 256> border_block_map_;
    int current_instatick_;
    std::uint32_t finalized_;  // opaque: LevelChunk::Finalization
    bool is_redstone_loaded_;
    bool owned_by_ticking_thread_;
    bool use_3d_biome_maps_;
    // Remaining members (telemetry, entities, block entities, ...) are not reconstructed.
};
