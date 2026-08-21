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

#include "bedrock/world/level/block/actor/block_actor.h"
#include "bedrock/world/level/block/actor/vanilla_block_actor.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/block/biome.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/block/block_state.h"
#include "endstone/core/block/campfire.h"
#include "endstone/core/block/container.h"
#include "endstone/core/block/creature_spawner.h"
#include "endstone/core/block/furnace.h"
#include "endstone/core/block/item_frame.h"
#include "endstone/core/block/lectern.h"
#include "endstone/core/block/sign.h"
#include "endstone/core/server.h"

using endstone::core::EndstoneServer;

namespace endstone::core {
EndstoneBlock::EndstoneBlock(BlockSource &block_source, BlockPos block_pos)
    : dimension_(block_source.getDimension().getEndstoneDimension().cast<EndstoneDimension>()),
      block_pos_(block_pos)
{
}

const BlockType &EndstoneBlock::getType() const
{
    return *BlockType::get(getBlockSource().getBlock(block_pos_).getName().getString());
}

void EndstoneBlock::setType(BlockTypeId type)
{
    return setType(type, true);
}

void EndstoneBlock::setType(BlockTypeId type, bool apply_physics)
{
    const auto &server = EndstoneServer::getInstance();
    return setData(*server.createBlockData(type), apply_physics);
}

NotNull<BlockData> EndstoneBlock::getData() const
{
    return std::make_shared<EndstoneBlockData>(getMinecraftBlock());
}

void EndstoneBlock::setData(const BlockData &data)
{
    return setData(std::move(data), true);
}

void EndstoneBlock::setData(const BlockData &data, bool apply_physics)
{
    const ::Block &block = static_cast<const EndstoneBlockData &>(data).getHandle();
    if (apply_physics) {
        getBlockSource().setBlock(block_pos_, block, ::BlockType::UPDATE_NEIGHBORS | ::BlockType::UPDATE_CLIENTS, nullptr,
                               nullptr);
    }
    else {
        getBlockSource().setBlock(block_pos_, block, ::BlockType::UPDATE_CLIENTS, nullptr, nullptr);  // NETWORK
    }
}

NotNull<Block> EndstoneBlock::getRelative(int offset_x, int offset_y, int offset_z)
{
    return getDimension()->getBlockAt(getX() + offset_x, getY() + offset_y, getZ() + offset_z);
}

NotNull<Block> EndstoneBlock::getRelative(BlockFace face)
{
    return getRelative(face, 1);
}

NotNull<Block> EndstoneBlock::getRelative(BlockFace face, int distance)
{
    return getRelative(EndstoneBlockFace::getOffsetX(face) * distance, EndstoneBlockFace::getOffsetY(face) * distance,
                       EndstoneBlockFace::getOffsetZ(face) * distance);
}

NotNull<Dimension> EndstoneBlock::getDimension() const
{
    return dimension_;
}

const Biome &EndstoneBlock::getBiome() const
{
    const auto &biome = getBlockSource().getBiome(block_pos_);
    return EndstoneServer::getInstance().getRegistry<Biome>().getOrThrow(biome.getFullName());
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
    return {getDimension(), getX(), getY(), getZ()};
}

NotNull<BlockState> EndstoneBlock::captureState() const
{
    if (auto *block_entity = getBlockSource().getBlockEntity(block_pos_)) {
        // TODO(block-state): once we add more type-specific block states (Sign, Furnace, CreatureSpawner, ...),
        // replace this switch with a BlockActorType -> factory registry (cf. CraftBukkit's CraftBlockStates),
        // keeping the generic getContainer() check as the fallback for container blocks.
        switch (block_entity->getType()) {
        case BlockActorType::ItemFrame:
        case BlockActorType::GlowItemFrame:
            return std::make_shared<EndstoneItemFrame>(*this, static_cast<ItemFrameBlockActor &>(*block_entity));
        case BlockActorType::Sign:
        case BlockActorType::HangingSign:
            return std::make_shared<EndstoneSign>(*this, static_cast<SignBlockActor &>(*block_entity));
        case BlockActorType::MobSpawner:
            return std::make_shared<EndstoneCreatureSpawner>(*this,
                                                             static_cast<MobSpawnerBlockActor &>(*block_entity));
        case BlockActorType::Campfire:
            return std::make_shared<EndstoneCampfire>(*this, static_cast<CampfireBlockActor &>(*block_entity));
        case BlockActorType::Lectern:
            return std::make_shared<EndstoneLectern>(*this, static_cast<LecternBlockActor &>(*block_entity));
        case BlockActorType::Furnace:
        case BlockActorType::BlastFurnace:
        case BlockActorType::Smoker:
            return std::make_shared<EndstoneFurnace>(*this, static_cast<FurnaceBlockActor &>(*block_entity));
        default:
            break;
        }
        if (static_cast<VanillaBlockActor *>(block_entity)->getContainer() != nullptr) {
            return std::make_shared<EndstoneContainer>(*this, *block_entity);
        }
    }
    return std::make_shared<EndstoneBlockState>(*this);
}

BlockSource &EndstoneBlock::getBlockSource() const
{
    return dimension_->getHandle().getBlockSourceFromMainChunkSource();
}

BlockPos EndstoneBlock::getPosition() const
{
    return block_pos_;
}

::Block &EndstoneBlock::getMinecraftBlock() const
{
    return const_cast<::Block &>(getBlockSource().getBlock(block_pos_));
}

NotNull<EndstoneBlock> EndstoneBlock::at(BlockSource &block_source, BlockPos block_pos)
{
    return std::make_shared<EndstoneBlock>(block_source, block_pos);
}
}  // namespace endstone::core
