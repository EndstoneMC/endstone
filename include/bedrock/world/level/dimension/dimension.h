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

#include <memory>
#include <unordered_map>
#include <vector>

#include "bedrock/automatic_id.h"
#include "bedrock/forward.h"
#include "bedrock/memory.h"
#include "bedrock/world/level/dimension/dimension_height_range.h"
#include "bedrock/world/level/dimension/dimension_interface.h"
#include "bedrock/world/level/level_listener.h"
#include "bedrock/world/level/storage/saved_data.h"

class Level;

class Dimension : public IDimension,
                  public LevelListener,
                  public SavedData,
                  public Bedrock::EnableNonOwnerReferences,
                  public std::enable_shared_from_this<Dimension> {
public:
    [[nodiscard]] Level &getLevel() const;

private:
    std::vector<void *> unknown_;                                                                 // +104
    std::unordered_map<ChunkKey, std::vector<ActorUnloadedChunkTransferEntry>> chunk_transfers_;  // +128
    Level &level_;                                                                                // +192 (+160)
    DimensionHeightRange height_range_;                                                           // +200 (+168)
};
