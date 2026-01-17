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

class LiquidBlockBase : public ::BlockType {};

class LiquidBlock : public LiquidBlockBase {
private:
    ENDSTONE_HOOK void _trySpreadTo(BlockSource &region, BlockPos const &pos, int neighbor,
                                    BlockPos const &flow_from_pos, FacingID flow_from_direction) const;
    bool _canSpreadTo(BlockSource &region, BlockPos const &pos, BlockPos const &flow_from_pos,
                      FacingID flow_from_direction) const;
    ENDSTONE_HOOK bool _isLiquidBlocking(BlockSource &region, BlockPos const &pos, BlockPos const &flow_from_pos,
                                         FacingID flow_from_direction) const;
};
