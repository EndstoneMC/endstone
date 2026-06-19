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

#include <string>
#include <vector>

#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/dimension/dimension_type.h"
#include "bedrock/world/level/ticking/ticking_area_description.h"

class LevelStorage;

enum class AddTickingAreaStatus : int {
    ExceededChunkLimit = 0,
    ExceededTickingAreaLimit = 1,
    ConflictingName = 2,
    UnknownDimension = 3,
    Success = 4,
    Undefined = 5,
};

// Reconstructed for the two call-only entry points Endstone needs (loadChunk / unloadChunk). Members are not
// reconstructed: we only ever hold a reference handed back by Level::getTickingAreasMgr() and call methods on it.
class TickingAreasManager {
public:
    enum class AreaLimitCheck : int {  // Endstone: private -> public
        None = 0,
        ActiveStandalone = 1,
        ActiveAndPendingStandalone = 2,
    };

    AddTickingAreaStatus addArea(DimensionType dimension_id, const std::string &name, const BlockPos &center, int radius,
                                 AreaLimitCheck limit_check, bool is_persistent, TickingAreaLoadMode load_mode,
                                 LevelStorage &level_storage);
    std::vector<TickingAreaDescription> removePendingAreaByName(DimensionType dimension_id, const std::string &name,
                                                                LevelStorage &level_storage);
};
