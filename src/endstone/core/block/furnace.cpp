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

#include "endstone/core/block/furnace.h"

#include "bedrock/world/level/block/furnace_block.h"
#include "endstone/core/block/block_data.h"

namespace endstone::core {

EndstoneFurnace::EndstoneFurnace(const EndstoneBlock &block, ::FurnaceBlockActor &furnace, bool use_snapshot)
    : EndstoneContainerBase<Furnace>(block, furnace, use_snapshot)
{
}

bool EndstoneFurnace::update(bool force, bool apply_physics)
{
    auto &region = getBlockSource();
    auto *furnace = static_cast<::FurnaceBlockActor *>(tryGetLiveBlockActor());
    const auto is_furnace = [furnace](const ::Block &block) {
        return &block.getBlockType() == &furnace->unlit_furnace_.getBlockType() ||
             &block.getBlockType() == &furnace->lit_furnace_.getBlockType();
    };
    const auto swaps_lit = furnace != nullptr && is_furnace(region.getBlock(block_pos_)) && is_furnace(*block_);
    if (swaps_lit) {
        furnace->no_drop_ = true;
    }
    const auto updated = EndstoneContainerBase::update(force, apply_physics);
    if (swaps_lit) {
        furnace->no_drop_ = false;
    }
    if (!updated) {
        return false;
    }

    furnace = static_cast<::FurnaceBlockActor *>(tryGetLiveBlockActor());
    if (furnace == nullptr) {
        return true;
    }
    const auto lit = furnace->getLitTime() > 0;
    furnace->no_drop_ = true;
    if (apply_physics) {
        ::FurnaceBlock::setLit(lit, region, block_pos_, furnace->getType(), furnace->unlit_furnace_,
                               furnace->lit_furnace_);
    }
    else {
        static const HashedString cardinal_direction("minecraft:cardinal_direction");
        const auto &current = region.getBlock(block_pos_);
        const auto &target = (lit ? furnace->lit_furnace_ : furnace->unlit_furnace_).getBlockType().getDefaultState();
        const ::Block *result = &target;
        if (current.getBlockType().hasState(cardinal_direction)) {
            if (const auto *turned =
                    target.trySetState(cardinal_direction, current.getState<int>(cardinal_direction))) {
                result = turned;
            }
        }
        getBlock()->setData(EndstoneBlockData(const_cast<::Block &>(*result)), false);
    }
    furnace->no_drop_ = false;
    block_ = const_cast<::Block *>(&region.getBlock(block_pos_));
    return true;
}

int EndstoneFurnace::getBurnTime() const
{
    return getFurnace().getLitTime();
}

void EndstoneFurnace::setBurnTime(int burn_time)
{
    getFurnace().setLitTime(burn_time);
    getFurnace().setChanged();
}

int EndstoneFurnace::getCookTime() const
{
    return getFurnace().getCookingProgress();
}

void EndstoneFurnace::setCookTime(int cook_time)
{
    getFurnace().setCookingProgress(cook_time);
    getFurnace().setChanged();
}

}  // namespace endstone::core
