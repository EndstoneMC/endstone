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

#include "bedrock/world/level/dimension_manager.h"
#include "bedrock/world/level/gameplay_user_manager_connector.h"
#include "bedrock/world/level/map_data_manager_options.h"
#include "bedrock/world/level/saveddata/map_item_saved_data.h"

class MapDataManager {
public:
    MapDataManager(DimensionManager &dimension_manager, LevelStorage *level_storage,
                   std::unique_ptr<IMapDataManagerOptions> map_data_manager_options,
                   std::function<ActorUniqueID()> get_new_unique_id);
    virtual ~MapDataManager() = default;
    virtual void registerOnGameplayUserAddedSubscription(IGameplayUserManagerConnector &);
    void registerOnSaveLevelDataSubscription(ILevelStorageManagerConnector &level_storage_manager_connector);
    MapItemSavedData *getMapSavedData(const ActorUniqueID uuid);
    const size_t getMapDataMapSize() const;
    MapItemSavedData &createMapSavedData(const ActorUniqueID &uuid, const BlockPos &origin, DimensionType dimension,
                                         int return_scale_level);
    MapItemSavedData &createMapSavedData(const std::vector<ActorUniqueID> &map_ids, const BlockPos &origin,
                                         DimensionType dimension, int return_scale_level);
    virtual MapItemSavedData &createMapSavedData(const ActorUniqueID &uuid);
    virtual void requestMapInfo(const ActorUniqueID uuid, bool force_update);
    bool copyAndLockMap(const ActorUniqueID original_map_uuid, const ActorUniqueID new_map_uuid);
    ActorUniqueID expandMapByID(const ActorUniqueID uuid, bool was_init);
    void setPacketSender(PacketSender &packet_sender);
    void onStartLeaveGame();
    void tick();
    std::unordered_map<ActorUniqueID, std::unique_ptr<MapItemSavedData>> &getMapDataMap()  // Endstone
    {
        return map_data_;
    }

protected:
    virtual void _copyAndLockMap(const ActorUniqueID original_map_uuid, const ActorUniqueID new_map_uuid);
    ENDSTONE_HOOK MapItemSavedData *_loadMapData(const ActorUniqueID &uuid);
    void _onSaveLevelData(LevelStorage &level_storage);
    DimensionManager &dimension_manager_;
    LevelStorage *level_storage_;
    gsl::not_null<std::unique_ptr<IMapDataManagerOptions>> map_data_manager_options_;
    std::function<ActorUniqueID()> get_new_unique_id_;
    Bedrock::PubSub::Subscription on_save_level_data_;
    std::unordered_map<ActorUniqueID, std::unique_ptr<MapItemSavedData>> map_data_;
    Bedrock::NonOwnerPointer<PacketSender> packet_sender_;
};
