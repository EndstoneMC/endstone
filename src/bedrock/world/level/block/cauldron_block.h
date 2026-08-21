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

#include "bedrock/bedrock.h"
#include "bedrock/world/level/block/actor_block.h"
#include "bedrock/world/level/block/block_event.h"

enum class CauldronLiquidType : int {
    Water = 0,
    Lava = 1,
    PowderSnow = 2,
    Count = 3,
};

class CauldronBlock : public ActorBlock {
public:
    static constexpr int MIN_FILL_LEVEL = 0;
    static constexpr int MAX_FILL_LEVEL = 6;

    ENDSTONE_HOOK void setLiquidLevel(BlockSource &, BlockPos const &, int, CauldronLiquidType) const;
    ENDSTONE_HOOK void tick(BlockEvents::BlockQueuedTickEvent &) const;
    ENDSTONE_HOOK void use(BlockEvents::BlockPlayerInteractEvent &event_data) const;
};
