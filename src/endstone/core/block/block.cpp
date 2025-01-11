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
#include "bedrock/world/level/level.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/block/block_state.h"
#include "endstone/core/server.h"
#include "endstone/core/util/error.h"

using endstone::core::EndstoneServer;

namespace endstone::core {
EndstoneBlock::EndstoneBlock(BlockSource &block_source, BlockPos block_pos)
    : block_source_(block_source), block_pos_(block_pos)
{
}

bool EndstoneBlock::isValid() const
{
    return checkState().has_value();
}

Result<std::string> EndstoneBlock::getType() const
{
    return checkState().and_then([&](const auto * /*self*/) -> Result<std::string> {
        return block_source_.getBlock(block_pos_).getLegacyBlock().getFullNameId();
    });
}

Result<void> EndstoneBlock::setType(std::string type)
{
    return setType(type, true);
}

Result<void> EndstoneBlock::setType(std::string type, bool apply_physics)
{
    return checkState().and_then([&](const auto * /*self*/) -> Result<void> {
        const auto &server = entt::locator<EndstoneServer>::value();
        auto result = server.createBlockData(type);
        if (!result) {
            return nonstd::make_unexpected(result.error());
        }
        return setData(result.value(), apply_physics);
    });
}

Result<std::shared_ptr<BlockData>> EndstoneBlock::getData() const
{
    return checkState().and_then([&](const auto * /*self*/) -> Result<std::shared_ptr<BlockData>> {
        return std::make_shared<EndstoneBlockData>(getMinecraftBlock());
    });
}

Result<void> EndstoneBlock::setData(std::shared_ptr<BlockData> data)
{
    return setData(std::move(data), true);
}

Result<void> EndstoneBlock::setData(std::shared_ptr<BlockData> data, bool apply_physics)
{
    if (!data) {
        return nonstd::make_unexpected(make_error("Block data cannot be null"));
    }

    return checkState().and_then([&](const auto * /*self*/) -> Result<void> {
        const ::Block &block = static_cast<EndstoneBlockData &>(*data).getHandle();
        if (apply_physics) {
            block_source_.setBlock(block_pos_, block, 1 | 2, nullptr, nullptr);  // NEIGHBORS | NETWORK
        }
        else {
            block_source_.setBlock(block_pos_, block, 2, nullptr, nullptr);  // NETWORK
        }
        return {};
    });
}

Result<std::shared_ptr<Block>> EndstoneBlock::getRelative(int offset_x, int offset_y, int offset_z)
{
    return checkState().and_then([&](const auto * /*self*/) -> Result<std::shared_ptr<Block>> {
        return getDimension().getBlockAt(getX() + offset_x, getY() + offset_y, getZ() + offset_z);
    });
}

Result<std::shared_ptr<Block>> EndstoneBlock::getRelative(BlockFace face)
{
    return checkState().and_then([&](const auto * /*self*/) -> Result<std::shared_ptr<Block>> {  //
        return getRelative(face, 1);
    });
}

Result<std::shared_ptr<Block>> EndstoneBlock::getRelative(BlockFace face, int distance)
{
    return checkState().and_then([&](const auto * /*self*/) -> Result<std::shared_ptr<Block>> {
        return getRelative(EndstoneBlockFace::getOffsetX(face) * distance,
                           EndstoneBlockFace::getOffsetY(face) * distance,
                           EndstoneBlockFace::getOffsetZ(face) * distance);
    });
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

std::shared_ptr<BlockState> EndstoneBlock::captureState() const
{
    return std::make_shared<EndstoneBlockState>(*this);
}

BlockPos EndstoneBlock::getPosition() const
{
    return block_pos_;
}

::Block &EndstoneBlock::getMinecraftBlock() const
{
    return const_cast<::Block &>(block_source_.getBlock(block_pos_));
}

Result<std::shared_ptr<EndstoneBlock>> EndstoneBlock::at(BlockSource &block_source, BlockPos block_pos)
{
    auto block = std::make_shared<EndstoneBlock>(block_source, block_pos);
    if (auto result = block->checkState(); !result) {
        return nonstd::make_unexpected(result.error());
    }
    return block;
}

Result<const EndstoneBlock *> EndstoneBlock::checkState() const
{
    if (block_pos_.y < block_source_.getMinHeight() || block_pos_.y > block_source_.getMaxHeight()) {
        return nonstd::make_unexpected(
            make_error("Trying to access location ({}, {}, {}) which is outside of the world boundaries.", block_pos_.x,
                       block_pos_.y, block_pos_.z));
    }

    const auto *chunk = block_source_.getChunkAt(block_pos_);
    if (!chunk) {
        return nonstd::make_unexpected(
            make_error("Trying to access location ({}, {}, {}) which is not in a chunk currently loaded.", block_pos_.x,
                       block_pos_.y, block_pos_.z));
    }

    const auto current_level_tick = block_source_.getLevel().getCurrentTick();
    const auto chunk_last_tick = chunk->getLastTick();
    if (current_level_tick != chunk_last_tick && current_level_tick != chunk_last_tick + 1) {
        return nonstd::make_unexpected(
            make_error("Trying to access location ({}, {}, {}) which is not in a chunk currently ticking.",
                       block_pos_.x, block_pos_.y, block_pos_.z));
    }
    return this;
}

}  // namespace endstone::core
