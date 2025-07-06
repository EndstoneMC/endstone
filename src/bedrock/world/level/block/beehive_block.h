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

#include "bedrock/world/level/block/actor/beehive_block_actor.h"
#include "bedrock/world/level/block/actor/block_actor.h"
#include "bedrock/world/level/block/face_directional_actor_block.h"
#include "bedrock/world/level/block_source.h"

class BeehiveBlock : public FaceDirectionalActorBlock {
public:
    BeehiveBlock(const std::string &, int);
    void evictAll(BlockSource &region, const BlockPos &pos, bool angry) const
    {
        auto *block_actor = const_cast<BlockActor *>(region.getBlockEntity(pos));
        if (!block_actor) {
            return;
        }
        if (block_actor->getType() == BeehiveBlockActor::TypeId) {
            static_cast<BeehiveBlockActor *>(block_actor)->evictAll(region, angry);
        }
    }
};
