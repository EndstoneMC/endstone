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

#include "bedrock/world/level/ticking/ticking_areas_manager.h"

#include "bedrock/symbol.h"

AddTickingAreaStatus TickingAreasManager::addArea(DimensionType dimension_id, const std::string &name,
                                                  const BlockPos &center, int radius, AreaLimitCheck limit_check,
                                                  bool is_persistent, TickingAreaLoadMode load_mode,
                                                  LevelStorage &level_storage)
{
    return BEDROCK_CALL(&TickingAreasManager::addArea, this, dimension_id, name, center, radius, limit_check,
                        is_persistent, load_mode, level_storage);
}

std::vector<TickingAreaDescription> TickingAreasManager::removePendingAreaByName(DimensionType dimension_id,
                                                                                 const std::string &name,
                                                                                 LevelStorage &level_storage)
{
    return BEDROCK_CALL(&TickingAreasManager::removePendingAreaByName, this, dimension_id, name, level_storage);
}
