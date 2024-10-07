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
#include "bedrock/core/threading/enable_queue_for_main_thread.h"
#include "bedrock/forward.h"
#include "bedrock/minecraft_app_interface.h"
#include "bedrock/network/connection_request.h"
#include "bedrock/network/disconnection_request_info.h"
#include "bedrock/network/net_event_callback.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/network_server_config.h"
#include "bedrock/network/sub_client_connection_request.h"
#include "bedrock/network/xbox_live_user_observer.h"
#include "bedrock/platform/multiplayer_service_observer.h"
#include "bedrock/server/commands/minecraft_commands.h"
#include "bedrock/server/deny_list.h"
#include "bedrock/server/server_player.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/events/server_events.h"
#include "bedrock/world/game_callbacks.h"
#include "bedrock/world/level/level_interface.h"
#include "bedrock/world/level/level_listener.h"
#include "endstone/endstone.h"

namespace endstone::detail {
class EndstoneServer;
}

enum class ServerTextEvent : std::uint8_t {
    Sleeping = 0,
    Connection = 1,
    ChangedSkin = 2,
    ConnectionOriginal = Connection | 0x80,  // Endstone
};

class ServerNetworkHandler : public Bedrock::Threading::EnableQueueForMainThread,
                             public NetEventCallback,
                             public LevelListener,
                             public Social::MultiplayerServiceObserver,
                             public Social::XboxLiveUserObserver {
public:
    ~ServerNetworkHandler() override = 0;

    ENDSTONE_HOOK bool trytLoadPlayer(ServerPlayer &, ConnectionRequest const &);
    ENDSTONE_HOOK void disconnectClient(NetworkIdentifier const &, SubClientId, Connection::DisconnectFailReason,
                                        std::string const &, std::optional<std::string>, bool);
    ENDSTONE_HOOK void updateServerAnnouncement();

    [[nodiscard]] const Bedrock::NonOwnerPointer<ILevel> &getLevel() const;  // Endstone

private:
    friend class endstone::detail::EndstoneServer;

    // NOLINTBEGIN(*-identifier-naming)
    ENDSTONE_HOOK ServerPlayer &_createNewPlayer(NetworkIdentifier const &, SubClientConnectionRequest const &,
                                                 SubClientId);
    ENDSTONE_HOOK void _displayGameMessage(Player const &, ChatEvent &);
    [[nodiscard]] ENDSTONE_HOOK bool _isServerTextEnabled(ServerTextEvent const &) const;
    // NOLINTEND(*-identifier-naming)

    GameCallbacks *callbacks_;                                   // +80
    Bedrock::NonOwnerPointer<ILevel> level_;                     // +88
    ServerNetworkSystem *network_;                               // +96
    PrivateKeyManager *server_keys_;                             // +112
    ServerLocator *server_locator_;                              // +120
    gsl::not_null<PacketSender *> packet_sender_;                // +128
    bool use_allow_list_;                                        // +136
    AllowList *allow_list_;                                      // +144
    PermissionsFile *permissions_file_;                          // +152
    DenyList server_deny_list_;                                  // +160
    NetworkServerConfig network_server_config_;                  // +264
    bool has_displayed_pack_errors_;                             // +336
    NetworkIdentifier my_id_;                                    // +344
    int max_chunk_radius_;                                       // +504
    MinecraftCommands *minecraft_commands_;                      // +512
    IMinecraftApp *app_;                                         // +520
    Bedrock::NonOwnerPointer<void *> text_filtering_processor_;  // +528 TextFilteringProcessor
    std::unique_ptr<void *> client_cache_manager_;               // +536
    std::unordered_map<unsigned long, std::string> server_storage_for_clients_connecting_attempt_;  // +552
    std::unique_ptr<void *> companion_handler_;          // +616 ClassroomModeNetworkHandler
    std::string tenant_id_;                              // +624
    std::string shareable_identity_token_;               // +656
    Bedrock::Threading::Mutex validate_player_mutex_;    // +688
    bool allow_incoming_;                                // +768
    std::unique_ptr<void *> server_network_controller_;  // +776 IServerNetworkController
    std::string server_name_;                            // +784
    std::vector<std::string> trusted_keys_;              // +816
    int max_num_players_;                                // +840
};
