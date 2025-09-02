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

#include "bedrock/world/item/item_instance.h"
#include "bedrock/world/level/block/actor/block_actor.h"

class CampfireBlockActor : public BlockActor {
public:
    static constexpr int MAX_COOKING_ITEMS = 4;
    CampfireBlockActor(const BlockPos &);

private:
    ENDSTONE_HOOK void _finishCooking(::BlockSource &region, int slot);

    ItemInstance cooking_item_[MAX_COOKING_ITEMS];
    int cooking_time_[MAX_COOKING_ITEMS];
    bool was_lit_;
    int next_smoke_particle_tick_;
};
