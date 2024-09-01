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

#include "endstone/detail/block/block.h"

#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/level.h"
#include "endstone/detail/block/block_face.h"
#include "endstone/detail/server.h"

namespace endstone::detail {
EndstoneBlock::EndstoneBlock(BlockSource &block_source, BlockPos block_pos)
    : block_source_(block_source), block_pos_(block_pos)
{
    (void)checkState();
}

bool EndstoneBlock::isValid() const
{
    return checkState();
}

std::string EndstoneBlock::getType() const
{
    if (checkState()) {
        return block_source_.getBlock(block_pos_).getLegacyBlock().getFullNameId();
    }
    return "minecraft:air";
}

void EndstoneBlock::setType(std::string type)
{
    setType(type, true);
}

void EndstoneBlock::setType(std::string type, bool apply_physics)
{
    using detail::EndstoneServer;
    if (checkState()) {
        auto &server = entt::locator<EndstoneServer>::value();
        setData(server.createBlockData(type), apply_physics);
    }
}

BlockData &EndstoneBlock::getData()
{
    // TODO: implement this
    throw std::runtime_error("BlockData::getBlockData(): not implemented");
}

void EndstoneBlock::setData(std::unique_ptr<BlockData> data)
{
    setData(std::move(data), true);
}

void EndstoneBlock::setData(std::unique_ptr<BlockData> data, bool apply_physics)
{
    // TODO: implement this
    throw std::runtime_error("BlockData::setBlockData(): not implemented");
}

std::unique_ptr<Block> EndstoneBlock::getRelative(int offset_x, int offset_y, int offset_z)
{
    if (checkState()) {
        return getDimension().getBlockAt(getX() + offset_x, getY() + offset_y, getZ() + offset_z);
    }
    return nullptr;
}

std::unique_ptr<Block> EndstoneBlock::getRelative(BlockFace face)
{
    if (checkState()) {
        return getRelative(face, 1);
    }
    return nullptr;
}

std::unique_ptr<Block> EndstoneBlock::getRelative(BlockFace face, int distance)
{
    if (checkState()) {
        return getRelative(EndstoneBlockFace::getOffsetX(face) * distance,
                           EndstoneBlockFace::getOffsetY(face) * distance,
                           EndstoneBlockFace::getOffsetZ(face) * distance);
    }
    return nullptr;
}

Dimension &EndstoneBlock::getDimension() const
{
    return block_source_.getDimension().getEndstoneDimension();
}

int EndstoneBlock::getX() const
{
    return block_pos_.x;
}

int EndstoneBlock::getY() const
{
    return block_pos_.y;
}

int EndstoneBlock::getZ() const
{
    return block_pos_.z;
}

Location EndstoneBlock::getLocation() const
{
    return {&getDimension(), getX(), getY(), getZ()};
}

std::unique_ptr<EndstoneBlock> EndstoneBlock::at(BlockSource &block_source, BlockPos block_pos)
{
    return std::make_unique<EndstoneBlock>(block_source, block_pos);
}

bool EndstoneBlock::checkState() const
{
    auto &server = entt::locator<EndstoneServer>::value();

    if (block_pos_.y < block_source_.getMinHeight() || block_pos_.y > block_source_.getMaxHeight()) {
        server.getLogger().error("Trying to access location ({}, {}, {}) which is outside of the world boundaries.",
                                 block_pos_.x, block_pos_.y, block_pos_.z);
        return false;
    }

    auto *chunk = block_source_.getChunkAt(block_pos_);
    if (!chunk) {
        server.getLogger().error("Trying to access location ({}, {}, {}) which is not in a chunk currently loaded.",
                                 block_pos_.x, block_pos_.y, block_pos_.z);
        return false;
    }

    auto current_level_tick = block_source_.getLevel().getCurrentTick();
    auto chunk_last_tick = chunk->getLastTick();
    if (current_level_tick != chunk_last_tick && current_level_tick != chunk_last_tick + 1) {
        server.getLogger().error("Trying to access location ({}, {}, {}) which is not in a chunk currently ticking.",
                                 block_pos_.x, block_pos_.y, block_pos_.z);
        return false;
    }
    return true;
}

}  // namespace endstone::detail
