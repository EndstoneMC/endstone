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

#include "bedrock/core/utility/automatic_id.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/forward.h"
#include "bedrock/gamerefs/owner_ptr.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/dimension/dimension_height_range.h"
#include "bedrock/world/level/level_listener.h"
#include "bedrock/world/level/saveddata/saved_data.h"
#include "endstone/level/dimension.h"

class Level;

class IDimension {
public:
    virtual ~IDimension() = 0;
    [[nodiscard]] virtual bool isNaturalDimension() const = 0;
    [[nodiscard]] virtual DimensionType getDimensionId() const = 0;
    // ...
};

class Dimension : public IDimension,
                  public LevelListener,
                  public SavedData,
                  public Bedrock::EnableNonOwnerReferences,
                  public EnableGetWeakRef<Dimension>,
                  public std::enable_shared_from_this<Dimension> {
public:
    [[nodiscard]] Level &getLevel() const;
    BlockSource &getBlockSourceFromMainChunkSource() const;

    [[nodiscard]] const std::string &getName() const;                 // Endstone
    [[nodiscard]] endstone::Dimension &getEndstoneDimension() const;  // Endstone

private:
    std::vector<ActorChunkTransferEntry> actor_chunk_transfer_queue_;                 // +104 (+96)
    std::unordered_map<ChunkKey, std::vector<ActorUnloadedChunkTransferEntry>>        //
        actor_unloaded_chunk_transfer_queue_;                                         // +128 (+120)
    Level *level_;                                                                    // +192 (+160)
    DimensionHeightRange height_range_;                                               // +200 (+168)
    OwnerPtr<BlockSource> block_source_;                                              // +208 (+176)
    float mobs_per_chunk_surface_[7];                                                 // +224
    float mobs_per_chunk_underground_[7];                                             // +252
    BrightnessPair default_brightness_;                                               // +280
    std::unique_ptr<BaseLightTextureImageBuilder> base_light_texture_image_builder_;  // +288
    std::unique_ptr<DimensionBrightnessRamp> dimension_brightness_ramp_;              // +296
    std::shared_ptr<LevelChunkMetaData> target_metadata_;                             // +304
    std::unique_ptr<RuntimeLightingManager> runtime_lighting_manager_;                // +320
    std::string name_;                                                                // +328 (+296)
    DimensionType id_;                                                                // +360 (+320)
    // ...
};
