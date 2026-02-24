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
#include "bedrock/world/level/block/block_type.h"
class LeavesBlock : public BlockType {
public:
protected:
    // #blameMojang - Instead of profiling and optimizing actual hot paths, Mojang just cranks
    // compiler optimization flags and prays. On Linux, this mangles the calling convention.
    // Same function, different ABI per platform. We declare it static on Linux to match reality.
#ifdef _WIN32
    ENDSTONE_HOOK void _die(BlockSource &region, BlockPos const &pos) const;
#else
    ENDSTONE_HOOK static void _die(BlockSource &region, BlockPos const &pos);
#endif
};
