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
#include "bedrock/network/packet/login_packet.h"
#include "bedrock/network/server_network_system.h"
#include "bedrock/network/sub_client_connection_request.h"
#include "bedrock/network/xbox_live_user_observer.h"
#include "bedrock/platform/multiplayer_service_observer.h"
#include "bedrock/server/commands/minecraft_commands.h"
#include "bedrock/server/deny_list.h"
#include "bedrock/server/server_player.h"
#include "bedrock/server/server_text_settings.h"
#include "bedrock/world/events/server_network_events.h"
#include "bedrock/world/game_callbacks.h"
#include "bedrock/world/level/level_interface.h"
#include "bedrock/world/level/level_listener.h"

class ServerNetworkHandler : public Bedrock::Threading::EnableQueueForMainThread,
                             public NetEventCallback,
                             public LevelListener,
                             public Social::MultiplayerServiceObserver,
                             public Social::XboxLiveUserObserver {
public:
    ServerNetworkHandler(GameCallbacks &, const Bedrock::NonOwnerPointer<ILevel> &, ServerNetworkSystem &,
                         PrivateKeyManager &, ServerLocator &, PacketSender &, AllowList &, PermissionsFile *,
                         const mce::UUID &, int, int, MinecraftCommands &, IMinecraftApp &,
                         const std::unordered_map<PackIdVersion, std::string> &, Scheduler &,
                         Bedrock::NonOwnerPointer<TextFilteringProcessor>, optional_ref<MinecraftGameTest>,
                         ServiceReference<AppConfigs>, ServiceReference<Social::MultiplayerServiceManager>,
                         NetworkServerConfig);

    ~ServerNetworkHandler() override = 0;

    ENDSTONE_HOOK void disconnectClientWithMessage(const NetworkIdentifier &id, SubClientId sub_id,
                                                   Connection::DisconnectFailReason disconnect_reason,
                                                   const std::string &message,
                                                   std::optional<std::string> filtered_message, bool skip_message);
    [[nodiscard]] int getMaxNumPlayers() const;
    int setMaxNumPlayers(int max_players);
    void updateServerAnnouncement();
    const ConnectionRequest &fetchConnectionRequest(const NetworkIdentifier &source);
    [[nodiscard]] PlayerAuthenticationInfo fetchPlayerAuthenticationInfo(const NetworkIdentifier &source);
    ENDSTONE_HOOK bool tryToLoadPlayer(ServerPlayer &, ConnectionRequest const &,
                                      const PlayerAuthenticationInfo &player_info);

    ServerPlayer *getServerPlayer(const NetworkIdentifier &, SubClientId);  // Endstone
    void disconnect(NetworkIdentifier const &network_id, SubClientId sub_client_id,
                    std::string const &reason);  // Endstone

private:
    friend class endstone::core::EndstoneServer;
    friend class NetworkConnection;
    friend class NetworkSystem;
    ENDSTONE_HOOK virtual std::optional<PlayerAuthenticationInfo> _validateLoginPacket(const NetworkIdentifier &source,
                                                                                       const LoginPacket &packet);
    // TODO: do not hook this method, listen for packet and then try get from sub player lists
    ServerPlayer &_createNewPlayer(NetworkIdentifier const &source,
                                                 SubClientConnectionRequest const &connection_request,
                                                 const PlayerAuthenticationInfo &player_info, SubClientId subid);
    [[nodiscard]] ENDSTONE_HOOK bool _isServerTextEnabled(ServerTextEvent const &) const;

protected:
    class Client {
    public:
        [[nodiscard]] ConnectionRequest const &getPrimaryRequest() const;
        PlayerAuthenticationInfo getPrimaryPlayerInfo() const;
        const std::unordered_map<SubClientId, PlayerAuthenticationInfo> &getSubClientsPlayerInfo() const;
        void addSubClientPlayerInfo(SubClientId subClientId, PlayerAuthenticationInfo playerInfo);
        void removeSubClientPlayerInfo(SubClientId subClientId);

    private:
        std::unique_ptr<ConnectionRequest> primary_request_;
        PlayerAuthenticationInfo primary_player_info_;
        std::unordered_map<SubClientId, PlayerAuthenticationInfo> sub_client_player_info_;
    };
    std::unordered_map<NetworkIdentifier, std::unique_ptr<Client>> clients_;  // +80

private:
    GameCallbacks &callbacks_;  // +144
    Bedrock::NonOwnerPointer<ILevel> level_;
    ServerNetworkSystem &network_;
    PrivateKeyManager &server_keys_;
    ServerLocator &server_locator_;
    gsl::not_null<PacketSender *> packet_sender_;  // +200
    bool use_allow_list_;
    AllowList &allow_list_;
    PermissionsFile *permissions_file_;
    DenyList server_deny_list_;
    NetworkServerConfig network_server_config_;
    bool has_displayed_pack_errors_;
    std::shared_ptr<ScriptPackSettingsCache> pack_settings_cache_;
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
    int max_num_players_;  // +872
    std::unordered_set<mce::UUID> known_emote_piece_id_lookup_;
    std::vector<mce::UUID> known_emote_piece_ids_;
    std::unordered_map<std::uint64_t, std::unordered_map<std::string, std::shared_ptr<ResourcePackFileUploadManager>>>
        resource_upload_managers_;
    gsl::not_null<std::shared_ptr<Bedrock::Threading::SharedAsync<void>>> previous_upload_;
    gsl::not_null<std::unique_ptr<ResourcePackPathLifetimeHelpers::ResourcePackPathCache>> resource_pack_path_cache_;
    gsl::not_null<std::unique_ptr<ServerConnectionAuthValidator>> connection_auth_validator_;
    gsl::not_null<std::unique_ptr<TaskGroup>> async_join_task_group_;
    gsl::not_null<std::unique_ptr<AsyncJoinTaskManager>> async_join_task_manager_;
    std::unique_ptr<TaskGroup> io_task_group_;
    bool is_trial_;
    std::unordered_map<PackIdVersion, std::string> pack_id_to_content_key_;  // TODO(fixme): check offset
};
