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

#include <cstdint>
#include <optional>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/core/resource/pack_id_version.h"
#include "bedrock/core/threading/enable_queue_for_main_thread.h"
#include "bedrock/forward.h"
#include "bedrock/minecraft_app_interface.h"
#include "bedrock/network/connection_request.h"
#include "bedrock/network/disconnection_request_info.h"
#include "bedrock/network/net_event_callback.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/network_server_config.h"
#include "bedrock/network/server_network_system.h"
#include "bedrock/network/sub_client_connection_request.h"
#include "bedrock/network/xbox_live_user_observer.h"
#include "bedrock/platform/multiplayer_service_observer.h"
#include "bedrock/server/commands/minecraft_commands.h"
#include "bedrock/server/deny_list.h"
#include "bedrock/server/server_player.h"
#include "bedrock/world/events/server_network_events.h"
#include "bedrock/world/game_callbacks.h"
#include "bedrock/world/level/level_interface.h"
#include "bedrock/world/level/level_listener.h"

enum class ServerTextEvent : std::uint8_t {
    Sleeping = 0,
    Connection = 1,
    ChangedSkin = 2,
};

class ServerNetworkHandler : public Bedrock::Threading::EnableQueueForMainThread,
                             public NetEventCallback,
                             public LevelListener,
                             public Social::MultiplayerServiceObserver,
                             public Social::XboxLiveUserObserver {
    class Client {
    public:
        [[nodiscard]] ConnectionRequest const &getPrimaryRequest() const;
        [[nodiscard]] std::unordered_map<SubClientId, std::unique_ptr<SubClientConnectionRequest>> const &
        getSubClientRequests() const;

    private:
        std::unique_ptr<ConnectionRequest> primary_request_;
        std::unordered_map<SubClientId, std::unique_ptr<SubClientConnectionRequest>> sub_client_requests_;
    };

public:
    ServerNetworkHandler(GameCallbacks &, const Bedrock::NonOwnerPointer<ILevel> &, ServerNetworkSystem &,
                         PrivateKeyManager &, ServerLocator &, PacketSender &, AllowList &, PermissionsFile *,
                         const mce::UUID &, int, int, MinecraftCommands &, IMinecraftApp &,
                         const std::unordered_map<PackIdVersion, std::string> &, Scheduler &,
                         Bedrock::NonOwnerPointer<TextFilteringProcessor>, optional_ref<MinecraftGameTest>,
                         ServiceReference<AppConfigs>, ServiceReference<Social::MultiplayerServiceManager>,
                         NetworkServerConfig);

    ~ServerNetworkHandler() override = 0;
    ENDSTONE_HOOK IncomingPacketFilterResult allowIncomingPacketId(const NetworkIdentifierWithSubId &sender,
                                                                   MinecraftPacketIds packet_id,
                                                                   std::size_t packet_size) override;
    ENDSTONE_HOOK OutgoingPacketFilterResult allowOutgoingPacket(const std::vector<NetworkIdentifierWithSubId> &ids,
                                                                 const Packet &packet) override;

    ENDSTONE_HOOK void disconnectClient(NetworkIdentifier const &, SubClientId, Connection::DisconnectFailReason,
                                        std::string const &, std::optional<std::string>, bool);
    [[nodiscard]] int getMaxNumPlayers() const;
    int setMaxNumPlayers(int max_players);
    void updateServerAnnouncement();
    ENDSTONE_HOOK bool trytLoadPlayer(ServerPlayer &, ConnectionRequest const &);

private:
    friend class endstone::core::EndstoneServer;

    ENDSTONE_HOOK ServerPlayer &_createNewPlayer(NetworkIdentifier const &, SubClientConnectionRequest const &,
                                                 SubClientId);
    ServerPlayer *_getServerPlayer(const NetworkIdentifier &, SubClientId);
    [[nodiscard]] ENDSTONE_HOOK bool _isServerTextEnabled(ServerTextEvent const &) const;

    GameCallbacks &callbacks_;
    Bedrock::NonOwnerPointer<ILevel> level_;
    ServerNetworkSystem &network_;
    PrivateKeyManager &server_keys_;
    ServerLocator &server_locator_;
    gsl::not_null<PacketSender *> packet_sender_;
    bool use_allow_list_;
    AllowList &allow_list_;
    PermissionsFile *permissions_file_;
    DenyList server_deny_list_;
    NetworkServerConfig network_server_config_;
    bool has_displayed_pack_errors_;
    NetworkIdentifier my_id_;
    int max_chunk_radius_;
    MinecraftCommands &minecraft_commands_;
    IMinecraftApp &app_;
    Bedrock::NonOwnerPointer<TextFilteringProcessor> text_filtering_processor_;
    std::unique_ptr<ClientBlobCache::Server::ActiveTransfersManager> client_cache_manager_;
    std::unordered_map<std::uint64_t, std::string> server_storage_for_clients_connecting_attempt_;
    std::unique_ptr<ClassroomModeNetworkHandler> companion_handler_;
    Bedrock::Threading::Mutex validate_player_mutex_;
    bool allow_incoming_;
    std::unique_ptr<IServerNetworkController> server_network_controller_;
    std::string server_name_;
    std::vector<std::string> trusted_keys_;
    int max_num_players_;
    std::unordered_set<mce::UUID> known_emote_piece_id_lookup_;
    std::vector<mce::UUID> known_emote_piece_ids_;
    std::unordered_map<std::uint64_t, std::unordered_map<std::string, std::shared_ptr<ResourcePackFileUploadManager>>>
        resource_upload_managers_;
    std::unique_ptr<TaskGroup> io_task_group_;
    std::unordered_map<NetworkIdentifier, std::unique_ptr<Client>> clients_;  // +960
    bool is_trial_;                                                           // +1024
    std::unordered_map<PackIdVersion, std::string> pack_id_to_content_key_;   // +1032
};
