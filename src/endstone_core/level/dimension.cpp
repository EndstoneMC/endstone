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

namespace endstone::detail {

EndstoneDimension::EndstoneDimension(::Dimension &dimension, Level &level) : dimension_(dimension), level_(level) {}

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
    // TODO(block): check if the chunk is within boundary, loaded and ticking.
    return EndstoneBlock::at(dimension_.getBlockSourceFromMainChunkSource(), BlockPos(x, y, z));
}

::Dimension &EndstoneDimension::getHandle() const
{
    return dimension_;
}

}  // namespace endstone::detail
