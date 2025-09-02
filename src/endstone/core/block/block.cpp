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

#include "endstone/core/block/block.h"

#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/block/block_state.h"
#include "endstone/core/server.h"

using endstone::core::EndstoneServer;

namespace endstone::core {
EndstoneBlock::EndstoneBlock(BlockSource &block_source, BlockPos block_pos)
    : block_source_(block_source), block_pos_(block_pos)
{
}

std::string EndstoneBlock::getType() const
{
    return block_source_.get().getBlock(block_pos_).getName().getString();
}

Result<void> EndstoneBlock::setType(std::string type)
{
    return setType(type, true);
}

Result<void> EndstoneBlock::setType(std::string type, bool apply_physics)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    auto result = server.createBlockData(type);
    if (!result) {
        return nonstd::make_unexpected(result.error());
    }
    return setData(*result.value(), apply_physics);
}

std::unique_ptr<BlockData> EndstoneBlock::getData() const
{
    return std::make_unique<EndstoneBlockData>(getMinecraftBlock());
}

Result<void> EndstoneBlock::setData(const BlockData &data)
{
    return setData(std::move(data), true);
}

Result<void> EndstoneBlock::setData(const BlockData &data, bool apply_physics)
{
    const ::Block &block = static_cast<const EndstoneBlockData &>(data).getHandle();
    if (apply_physics) {
        block_source_.get().setBlock(block_pos_, block, BlockType::UPDATE_NEIGHBORS | BlockType::UPDATE_CLIENTS,
                                     nullptr, nullptr);
    }
    else {
        block_source_.get().setBlock(block_pos_, block, BlockType::UPDATE_CLIENTS, nullptr, nullptr);  // NETWORK
    }
    return {};
}

std::unique_ptr<Block> EndstoneBlock::getRelative(int offset_x, int offset_y, int offset_z)
{
    return getDimension().getBlockAt(getX() + offset_x, getY() + offset_y, getZ() + offset_z);
}

std::unique_ptr<Block> EndstoneBlock::getRelative(BlockFace face)
{
    return getRelative(face, 1);
}

std::unique_ptr<Block> EndstoneBlock::getRelative(BlockFace face, int distance)
{
    return getRelative(EndstoneBlockFace::getOffsetX(face) * distance, EndstoneBlockFace::getOffsetY(face) * distance,
                       EndstoneBlockFace::getOffsetZ(face) * distance);
}

Dimension &EndstoneBlock::getDimension() const
{
    return block_source_.get().getDimension().getEndstoneDimension();
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
    return {getX(), getY(), getZ(), getDimension()};
}

std::unique_ptr<BlockState> EndstoneBlock::captureState() const
{
    return std::make_unique<EndstoneBlockState>(*this);
}

std::unique_ptr<Block> EndstoneBlock::clone() const
{
    return std::make_unique<EndstoneBlock>(*this);
}

BlockPos EndstoneBlock::getPosition() const
{
    return block_pos_;
}

::Block &EndstoneBlock::getMinecraftBlock() const
{
    return const_cast<::Block &>(block_source_.get().getBlock(block_pos_));
}

std::unique_ptr<EndstoneBlock> EndstoneBlock::at(BlockSource &block_source, BlockPos block_pos)
{
    return std::make_unique<EndstoneBlock>(block_source, block_pos);
}
}  // namespace endstone::core
