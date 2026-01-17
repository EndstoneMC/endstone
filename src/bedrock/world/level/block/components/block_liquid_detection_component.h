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

#include "bedrock/bedrock.h"
#include "bedrock/world/level/block/detection_rule.h"
#include "bedrock/world/level/block_source.h"

struct BlockLiquidDetectionComponent {
    DetectionRule water_detection_rule;
    static bool canContainLiquid(const Block &block);
    static bool isLiquidBlocking(const Block &block)
    {
        return block.getDirectData().getWaterDetectionRule().on_liquid_touches == BLOCKING;
    }
    static bool canBeDestroyedByLiquidSpread(const Block &block);
    static bool liquidSpreadCausesSpawn(const Block &block);
    static bool liquidCanFlowIntoFromDirection(const Block &block, FacingID flow_into_facing,
                                               const GetBlockFunction &get_block, const BlockPos &pos)
    {
        auto mask = block.getDirectData().getWaterDetectionRule().stop_flow_directions.stops_flow_directions_mask;
        if ((mask & Facing::FACINGMASK[flow_into_facing]) == Facing::FACINGMASK[flow_into_facing]) {
            return false;
        }
        return block.getBlockType().liquidCanFlowIntoFromDirection(flow_into_facing, get_block, pos);
    }
};
