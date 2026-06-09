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

#include "bedrock/world/level/biome/biome.h"
#include "bedrock/world/level/biome/registry/biome_registry.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/level.h"

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

const Biome &LevelChunk::getBiome(const ChunkBlockPos &pos) const
{
    const BiomeRegistry &registry = level_.getBiomeRegistry();
    if (use_3d_biome_maps_) {
        if (biomes_.biome_stack_size_3d) {
            const auto y = static_cast<std::uint16_t>(pos.y.getVal());
            const std::uint16_t layer = y >> 4;
            const SubChunkStorage<Biome> *storage;
            std::uint16_t local_y;
            if (layer >= biomes_.biome_stack_size_3d) {
                storage = biomes_.biome_subchunks[biomes_.biome_stack_size_3d - 1].get();
                local_y = 15;
            }
            else {
                storage = biomes_.biome_subchunks[layer].get();
                local_y = y & 0xF;
            }
            const auto index = static_cast<std::uint16_t>(local_y + 16 * (16 * pos.x + pos.z));
            return storage->getElement(index);
        }
    }
    else if (const Biome *biome = registry.lookupById(biomes_.biomes_2d[(pos.x + 16 * pos.z) & 0xFF])) {
        return *biome;
    }
    // Fallback: the dimension's default biome.
    return *registry.lookupById(dimension_.getDefaultBiomeId());
}
