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

#include "bedrock/core/threading/async.h"
#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/forward.h"
#include "bedrock/network/net_event_callback.h"
#include "bedrock/network/network_connection.h"
#include "bedrock/network/network_enable_disable_listener.h"
#include "bedrock/network/packet_observer_interface.h"
#include "bedrock/network/packet_serialization_controller.h"
#include "bedrock/network/rak_peer_helper.h"
#include "bedrock/network/raknet_connector.h"
#include "bedrock/platform/threading/mutex_details.h"

class NetworkSystem : public RakNetConnector::ConnectionCallbacks,
                      public RakPeerHelper::IPSupportInterface,
                      public NetworkEnableDisableListener {
public:
    Bedrock::NotNullNonOwnerPtr<RemoteConnector> getRemoteConnector();
    [[nodiscard]] Bedrock::NotNullNonOwnerPtr<const RemoteConnector> getRemoteConnector() const;
    ENDSTONE_HOOK void send(const NetworkIdentifier &network_id, const Packet &packet, SubClientId sender_sub_id);
    ENDSTONE_HOOK void sendToMultiple(const std::vector<NetworkIdentifierWithSubId> &recipients, const Packet &packet);
    void setCloseConnection(const NetworkIdentifier &id);
    void closeConnection(const NetworkIdentifier &id, const Connection::DisconnectFailReason reason,
                         const std::string &message_from_server);
    [[nodiscard]] const cereal::ReflectionCtx &getPacketReflectionCtx() const;

protected:
    struct Dependencies;
    NetworkSystem(Dependencies &&);

    Bedrock::NotNullNonOwnerPtr<NetworkSessionOwner> network_session_owner_;
    Bedrock::Threading::RecursiveMutex connections_mutex_;
    std::vector<std::unique_ptr<NetworkConnection>> connections_;
    std::unique_ptr<LocalConnector> local_connector_;
    std::shared_ptr<PacketGroupDefinition::PacketGroupBuilder> packet_group_builder_;

public:
    NetworkConnection *_getConnectionFromId(const NetworkIdentifier &) const;  // Endstone: private -> public

private:
    void _sendInternal(const NetworkIdentifier &id, const Packet &packet, const std::string &data);
    std::unique_ptr<RemoteConnector> remote_connector_;
    std::unique_ptr<ServerLocator> server_locator_;
    std::size_t current_connection_;
    Bedrock::Threading::Async<void> receive_task_;
    std::unique_ptr<TaskGroup> receive_task_group_;
    Bedrock::NonOwnerPointer<IPacketObserver> packet_observer_;
    Scheduler &main_thread_;

public:                           // Endstone: private -> public
    std::string receive_buffer_;  // +272
    std::string send_buffer_;
    BinaryStream send_stream_;

private:
    struct IncomingPacketQueue {
        NetEventCallback &callback_obj;
        Bedrock::Threading::Mutex mutex;
    };
    std::unique_ptr<IncomingPacketQueue> incoming_packets[4];
    bool use_ipv6_only_;
    uint16_t default_game_port_;
    uint16_t default_game_port_v6_;
    bool is_lan_discovery_enabled_;
    std::unique_ptr<NetworkStatistics> network_statistics_;
    bool websockets_enabled_;
    NetworkSettingOptions network_setting_options_;
    gsl::not_null<std::unique_ptr<cereal::ReflectionCtx>> reflection_ctx_;             // +512
    gsl::not_null<std::unique_ptr<IPacketSerializationController>> packet_overrides_;  // +520
    gsl::not_null<std::unique_ptr<
        Bedrock::PubSub::Publisher<void(const Json::Value &), Bedrock::PubSub::ThreadModel::MultiThreaded>>>
        session_summary_publisher_;
};
