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

#include "endstone/core/block/block_state.h"

#include "bedrock/world/level/block/block_descriptor.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/util/error.h"

namespace endstone::core {

EndstoneBlockState::EndstoneBlockState(const EndstoneBlock &block)
    : EndstoneBlockState(block.getDimension(), block.getPosition(), block.getMinecraftBlock())
{
}

EndstoneBlockState::EndstoneBlockState(Dimension &dimension, BlockPos block_pos, ::Block &block)
    : dimension_(static_cast<EndstoneDimension &>(dimension)),
      block_source_(dimension_.getHandle().getBlockSourceFromMainChunkSource()), block_pos_(block_pos), block_(&block)
{
}

Result<std::unique_ptr<Block>> EndstoneBlockState::getBlock() const
{
    return EndstoneBlock::at(block_source_, block_pos_);
}

std::string EndstoneBlockState::getType() const
{
    return block_->getLegacyBlock().getFullNameId();
}

Result<void> EndstoneBlockState::setType(std::string type)
{
    if (getType() != type) {
        using ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor;
        const auto block_descriptor = createBlockDescriptor(type, std::nullopt);
        auto *block = const_cast<::Block *>(block_descriptor.tryGetBlockNoLogging());
        if (!block) {
            return nonstd::make_unexpected(make_error("BlockState::setType failed: unknown block type {}.", type));
        }
        block_ = block;
    }
    return {};
}

std::shared_ptr<BlockData> EndstoneBlockState::getData() const
{
    return std::make_shared<EndstoneBlockData>(*block_);
}

Result<void> EndstoneBlockState::setData(std::shared_ptr<BlockData> data)
{
    if (!data) {
        return nonstd::make_unexpected(make_error("Block data cannot be null"));
    }
    block_ = &std::dynamic_pointer_cast<EndstoneBlockData>(data)->getHandle();
    return {};
}

Dimension &EndstoneBlockState::getDimension() const
{
    return dimension_;
}

int EndstoneBlockState::getX() const
{
    return block_pos_.x;
}

int EndstoneBlockState::getY() const
{
    return block_pos_.y;
}

int EndstoneBlockState::getZ() const
{
    return block_pos_.z;
}

Location EndstoneBlockState::getLocation() const
{
    return Location{&dimension_, getX(), getY(), getZ()};
}

Result<bool> EndstoneBlockState::update()
{
    return update(false);
}

Result<bool> EndstoneBlockState::update(bool force)
{
    return update(force, true);
}

Result<bool> EndstoneBlockState::update(bool force, bool apply_physics)
{
    auto result = getBlock();
    if (!result) {
        return nonstd::make_unexpected(result.error());
    }

    auto &block = result.value();
    if (block->getType() != getType() && !force) {
        return false;
    }
    block->setData(getData(), apply_physics);
    return true;
}

}  // namespace endstone::core
