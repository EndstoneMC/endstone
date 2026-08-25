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

#include <optional>
#include <stdexcept>
#include <type_traits>

#include "bedrock/world/level/block/actor/block_actor.h"
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

    EndstoneBlockStateBase(const EndstoneBlock &block, const ::BlockActor &block_actor)
        : EndstoneBlockStateBase(block)
    {
        block_actor_type_ = block_actor.getType();
    }

    explicit EndstoneBlockStateBase(NotNull<Dimension> dimension, BlockPos block_pos, const ::Block &block)
        : dimension_(dimension.cast<EndstoneDimension>()), block_pos_(block_pos),
          block_(const_cast<::Block *>(&block))
    {
    }

    [[nodiscard]] ClassInfo getClassInfo() const override
    {
        return typeid(Interface);
    }

    [[nodiscard]] bool isInstanceOf(ClassInfo target) const override
    {
        return core::isInstanceOf(*this, target);
    }

    [[nodiscard]] NotNull<Block> getBlock() const override { return EndstoneBlock::at(getBlockSource(), block_pos_); }

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

    [[nodiscard]] NotNull<BlockData> getData() const override { return std::make_shared<EndstoneBlockData>(*block_); }

    void setData(const BlockData &data) override
    {
        block_ = &static_cast<const EndstoneBlockData &>(data).getHandle();
    }

    [[nodiscard]] NotNull<Dimension> getDimension() const override
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
        return Location{getDimension(), getX(), getY(), getZ()};
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
    [[nodiscard]] BlockSource &getBlockSource() const
    {
        return dimension_->getHandle().getBlockSourceFromMainChunkSource();
    }

    template <typename T>
    [[nodiscard]] T &getBlockActor() const
    {
        auto *block_entity = getBlockSource().getBlockEntity(block_pos_);
        if (block_entity == nullptr || !block_actor_type_.has_value() ||
            block_entity->getType() != block_actor_type_.value()) {
            throw std::runtime_error("Trying to access a block state that is no longer valid.");
        }
        return static_cast<T &>(*block_entity);
    }

    NotNull<EndstoneDimension> dimension_;
    BlockPos block_pos_;
    ::Block *block_;
    std::optional<::BlockActorType> block_actor_type_;
};

using EndstoneBlockState = EndstoneBlockStateBase<BlockState>;

}  // namespace endstone::core
