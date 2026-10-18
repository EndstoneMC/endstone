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

#include <cstddef>
#include <cstdint>
#include <memory>

#include "bedrock/bedrock.h"
#include "bedrock/core/hash/xxhash.h"
#include "bedrock/forward.h"
#include "bedrock/platform/threading/mutex_details.h"
#include "bedrock/world/level/chunk/dirty_ticks_counter.h"
#include "bedrock/world/level/chunk/sub_chunk_storage.h"

using SubChunkBlockStorage = SubChunkStorage<Block>;

struct SubChunk {
    using Layer = std::uint8_t;
    enum class BlockLayer : Layer {
        Standard = 0,
        Extra = 1,
        Count = 2,
    };
    enum class SubChunkState : int {
        Invalid = -1,
        Normal = 0,
        IsLightingSystemSubChunk = 1,
        NeedsRequest = 2,
        ReceivedResponseFromServer = 3,
        ProcessingSubChunk = 4,
        WaitingForCacheResponse = 5,
        ProcessedSubChunk = 6,
        RequestFinished = 7,
    };
    DirtyTicksCounter dirty_ticks_counter;
    SubChunk();
    SubChunk(const Block *init_block, bool max_sky_light, bool max_light, std::int8_t absolute_index);
    ~SubChunk();
    SubChunk(const SubChunk &);
    SubChunk &operator=(const SubChunk &);
    SubChunk(SubChunk &&rhs);
    SubChunk &operator=(SubChunk &&rhs);

protected:
    static const Util::XXHash::Digest DEFAULT_HASH = 0;
    std::unique_ptr<SubChunkBrightnessStorage> sky_light_;
    std::unique_ptr<SubChunkBrightnessStorage> block_light_;
    SubChunkState sub_chunk_state_;
    bool has_max_sky_light_;
    bool needs_init_lighting_;
    bool needs_client_lighting_;
    static const std::size_t LAYERS = 2;
    std::unique_ptr<SubChunkStorage<Block>> blocks_[2];

public:  // Endstone: protected -> public
    SubChunkBlockStorage *blocks_read_ptr_[2];

protected:
    SpinLock write_lock_;
    Util::XXHash::Digest hash_;
    bool hash_dirty_;
    std::int8_t absolute_index_;
    bool is_replacement_sub_chunk_;
    std::uint8_t render_chunk_tracking_version_number_;
    bool is_initialized_;
};
BEDROCK_STATIC_ASSERT_SIZE(SubChunk, 104, 104);
