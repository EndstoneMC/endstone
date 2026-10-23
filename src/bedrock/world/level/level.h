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

#include "bedrock/world/level/block_source_listener.h"
#include "bedrock/world/level/gameplay_user_manager.h"
#include "bedrock/world/level/level_interface.h"
#include "bedrock/world/level/world_registries_provider.h"

namespace endstone::core {
class EndstoneServer;
}

class Level : public ILevel, public BlockSourceListener, public IWorldRegistriesProvider {
public:
    ~Level() override;
    [[nodiscard]] ActorDefinitionGroup *getEntityDefinitions() const override;
    virtual ArmorTrimUnloader *getArmorTrimUnloader() = 0;
    [[nodiscard]] const BiomeRegistry &getBiomeRegistry() const override;
    BiomeRegistry &getBiomeRegistry() override;
    [[nodiscard]] const BlockPalette &getBlockPalette() const override;
    BlockPalette &getBlockPalette() override;
    [[nodiscard]] const FeatureRegistry &getFeatureRegistry() const override;
    FeatureRegistry &getFeatureRegistry() override;
    [[nodiscard]] const FeatureTypeFactory &getFeatureTypeFactory() const override;
    FeatureTypeFactory &getFeatureTypeFactory() override;
    [[nodiscard]] const JigsawStructureRegistry &getJigsawStructureRegistry() const override;
    JigsawStructureRegistry &getJigsawStructureRegistry() override;
    [[nodiscard]] const StructureSpawnRegistry &getStructureSpawnRegistry() const override;
    StructureSpawnRegistry &getStructureSpawnRegistry() override;
    [[nodiscard]] const Bedrock::NotNullNonOwnerPtr<StructureManager> getStructureManager() const override;
    Bedrock::NotNullNonOwnerPtr<StructureManager> getStructureManager() override;
    [[nodiscard]] const BiomeComponentFactory &getBiomeComponentFactory() const override;
    BiomeComponentFactory &getBiomeComponentFactory() override;
    [[nodiscard]] const SurfaceBuilderRegistry &getSurfaceBuilderRegistry() const override;
    SurfaceBuilderRegistry &getSurfaceBuilderRegistry() override;
    [[nodiscard]] const DimensionOwnerFactory &getDimensionFactory() const override;
    DimensionOwnerFactory &getDimensionFactory() override;
    ENDSTONE_HOOK void tick() override;
    // ENDSTONE_HOOK void onChunkDiscarded(LevelChunk &);
    // ENDSTONE_HOOK void onChunkLoaded(ChunkSource &, LevelChunk &);
    [[nodiscard]] virtual const PlayerSleepManager &getPlayerSleepManager() const = 0;
    virtual PlayerSleepManager &getPlayerSleepManager() = 0;
    [[nodiscard]] ENDSTONE_HOOK int getActivePlayerCount() const override;
    void onSourceCreated(BlockSource &) override;
    void onSourceDestroyed(BlockSource &) override;

protected:
    virtual void _subTick() = 0;

public:
    static bool isUsableLevel(const ILevel &level);

protected:
    virtual void _initializeMapDataManager() = 0;
    friend class endstone::core::EndstoneServer;
};
