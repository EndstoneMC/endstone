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

#include "bedrock/bedrock.h"
#include "bedrock/certificates/identity/game_server_token.h"
#include "bedrock/network/server_network_system.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/inventory/inventory_menu.h"

class ServerPlayer : public Player {
public:
    using OnPlayerLoadedCallback = std::function<void(ServerPlayer &)>;

    ServerPlayer(Level &, PacketSender &, ServerNetworkSystem &, ClientBlobCache::Server::ActiveTransfersManager &,
                 GameType, bool, const NetworkIdentifier &, SubClientId, OnPlayerLoadedCallback, mce::UUID,
                 const std::string &, const std::string &, const GameServerToken &, int, bool, EntityContext &,
                  PlatformType, InputMode, SyncedClientOptionsComponent);
    ~ServerPlayer() override = 0;
    ENDSTONE_HOOK void disconnect();
    void setLocalPlayerAsInitialized();

protected:
    PlatformType platform_type_;

private:
    ServerNetworkSystem &network_;
    OnPlayerLoadedCallback on_player_loaded_callback_;
    InventoryMenu inventory_menu_;
    ContainerID container_counter_;
    uint32_t max_chunk_radius_;
    bool is_initial_player_load_happening_;
    bool is_teacher_;
    bool local_player_initialized_;  // +3498
    bool waiting_for_ticking_areas_preload_;
    Tick prev_shield_blocking_tick_;
    uint32_t client_view_radius_;
    uint32_t client_requested_radius_;
    bool is_compatible_with_client_side_chunk_gen_;
    int remaining_structure_refresh_ticks_;
    StructureFeatureType current_structure_feature_;
    int last_known_sync_tick_;
};
