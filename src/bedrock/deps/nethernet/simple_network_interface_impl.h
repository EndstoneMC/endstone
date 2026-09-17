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

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string_view>

#include "bedrock/bedrock.h"
#include "bedrock/deps/nethernet/context.h"
#include "bedrock/deps/nethernet/events.h"
#include "bedrock/deps/nethernet/nethernet_transport.h"
#include "bedrock/deps/nethernet/network_id.h"
#include "bedrock/deps/webrtc/scoped_refptr.h"

namespace endstone::core {
class EndstoneServer;
}

namespace webrtc {
class PeerConnectionFactory;
}

namespace NetherNet {

class NetworkSessionManager;

enum class SignalingChannelId {
    WebSocket = 0,
    Lan = 1,
};

class SimpleNetworkInterfaceImpl : ContextProxy,
                                   public INetherNetTransportInterface,
                                   public ISignalingEventHandler,
                                   public ILanEventHandler {
public:
    ~SimpleNetworkInterfaceImpl() override;

private:
    ENDSTONE_HOOK void ReceiveFromSignalingChannel(NetworkID from, std::string_view message,
                                                   SignalingChannelId source_channel);

    std::atomic<unsigned int> ref_;
    INetherNetTransportInterfaceCallbacks *callbacks_;
    NetworkID network_id_;
    std::unique_ptr<NetworkSessionManager> network_session_manager_;
    webrtc::scoped_refptr<webrtc::PeerConnectionFactory> peer_connection_factory_;
    std::mutex api_level_mutex_;
    // TODO(fixme): webrtc::PeerConnectionInterface::RTCConfiguration
#ifdef _WIN32
    std::byte rtc_config_[416];
#elif __linux__
    std::byte rtc_config_[408];
#endif
    bool disable_lan_signaling_;
    bool disable_trickle_ice_;
    std::shared_ptr<ISignalingInterface> signaling_interface_;

    friend class endstone::core::EndstoneServer;
};

}  // namespace NetherNet
