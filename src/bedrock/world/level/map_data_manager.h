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

#include "bedrock/world/level/gameplay_user_manager_connector.h"
#include "bedrock/world/level/map_data_manager_options.h"
#include "bedrock/world/level/saveddata/map_item_saved_data.h"

class MapDataManager {
public:
    virtual ~MapDataManager() = default;
    virtual void registerOnGameplayUserAddedSubscription(IGameplayUserManagerConnector &);
    void registerOnSaveLevelDataSubscription(ILevelStorageManagerConnector &levelStorageManagerConnector);
    MapItemSavedData *getMapSavedData(const ActorUniqueID uuid);
    const size_t getMapDataMapSize() const;
    MapItemSavedData &createMapSavedData(const ActorUniqueID &uuid, const BlockPos &origin, DimensionType dimension,
                                         int returnScaleLevel);
    MapItemSavedData &createMapSavedData(const std::vector<ActorUniqueID> &mapIds, const BlockPos &origin,
                                         DimensionType dimension, int returnScaleLevel);
    virtual MapItemSavedData &createMapSavedData(const ActorUniqueID &uuid);
    virtual void requestMapInfo(const ActorUniqueID uuid, bool forceUpdate);
    bool copyAndLockMap(const ActorUniqueID originalMapUuid, const ActorUniqueID newMapUuid);
    ActorUniqueID expandMapByID(const ActorUniqueID uuid, bool wasInit);
    void setPacketSender(PacketSender &packetSender);
    void onStartLeaveGame();
    void tick();

protected:
    virtual void _copyAndLockMap(const ActorUniqueID originalMapUuid, const ActorUniqueID newMapUuid);
    ENDSTONE_HOOK MapItemSavedData *_loadMapData(const ActorUniqueID &uuid);
    void _onSaveLevelData(LevelStorage &levelStorage);
    DimensionManager &dimension_manager_;
    LevelStorage *level_storage_;
    gsl::not_null<std::unique_ptr<IMapDataManagerOptions>> map_data_manager_options_;
    std::function<ActorUniqueID()> get_new_unique_id_;
    Bedrock::PubSub::Subscription on_save_level_data_;
    std::unordered_map<ActorUniqueID, std::unique_ptr<MapItemSavedData>> map_data_;
    Bedrock::NonOwnerPointer<PacketSender> packet_sender_;
};
