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

#include "bedrock/core/automatic_id.h"
#include "bedrock/core/memory.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/entity/entity_context.h"
#include "bedrock/forward.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/dimension/dimension_height_range.h"
#include "bedrock/world/level/dimension/dimension_interface.h"
#include "bedrock/world/level/level_listener.h"
#include "bedrock/world/level/saveddata/saved_data.h"

class Level;

class Dimension : public IDimension,
                  public LevelListener,
                  public SavedData,
                  public Bedrock::EnableNonOwnerReferences,
                  public std::enable_shared_from_this<Dimension> {
public:
    [[nodiscard]] Level &getLevel() const;
    [[nodiscard]] const std::string &getName() const;  // Endstone
    BlockSource &getBlockSourceFromMainChunkSource() const;

private:
    std::vector<ActorChunkTransferEntry> actor_chunk_transfer_queue_;           // +104 (+96)
    std::unordered_map<ChunkKey, std::vector<ActorUnloadedChunkTransferEntry>>  //
        actor_unloaded_chunk_transfer_queue_;                                   // +128 (+120)
    Level *level_;                                                              // +192 (+160)
    DimensionHeightRange height_range_;                                         // +200 (+168)
    BlockSource *block_source_;                                                 // +208 (+176)
    size_t pad_[14];                                                            //
    std::string name_;                                                          // +328 (+296)
    DimensionType id_;                                                          // +360 (+320)
};
