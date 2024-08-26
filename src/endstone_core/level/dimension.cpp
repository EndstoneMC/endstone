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

#include "endstone/detail/level/dimension.h"

#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/detail/block/block.h"
#include "endstone/detail/level/level.h"

namespace endstone::detail {

EndstoneDimension::EndstoneDimension(::Dimension &dimension, EndstoneLevel &level)
    : dimension_(dimension), level_(level)
{
}

std::string EndstoneDimension::getName() const
{
    return dimension_.getName();
}

Dimension::Type EndstoneDimension::getType() const
{
    switch (dimension_.getDimensionId().id) {
    case VanillaDimensions::Overworld.id:
        return Type::Overworld;
    case VanillaDimensions::Nether.id:
        return Type::Nether;
    case VanillaDimensions::TheEnd.id:
        return Type::TheEnd;
    default:
        return Type::Custom;
    }
}

Level &EndstoneDimension::getLevel() const
{
    return level_;
}

std::unique_ptr<Block> EndstoneDimension::getBlockAt(int x, int y, int z)
{
    auto &block_source = getHandle().getBlockSourceFromMainChunkSource();
    auto &logger = level_.getServer().getLogger();

    if (y < block_source.getMinHeight() || y > block_source.getMaxHeight()) {
        logger.error("Trying to access location ({}, {}, {}) which is outside of the world boundaries.", x, y, z);
        return nullptr;
    }

    auto *chunk = block_source.getChunkAt(BlockPos{x, y, z});
    if (!chunk) {
        logger.error("Trying to access location ({}, {}, {}) which is not in a chunk currently loaded.", x, y, z);
        return nullptr;
    }

    auto current_level_tick = level_.getHandle().getCurrentTick();
    auto chunk_last_tick = chunk->getLastTick();
    if (current_level_tick != chunk_last_tick && current_level_tick != chunk_last_tick + 1) {
        logger.error("Trying to access location ({}, {}, {}) which is not in a chunk currently ticking.", x, y, z);
        return nullptr;
    }

    return EndstoneBlock::at(dimension_.getBlockSourceFromMainChunkSource(), BlockPos(x, y, z));
}

::Dimension &EndstoneDimension::getHandle() const
{
    return dimension_;
}

}  // namespace endstone::detail
