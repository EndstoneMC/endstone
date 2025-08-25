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

#include <optional>
#include <string>

#include "bedrock/util/rotation.h"
#include "bedrock/world/level/block/block_type.h"
#include "bedrock/world/level/block_pos.h"

namespace BedBlockSpawnOffset {
static constexpr std::array<const BlockPos, 12UL> RESPAWN_OFFSETS_NORTH_LEFT = {{{-1, 0, 0},
                                                                                 {-1, 0, 1},
                                                                                 {-1, 0, 2},
                                                                                 {0, 0, 2},
                                                                                 {1, 0, 2},
                                                                                 {1, 0, 1},
                                                                                 {1, 0, 0},
                                                                                 {1, 0, -1},
                                                                                 {0, 0, -1},
                                                                                 {-1, 0, -1},
                                                                                 {0, 1, 0},
                                                                                 {0, 1, 1}}};
static constexpr std::array<const BlockPos, 12UL> RESPAWN_OFFSETS_NORTH_RIGHT = {{{1, 0, 0},
                                                                                  {1, 0, 1},
                                                                                  {1, 0, 2},
                                                                                  {0, 0, 2},
                                                                                  {-1, 0, 2},
                                                                                  {-1, 0, 1},
                                                                                  {-1, 0, 0},
                                                                                  {-1, 0, -1},
                                                                                  {0, 0, -1},
                                                                                  {1, 0, -1},
                                                                                  {0, 1, 0},
                                                                                  {0, 1, 1}}};
static constexpr Rotation RESPAWN_OFFSET_ROTATION_FROM_DIRECTION[4] = {Rotation::Rotate180, Rotation::Rotate270,
                                                                       Rotation::None, Rotation::Rotate90};
}  // namespace BedBlockSpawnOffset

class BedBlock : public BlockType {
public:
    BedBlock(const std::string &, int);
    static void setOccupied(BlockSource &, const BlockPos &, bool);
    static std::optional<BlockPos> findWakeupPosition(BlockSource &, const BlockPos &, const std::optional<Vec3> &);
    static bool isDangerousSpawnPosition(BlockSource &, const BlockPos &);
    static bool isValidStandUpPosition(BlockSource &, const BlockPos &);
};
