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
#include <deque>
#include <optional>
#include <unordered_map>

#include "bedrock/bedrock.h"
#include "bedrock/world/level/tick.h"

class Block;
class LevelChunk;

struct ChunkBlockPos {
    std::uint8_t x;
    std::uint8_t z;
    std::uint16_t y;
};

class NeighborChangedQueue {
public:
    struct Record {
        ChunkBlockPos notified_pos_;
        Block *listener_permutation_;
        alignas(8) std::byte changes_[40];
    };

    explicit NeighborChangedQueue(LevelChunk &owning_chunk);

private:
    std::deque<Record> delivery_queue_;
    std::unordered_map<ChunkBlockPos, Record *> same_tick_merge_index_;
    std::optional<Tick> merge_index_tick_;
    LevelChunk &owning_chunk_;
};
BEDROCK_STATIC_ASSERT_SIZE(NeighborChangedQueue, 128, 112);
