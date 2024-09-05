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

#include "endstone/detail/block/block_state.h"

#include <bedrock/world/level/block/block_descriptor.h>
#include <endstone/detail/block/block_data.h>

#include "endstone/detail/block/block.h"

namespace endstone::detail {

EndstoneBlockState::EndstoneBlockState(const EndstoneBlock &block)
    : dimension_(static_cast<EndstoneDimension &>(block.getDimension())),
      block_source_(dimension_.getHandle().getBlockSourceFromMainChunkSource()), block_pos_(block.getPosition()),
      block_(&block.getMinecraftBlock())
{
}

std::unique_ptr<Block> EndstoneBlockState::getBlock() const
{
    return EndstoneBlock::at(block_source_, block_pos_);
}

std::string EndstoneBlockState::getType() const
{
    return block_->getLegacyBlock().getFullNameId();
}

void EndstoneBlockState::setType(std::string type)
{
    if (getType() != type) {
        const auto block_descriptor =
            ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor(type, std::nullopt);
        auto *block = const_cast<::Block *>(block_descriptor.tryGetBlockNoLogging());
        if (!block) {
            const auto &server = entt::locator<EndstoneServer>::value();
            server.getLogger().error("BlockState::setType failed: unknown block type {}.", type);
            return;
        }
        block_ = block;
    }
}

std::shared_ptr<BlockData> EndstoneBlockState::getData() const
{
    return std::make_shared<EndstoneBlockData>(*block_);
}

void EndstoneBlockState::setData(std::shared_ptr<BlockData> data)
{
    if (!data) {
        const auto &server = entt::locator<EndstoneServer>::value();
        server.getLogger().error("BlockState::setData failed: data cannot be null.");
        return;
    }
    block_ = &std::dynamic_pointer_cast<EndstoneBlockData>(data)->getHandle();
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

bool EndstoneBlockState::update()
{
    return update(false);
}

bool EndstoneBlockState::update(bool force)
{
    return update(force, true);
}

bool EndstoneBlockState::update(bool force, bool apply_physics)
{
    const auto block = getBlock();
    if (block->getType() != getType() && !force) {
        return false;
    }
    block->setData(getData(), apply_physics);
    return true;
}

}  // namespace endstone::detail
