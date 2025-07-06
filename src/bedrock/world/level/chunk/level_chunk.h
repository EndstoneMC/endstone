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

#include <optional>
#include <vector>

#include "bedrock/forward.h"
#include "bedrock/platform/threading/mutex_details.h"
#include "bedrock/platform/threading/spin_lock.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/chunk/chunk_state.h"
#include "bedrock/world/level/chunk_pos.h"
#include "bedrock/world/level/tick.h"

class Level;
class Dimension;
class ChunkSource;

enum class SubChunkInitMode : int {
    All = 0,
    AllButLast = 1,
    None = 2,
    ClientRequestSystemBlock = 3,
    ReplaceWithAllAir = 4,
};

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
    // ...
};
