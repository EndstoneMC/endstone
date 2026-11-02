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

#include <cstddef>
#include <functional>

#include "bedrock/bedrock.h"
#include "bedrock/world/level/block_pos.h"

class Actor;

class PortalRecord {
public:
    PortalRecord(const BlockPos &tp);
    PortalRecord(const BlockPos &tp, int span, int x_inc, int z_inc);
    [[nodiscard]] BlockPos closestBlockPosTo(const Actor &entity) const;
    [[nodiscard]] std::size_t hashCode() const;
    bool operator==(const PortalRecord &rhs) const { return base_block_pos == rhs.base_block_pos; }
    BlockPos base_block_pos;
    signed char span;
    signed char x_inc;
    signed char z_inc;
};
BEDROCK_STATIC_ASSERT_SIZE(PortalRecord, 16, 16);

template <>
struct std::hash<PortalRecord> {
    std::size_t operator()(const PortalRecord &key) const;
};
