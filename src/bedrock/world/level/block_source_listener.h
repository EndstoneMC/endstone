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

#include "bedrock/world/level/block_pos.h"

enum class BlockChangedEventTarget : std::uint8_t {
    SelfBlock = 0,
    NeighborBlock = 1,
};

class BlockSource;

class BlockSourceListener {
public:
    virtual ~BlockSourceListener() = 0;
    virtual void onSourceCreated(BlockSource &) = 0;
    virtual void onSourceDestroyed(BlockSource &) = 0;
    virtual void onAreaChanged(BlockSource &, const BlockPos &, const BlockPos &) = 0;
    virtual void onBlockChanged(BlockSource &, const BlockPos &, uint32_t, const Block &, const Block &, int,
                                const ActorBlockSyncMessage *, BlockChangedEventTarget, Actor *) = 0;
    virtual void onBrightnessChanged(BlockSource &, const BlockPos &) = 0;
    virtual void onBlockEntityChanged(BlockSource &, BlockActor &) = 0;
    virtual void onEntityChanged(BlockSource &, Actor &) = 0;
    virtual void onBlockEvent(BlockSource &, int, int, int, int, int) = 0;
};
