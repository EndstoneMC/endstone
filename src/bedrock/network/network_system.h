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
#include "bedrock/network/network_enable_disable_listener.h"
#include "bedrock/network/rak_peer_helper.h"
#include "bedrock/network/raknet_connector.h"
#include "bedrock/platform/threading/mutex_details.h"

class NetworkSystem : public RakNetConnector::ConnectionCallbacks,
                      public RakPeerHelper::IPSupportInterface,
                      public NetworkEnableDisableListener {
protected:
    struct Dependencies;
    NetworkSystem(Dependencies &&);

    Bedrock::NotNullNonOwnerPtr<NetworkSessionOwner> network_session_owner_;
    std::vector<std::unique_ptr<NetworkConnection>> connections_;
    std::unique_ptr<LocalConnector> local_connector_;

private:
    friend class endstone::core::EndstoneServerNetworkEventHandler;

    std::unique_ptr<RemoteConnector> remote_connector_;
    std::unique_ptr<ServerLocator> server_locator_;
    Bedrock::Threading::RecursiveMutex connections_mutex_;
    std::size_t current_connection_;
    Bedrock::Threading::IAsyncResult<void>::Handle receive_task_;
    std::unique_ptr<TaskGroup> receive_task_group_;
    Bedrock::NonOwnerPointer<IPacketObserver> packet_observer_;
    Scheduler &main_thread_;
    std::string receive_buffer_;  // +272
    std::string send_buffer_;
    BinaryStream send_stream_;
    struct IncomingPacketQueue {
        NetEventCallback &callback_obj;
        Bedrock::Threading::Mutex mutex;
    };
    std::unique_ptr<IncomingPacketQueue> incoming_packets[4];
    bool use_ipv6_only_;
    uint16_t default_game_port_;
    uint16_t default_game_port_v6_;
    bool is_lan_discovery_enabled_;
    void *network_statistics_;
    // ...
};
