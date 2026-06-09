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

#include <cstdint>

#include "bedrock/shared_types/height.h"
#include "bedrock/world/level/block_pos.h"

class ChunkLocalHeight {
public:
    ChunkLocalHeight() = default;
    explicit ChunkLocalHeight(Height value) : value_(value) {}
    [[nodiscard]] Height getVal() const { return value_; }

private:
    Height value_;
};
static_assert(sizeof(ChunkLocalHeight) == 2);

class ChunkBlockPos {
public:
    ChunkBlockPos(const BlockPos &pos, Height min_dimension_height)
        : x(pos.x & 0xF), z(pos.z & 0xF), y(static_cast<Height>(pos.y - min_dimension_height))
    {
    }

    std::uint8_t x;
    std::uint8_t z;
    ChunkLocalHeight y;
};
static_assert(sizeof(ChunkBlockPos) == 4);
