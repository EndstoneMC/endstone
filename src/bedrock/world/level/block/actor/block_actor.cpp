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

#include "bedrock/world/level/block/actor/block_actor.h"

#include "bedrock/symbol.h"

std::shared_ptr<BlockActor> BlockActor::loadStatic(const BlockType &block, const BlockPos &pos, ILevel &level,
                                                   const CompoundTag &tag, DataLoadHelper &data_load_helper)
{
    return BEDROCK_CALL(&BlockActor::loadStatic, block, pos, level, tag, data_load_helper);
}
