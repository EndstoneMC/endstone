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

class IServerMapDataManagerConnector {
public:
    virtual Bedrock::PubSub::Connector<void(MapItemSavedData &)> &getOnCreateMapSavedDataConnector() = 0;
};

class ServerMapDataManager : public MapDataManager, public IServerMapDataManagerConnector {
public:
    ServerMapDataManager(DimensionManager &dimension_manager, LevelStorage *level_storage,
                         std::unique_ptr<IMapDataManagerOptions> map_data_manager_options,
                         std::function<ActorUniqueID()> get_new_unique_id);
    ~ServerMapDataManager() override = default;
    void registerOnGameplayUserAddedSubscription(
        IGameplayUserManagerConnector &gameplay_user_manager_connector) override;
    Bedrock::PubSub::Connector<void(MapItemSavedData &)> &getOnCreateMapSavedDataConnector() override;
    MapItemSavedData &createMapSavedData(const ActorUniqueID &uuid) override;
    void requestMapInfo(const ActorUniqueID, bool) override;

private:
    void _copyAndLockMap(const ActorUniqueID originalMapUuid, const ActorUniqueID newMapUuid) override;
    void _onGameplayUserAdded(EntityContext &entity);
    Bedrock::PubSub::Publisher<void(MapItemSavedData &), Bedrock::PubSub::ThreadModel::MultiThreaded>
        on_create_map_saved_data_;
    Bedrock::PubSub::Subscription on_gameplay_user_added_;
};
