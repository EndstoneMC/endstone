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

#include "bedrock/world/level/block/fire_block.h"

#include "bedrock/world/level/block/beehive_block.h"
#include "bedrock/world/level/block/vanilla_block_type_ids.h"

void FireBlock::_tryEvictBeehive(BlockSource &region, const BlockPos &pos) const
{
    auto &block = region.getBlock(pos);
    if (block.getName() == VanillaBlockTypeIds::Beehive || block.getName() == VanillaBlockTypeIds::BeeNest) {
        static_cast<const BeehiveBlock &>(block.getBlockType()).evictAll(region, pos, false);
    }
}

bool FireBlock::isValidFireLocation(BlockSource &region, const BlockPos &pos) const
{
    return getFireOdds(region, pos) > 0;
}

float FireBlock::getFireOdds(BlockSource &region, const BlockPos &pos) const
{
    if (const auto odds = region.getBlock(pos.east()).getFlameOdds(); odds > 0) {
        return static_cast<float>(odds);
    }
    if (const auto odds = region.getBlock(pos.west()).getFlameOdds(); odds > 0) {
        return static_cast<float>(odds);
    }
    if (const auto odds = region.getBlock(pos.below()).getFlameOdds(); odds > 0) {
        return static_cast<float>(odds);
    }
    if (const auto odds = region.getBlock(pos.above()).getFlameOdds(); odds > 0) {
        return static_cast<float>(odds);
    }
    if (const auto odds = region.getBlock(pos.north()).getFlameOdds(); odds > 0) {
        return static_cast<float>(odds);
    }
    if (const auto odds = region.getBlock(pos.south()).getFlameOdds(); odds > 0) {
        return static_cast<float>(odds);
    }
    return 0;
}
