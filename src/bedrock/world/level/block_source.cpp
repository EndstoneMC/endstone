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

#include "bedrock/world/level/block_source.h"

#include "bedrock/world/level/biome/registry/biome_registry.h"
#include "bedrock/world/level/block/bedrock_block_names.h"
#include "bedrock/world/level/chunk/chunk_block_pos.h"
#include "bedrock/world/level/chunk/level_chunk.h"
#include "bedrock/world/level/dimension/dimension.h"

bool BlockSource::isEmptyBlock(const BlockPos &pos) const
{
    return getBlock(pos).getName() == BedrockBlockNames::Air;
}

const Biome &BlockSource::getBiome(const BlockPos &pos) const
{
    if (const LevelChunk *chunk = getChunkAt(pos)) {
        const auto min_height = getMinHeight();
        if (pos.y >= min_height && pos.y < getMaxHeight()) {
            return chunk->getBiome(ChunkBlockPos(pos, min_height));
        }
    }
    // No chunk or outside the dimension's vertical range: the dimension's default biome.
    auto &dimension = getDimension();
    return *dimension.getBiomeRegistry().lookupById(dimension.getDefaultBiomeId());
}
