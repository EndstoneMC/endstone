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

#include "endstone/core/block/block_face.h"

#include <stdexcept>

namespace endstone::core {

int EndstoneBlockFace::getOffsetX(BlockFace face)
{
    switch (face) {
    case BlockFace::East:
        return 1;
    case BlockFace::West:
        return -1;
    default:
        return 0;
    }
}

int EndstoneBlockFace::getOffsetY(BlockFace face)
{
    switch (face) {
    case BlockFace::Up:
        return 1;
    case BlockFace::Down:
        return -1;
    default:
        return 0;
    }
}

int EndstoneBlockFace::getOffsetZ(BlockFace face)
{
    switch (face) {
    case BlockFace::South:
        return 1;
    case BlockFace::North:
        return -1;
    default:
        return 0;
    }
}

BlockFace EndstoneBlockFace::getOpposite(BlockFace face)
{
    switch (face) {
    case BlockFace::Down:
        return BlockFace::Up;
    case BlockFace::Up:
        return BlockFace::Down;
    case BlockFace::North:
        return BlockFace::South;
    case BlockFace::South:
        return BlockFace::North;
    case BlockFace::West:
        return BlockFace::East;
    case BlockFace::East:
        return BlockFace::West;
    default:
        throw std::invalid_argument("Invalid face");
    }
}
}  // namespace endstone::core
