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
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/forward.h"
#include "bedrock/gamerefs/owner_ptr.h"
#include "bedrock/network/packet.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/biome/registry/biome_registry.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/dimension/dimension_height_range.h"
#include "bedrock/world/level/level_listener.h"
#include "bedrock/world/level/levelgen/v2/providers/int_provider.h"
#include "bedrock/world/level/saveddata/saved_data.h"
#include "endstone/level/dimension.h"

class Level;

class IDimension {
public:
    virtual ~IDimension() = 0;
    [[nodiscard]] virtual bool isNaturalDimension() const = 0;
    virtual AutomaticID<Dimension, int> getDimensionId() = 0;
    virtual void sendPacketForPosition(BlockPos const &, Packet const &, Player const *) = 0;
    virtual void sendPacketForEntity(Actor const &, Packet const &, Player const *) = 0;
    virtual void flushLevelChunkGarbageCollector() = 0;
    virtual void initializeWithLevelStorageManagerConnector(ILevelStorageManagerConnector &) = 0;
    virtual BiomeRegistry &getBiomeRegistry() = 0;
    [[nodiscard]] virtual BiomeRegistry const &getBiomeRegistry() const = 0;
    virtual Vec3 translatePosAcrossDimension(Vec3 const &, AutomaticID<Dimension, int>) = 0;
    virtual void forEachPlayer(std::function<bool(Player &)>) = 0;
    virtual Actor *fetchEntity(ActorUniqueID, bool) = 0;
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
    std::vector<ActorChunkTransferEntry> actor_chunk_transfer_queue_;                 // +104
    std::unordered_map<ChunkKey, std::vector<ActorUnloadedChunkTransferEntry>>        //
        actor_unloaded_chunk_transfer_queue_;                                         // +128
    Level *level_;                                                                    // +192
    DimensionHeightRange height_range_;                                               // +200
    std::int16_t sea_level_;                                                          // +204
    std::uint8_t monster_spawn_block_light_limit_;                                    // +206
    IntProvider monster_spawn_light_test_;                                            // +208
    OwnerPtr<BlockSource> block_source_;                                              // +240
    float mobs_per_chunk_surface_[7];                                                 // +256
    float mobs_per_chunk_underground_[7];                                             // +284
    BrightnessPair default_brightness_;                                               // +312
    std::unique_ptr<BaseLightTextureImageBuilder> base_light_texture_image_builder_;  // +320
    std::unique_ptr<DimensionBrightnessRamp> dimension_brightness_ramp_;              // +328
    std::shared_ptr<LevelChunkMetaData> target_metadata_;                             // +344
    std::unique_ptr<RuntimeLightingManager> runtime_lighting_manager_;                // +352
    std::string name_;                                                                // +360
    DimensionType id_;                                                                // +392
    // ...
};
