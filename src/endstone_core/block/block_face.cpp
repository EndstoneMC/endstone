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

#include "endstone/detail/block/block_face.h"

namespace endstone::detail {

int EndstoneBlockFace::getOffsetX(BlockFace face)
{
    switch (face) {
    case BlockFace::East:
    case BlockFace::NorthEast:
    case BlockFace::SouthEast:
        return 1;
    case BlockFace::West:
    case BlockFace::NorthWest:
    case BlockFace::SouthWest:
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
    case BlockFace::SouthEast:
    case BlockFace::SouthWest:
        return 1;
    case BlockFace::North:
    case BlockFace::NorthEast:
    case BlockFace::NorthWest:
        return -1;
    default:
        return 0;
    }
}
}  // namespace endstone::detail
