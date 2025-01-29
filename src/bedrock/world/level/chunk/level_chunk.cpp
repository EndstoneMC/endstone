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

#include "bedrock/world/level/chunk/level_chunk.h"

const std::atomic<ChunkState> &LevelChunk::getState() const
{
    return load_state_;
}

Tick LevelChunk::getLastTick() const
{
    return last_tick_;
}

const BlockPos &LevelChunk::getMin() const
{
    return min_;
}

const BlockPos &LevelChunk::getMax() const
{
    return max_;
}

const ChunkPos &LevelChunk::getPosition() const
{
    return position_;
}

ChunkSource *LevelChunk::getGenerator() const
{
    return generator_;
}

Dimension &LevelChunk::getDimension() const
{
    return dimension_;
}

Level &LevelChunk::getLevel() const
{
    return level_;
}
