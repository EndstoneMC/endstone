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

#pragma once

#include <type_traits>

#include "bedrock/world/level/block/block_descriptor.h"
#include "endstone/block/block.h"
#include "endstone/block/block_state.h"
#include "endstone/check.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/level/dimension.h"
#include "endstone/core/type.h"

namespace endstone::core {

template <typename Interface = BlockState>
    requires std::is_base_of_v<BlockState, Interface>
class EndstoneBlockStateBase : public Interface {
public:
    explicit EndstoneBlockStateBase(const EndstoneBlock &block)
        : EndstoneBlockStateBase(block.getDimension(), block.getPosition(), block.getMinecraftBlock())
    {
    }

    explicit EndstoneBlockStateBase(Dimension &dimension, BlockPos block_pos, const ::Block &block)
        : dimension_(static_cast<EndstoneDimension &>(dimension)),
          block_source_(dimension_.getHandle().getBlockSourceFromMainChunkSource()), block_pos_(block_pos),
          block_(const_cast<::Block *>(&block))
    {
    }

    [[nodiscard]] const std::type_info &getClassTypeId() const override
    {
        return typeid(Interface);
    }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return core::isInstanceOf(*this, target);
    }

    [[nodiscard]] std::unique_ptr<Block> getBlock() const override
    {
        return EndstoneBlock::at(block_source_, block_pos_);
    }

    [[nodiscard]] const BlockType &getType() const override
    {
        return *BlockType::get(block_->getName().getString());
    }

    void setType(BlockTypeId type) override
    {
        if (getType() != type) {
            using ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor;
            const auto block_descriptor = createBlockDescriptor(std::string(type), std::nullopt);
            auto *block = const_cast<::Block *>(block_descriptor.tryGetBlockNoLogging());
            Preconditions::checkArgument(block != nullptr, "BlockState::setType failed: unknown block type {}.", type);
            block_ = block;
        }
    }

    [[nodiscard]] std::unique_ptr<BlockData> getData() const override
    {
        return std::make_unique<EndstoneBlockData>(*block_);
    }

    void setData(const BlockData &data) override
    {
        block_ = &static_cast<const EndstoneBlockData &>(data).getHandle();
    }

    [[nodiscard]] Dimension &getDimension() const override
    {
        return dimension_;
    }

    [[nodiscard]] int getX() const override
    {
        return block_pos_.x;
    }

    [[nodiscard]] int getY() const override
    {
        return block_pos_.y;
    }

    [[nodiscard]] int getZ() const override
    {
        return block_pos_.z;
    }

    [[nodiscard]] Location getLocation() const override
    {
        return Location{getDimension().shared_from_this(), getX(), getY(), getZ()};
    }

    bool update() override
    {
        return update(false);
    }

    bool update(bool force) override
    {
        return update(force, true);
    }

    bool update(bool force, bool apply_physics) override
    {
        const auto block = getBlock();
        if (block->getType() != getType() && !force) {
            return false;
        }
        block->setData(*getData(), apply_physics);
        return true;
    }

protected:
    EndstoneDimension &dimension_;
    BlockSource &block_source_;
    BlockPos block_pos_;
    ::Block *block_;
};

using EndstoneBlockState = EndstoneBlockStateBase<BlockState>;

}  // namespace endstone::core
