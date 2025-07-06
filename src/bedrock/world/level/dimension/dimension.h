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

#include "bedrock/forward.h"
#include "bedrock/gamerefs/owner_ptr.h"
#include "bedrock/network/packet.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/biome/registry/biome_registry.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/chunk/chunk_source.h"
#include "bedrock/world/level/dimension/dimension_height_range.h"
#include "bedrock/world/level/dimension/dimension_type.h"
#include "bedrock/world/level/level_listener.h"
#include "bedrock/world/level/levelgen/v2/providers/int_provider.h"
#include "bedrock/world/level/saveddata/saved_data.h"
#include "bedrock/world/level/weather.h"
#include "bedrock/world/redstone/circuit/circuit_system.h"

class Level;

class IDimension {
public:
    virtual ~IDimension() = 0;
    [[nodiscard]] virtual bool isNaturalDimension() const = 0;
    virtual AutomaticID<Dimension, int> getDimensionId() = 0;
    virtual void sendPacketForPosition(BlockPos const &, Packet const &, Player const *) = 0;
    virtual void sendPacketForEntity(Actor const &actor, Packet const &packet, Player const *except) = 0;
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
    Dimension(ILevel &, DimensionType, DimensionHeightRange, Scheduler &, std::string);

    [[nodiscard]] bool isBrightOutside() const;
    [[nodiscard]] std::string getLocalizationKey() const;
    [[nodiscard]] Level &getLevel() const;
    [[nodiscard]] ChunkSource &getChunkSource() const;
    [[nodiscard]] BlockSource &getBlockSourceFromMainChunkSource() const;
    Weather &getWeather() const;
    CircuitSystem &getCircuitSystem();
    [[nodiscard]] bool isRedstoneTick() const;
    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] endstone::Dimension &getEndstoneDimension() const;  // Endstone

private:
    std::vector<ActorChunkTransferEntry> actor_chunk_transfer_queue_;  // +104
    std::unordered_map<ChunkKey, std::vector<ActorUnloadedChunkTransferEntry>> actor_unloaded_chunk_transfer_queue_;
    Level *level_;
    DimensionHeightRange height_range_;
    std::int16_t sea_level_;
    std::uint8_t monster_spawn_block_light_limit_;
    IntProvider monster_spawn_light_test_;
    OwnerPtr<BlockSource> block_source_;  // +240
    bool has_weather_;                    // +256
    float mobs_per_chunk_surface_[7];
    float mobs_per_chunk_underground_[7];
    BrightnessPair default_brightness_;  // +316
    std::unique_ptr<BaseLightTextureImageBuilder> base_light_texture_image_builder_;
    std::unique_ptr<DimensionBrightnessRamp> dimension_brightness_ramp_;
    std::shared_ptr<LevelChunkMetaData> target_metadata_;
    std::unique_ptr<RuntimeLightingManager> runtime_lighting_manager_;
    std::string name_;  // +360
    DimensionType id_;  // +392
    bool ultra_warm_;   // +396
    bool has_ceiling_;
    bool has_skylight_;
    Brightness sky_darken_;
    std::unique_ptr<BlockEventDispatcher> dispatcher_;  // +400
    std::unique_ptr<TaskGroup> task_group_;
    std::unique_ptr<TaskGroup> chunk_gen_task_group_;
    std::unique_ptr<PostprocessingManager> post_processing_manager_;
    std::unique_ptr<SubChunkInterlocker> sub_chunk_interlocker_;
    std::unique_ptr<ChunkSource> chunk_source_;
    WorldGenerator *world_generator_;
    std::unique_ptr<Weather> weather_;
    std::unique_ptr<Seasons> seasons_;
    std::unique_ptr<GameEventDispatcher> game_event_dispatcher_;
    std::unique_ptr<CircuitSystem> circuit_system_;
    const int CIRCUIT_TICK_RATE;
    int circuit_system_tick_rate_;
};
