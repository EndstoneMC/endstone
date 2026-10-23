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

#include "bedrock/world/level/block/furnace_block.h"

#include "bedrock/world/level/block/actor/block_actor.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/block_change_context.h"
#include "bedrock/world/level/block_source.h"

void FurnaceBlock::setLit(bool lit, BlockSource &region, const BlockPos &pos, BlockActorType block_actor_type,
                          const Block &unlit_furnace, const Block &lit_furnace)
{
    static const HashedString cardinal_direction("minecraft:cardinal_direction");

    const auto &block = region.getBlock(pos);
    const auto *block_actor = region.getBlockEntity(pos);
    if (block_actor == nullptr || block_actor->getType() != block_actor_type) {
        return;
    }

    const auto &furnace = (lit ? lit_furnace : unlit_furnace).getBlockType().getDefaultState();
    const Block *result = &furnace;
    if (block.getBlockType().hasState(cardinal_direction)) {
        if (const auto *turned = furnace.trySetState(cardinal_direction, block.getState<int>(cardinal_direction))) {
            result = turned;
        }
    }
    region.setBlock(pos, *result, BlockType::UPDATE_ALL, nullptr, BlockChangeContext());
}
