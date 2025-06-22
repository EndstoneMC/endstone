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
#include <vector>

#include "bedrock/core/utility/buffer_span.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/dimension/dimension_type.h"
#include "bedrock/world/level/saveddata/maps/map_decoration.h"
#include "bedrock/world/level/saveddata/maps/map_item_tracked_actor.h"
#include "bedrock/world/level/storage/level_storage.h"

struct ClientTerrainPixel {
    std::uint32_t pixel;
    bool assigned_to_map;
    bool sent_to_server;
};
static_assert(sizeof(ClientTerrainPixel) == 8);

namespace endstone::core {
class EndstoneMapView;
}

class MapItemSavedData {
public:
    struct ChunkBounds {
        uint32_t x0;
        uint32_t z0;
        uint32_t x1;
        uint32_t z1;
    };

private:
    using DecorationCollection = std::vector<std::pair<MapItemTrackedActor::UniqueId, std::shared_ptr<MapDecoration>>>;

public:
    MapItemSavedData(ActorUniqueID, bool);
    void setOrigin(Vec3 origin, int scale, DimensionType dimension, bool is_legacy_level, bool is_centered,
                   const BlockPos &world_center);
    void setScale(int);
    void setDimensionId(DimensionType);
    void setScaleAndParentMapId(int, ActorUniqueID);
    [[nodiscard]] int getScale() const;
    [[nodiscard]] const BlockPos &getOrigin() const;
    [[nodiscard]] DimensionType getDimensionId() const;
    [[nodiscard]] const DecorationCollection &getDecorations() const;
    void enableUnlimitedTracking();
    void deserialize(const CompoundTag &);
    void serialize(CompoundTag &) const;
    void save(LevelStorage &);
    void trySave(LevelStorage &);
    void tickCarriedBy(Actor &, const CompoundTag *);
    void tickByBlock(const BlockPos &, BlockSource &);
    std::shared_ptr<MapItemTrackedActor> addTrackedMapEntity(Actor &, MapDecoration::Type);
    std::shared_ptr<MapItemTrackedActor> addTrackedMapEntity(const BlockPos &, BlockSource &, MapDecoration::Type);
    void removeTrackedMapEntity(Actor &);
    void removeTrackedMapEntity(const BlockPos &);
    [[nodiscard]] std::unique_ptr<Packet> getUpdatePacket(const ItemStack &, Level &, Actor &) const;
    [[nodiscard]] std::unique_ptr<Packet> getUpdatePacket(Level &, const BlockPos &) const;
    [[nodiscard]] std::unique_ptr<Packet> getFullDataPacket() const;
    void setLocked();
    [[nodiscard]] bool isLocked() const;
    void setDirtyForSave();
    [[nodiscard]] bool isDirtyForSave() const;
    void setDirtyForSaveAndPixelData();
    void setPixelDirty(uint32_t, uint32_t);
    void setAllPixelsDirty();
    bool setPixel(uint32_t, uint32_t, uint32_t);
    [[nodiscard]] bool isChunkAllEmpty(ChunkBounds) const;
    void setMapSection(buffer_span<unsigned int>, ChunkBounds);
    [[nodiscard]] ActorUniqueID getMapId() const;
    std::shared_ptr<MapItemTrackedActor> getTrackedMapEntity(Actor &);
    std::shared_ptr<MapItemTrackedActor> getTrackedMapEntity(const BlockPos &, BlockSource &);
    void replaceDecorations(std::vector<std::shared_ptr<MapDecoration>>,
                            const std::vector<MapItemTrackedActor::UniqueId> &);
    void replacePixels(buffer_span<unsigned int>, uint32_t, uint32_t, uint32_t, uint32_t);
    void addDecoration(const MapItemTrackedActor::UniqueId &, const std::shared_ptr<MapDecoration> &);
    static bool pointInMapBounds(float, float);
    [[nodiscard]] ActorUniqueID getParentMapId() const;
    [[nodiscard]] bool hasParentMap() const;
    [[nodiscard]] bool isAdjacent(const MapItemSavedData &, int) const;
    [[nodiscard]] bool isFullyExplored() const;
    [[nodiscard]] bool isPreviewIncomplete() const;
    void _setPreviewIncomplete(bool);
    void setPixelDataClean();
    [[nodiscard]] bool isPixelDataDirty() const;
    [[nodiscard]] buffer_span<unsigned int> getPixels() const;
    std::vector<ClientTerrainPixel> &getClientPixels();
    void copyMapData(const MapItemSavedData &);
    static AABB getMapWorldBounds(const BlockPos &, int, int);

    void setClientPixelsDirty(bool);
    [[nodiscard]] bool areClientPixelsDirty() const;
    SpinLock *getClientSamplingLock();
    [[nodiscard]] bool needsResampling() const;
    void checkNeedsResampling();

    size_t update_interval;

private:
    friend class endstone::core::EndstoneMapView;

    ActorUniqueID map_id_;
    ActorUniqueID parent_map_id_;
    bool is_fully_explored_;
    bool preview_incomplete_;
    BlockPos origin_;
    DimensionType dimension_;
    std::int8_t scale_;
    std::vector<unsigned int> pixels_;
    std::vector<ClientTerrainPixel> client_pixels_;
    std::vector<std::shared_ptr<MapItemTrackedActor>> tracked_entities_;
    bool unlimited_tracking_;
    bool dirty_for_save_;
    bool dirty_pixel_data_;
    bool locked_;
    DecorationCollection decorations_;
    bool has_dirty_client_pixels_;
    std::unique_ptr<SpinLockImpl> client_sampling_lock_;
    bool needs_resampling_;
    bool is_dlc_world_;
};
static_assert(sizeof(MapItemSavedData) == 176);
