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
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/level.h"

namespace BlockEvents {
enum class EventType : uint8_t {
    EntityFallOn = 0,
    Place = 1,
    StateChange = 2,
    PlayerDestroy = 3,
    PlayerInteract = 4,
    PlayerPlacing = 5,
    QueuedTick = 6,
    RandomTick = 7,
    RandomTickLegacy = 8,
    StepOff = 9,
    StepOn = 10,
    IsValidSpawn = 11,
    BlockBreak = 12,
    RedstoneUpdate = 13,
    Actor = 14,
    InternalActor = 15,
    _Count = 16,
};

class BlockEventBase {
public:
    virtual ~BlockEventBase();
    BlockEventBase(const BlockPos pos) : pos(pos) {}
    virtual const Block &getBlock() const { return getBlockSource().getBlock(pos); }
    virtual const BlockSource &getBlockSource() const = 0;
    const BlockPos pos;
};

class BlockRandomTickEvent : public BlockEventBase {
public:
    static constexpr EventType Type = EventType::RandomTick;
    BlockRandomTickEvent(BlockSource &region, const BlockPos pos, Random &random)
        : BlockEventBase(pos), region(region), random(random)
    {
    }
    [[nodiscard]] const BlockSource &getBlockSource() const override { return region; }
    BlockSource &region;
    Random &random;
};
}  // namespace BlockEvents
