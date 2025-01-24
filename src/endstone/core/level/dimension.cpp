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

#include "endstone/core/level/dimension.h"

#include "bedrock/world/level/block/bedrock_block_names.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/core/block/block.h"
#include "endstone/core/level/level.h"

namespace endstone::core {

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
    switch (dimension_.getDimensionId().runtime_id) {
    case VanillaDimensions::Overworld.runtime_id:
        return Type::Overworld;
    case VanillaDimensions::Nether.runtime_id:
        return Type::Nether;
    case VanillaDimensions::TheEnd.runtime_id:
        return Type::TheEnd;
    default:
        return Type::Custom;
    }
}

Level &EndstoneDimension::getLevel() const
{
    return level_;
}

Result<std::shared_ptr<Block>> EndstoneDimension::getBlockAt(int x, int y, int z) const
{
    return EndstoneBlock::at(getHandle().getBlockSourceFromMainChunkSource(), BlockPos(x, y, z));
}

Result<std::shared_ptr<Block>> EndstoneDimension::getBlockAt(Location location) const
{
    return getBlockAt(location.getBlockX(), location.getBlockY(), location.getBlockZ());
}

int EndstoneDimension::getHighestBlockYAt(int x, int z) const
{
    return getHandle().getBlockSourceFromMainChunkSource().getHeight(
        [](auto &block) { return block.getName() != BedrockBlockNames::Air; }, x, z);
}

Result<std::shared_ptr<Block>> EndstoneDimension::getHighestBlockAt(int x, int z) const
{
    return getBlockAt(x, getHighestBlockYAt(x, z), z);
}

Result<std::shared_ptr<Block>> EndstoneDimension::getHighestBlockAt(Location location) const
{
    return getHighestBlockAt(location.getBlockX(), location.getBlockZ());
}

::Dimension &EndstoneDimension::getHandle() const
{
    return dimension_;
}

}  // namespace endstone::core

endstone::Dimension &Dimension::getEndstoneDimension() const
{
    using endstone::core::EndstoneServer;
    auto &server = entt::locator<EndstoneServer>::value();
    return *server.getLevel()->getDimension(getName());
}
