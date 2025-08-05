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

#include "bedrock/util/randomize.h"
#include "bedrock/world/level/block/block_type.h"
#include "bedrock/world/level/block_source.h"

class FireBlock : public BlockType {
public:
    static constexpr int NEIGHBOR_UPDATE_TICK_DELAY = 3;
    static constexpr int CARDINAL_DIRECTION_BURN_CHANCE = 300;
    static constexpr int VERTICAL_DIRECTION_BURN_CHANCE = 250;
    static constexpr int HUMID_BURN_CHANCE_OFFSET = 50;
    FireBlock(const std::string &, int);

    void checkBurn(BlockSource &region, const BlockPos &pos, int chance, Randomize &randomize, int age) const;

private:
    bool _trySpawnSoulFire(BlockSource &, const BlockPos &) const;
    void _tryEvictBeehive(BlockSource &region, const BlockPos &block_pos) const;
    void _tryAddToTickingQueue(BlockSource &, const BlockPos &, Random &) const;
    bool isValidFireLocation(BlockSource &region, const BlockPos &pos) const;
    float getFireOdds(BlockSource &region, const BlockPos &pos) const;
};
