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

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/pub_sub/subscription.h"
#include "bedrock/deps/nethernet/events.h"
#include "bedrock/deps/nethernet/nethernet_shared_constants.h"
#include "bedrock/deps/nethernet/network_id.h"

namespace NetherNet {

class IIdentityAssertionGenerator;
struct SessionState;

struct StunRelayServer {
    std::string uri;
    std::string username;
    std::string password;
};
BEDROCK_STATIC_ASSERT_SIZE(StunRelayServer, 96, 72);

using RelayConfig = std::vector<StunRelayServer>;

struct MappedAddressRange {
    std::optional<std::string> internal_address;
    std::uint16_t internal_port_min;
    std::uint16_t internal_port_max;
    std::optional<std::string> external_address;
    int external_port_offset;
};
BEDROCK_STATIC_ASSERT_SIZE(MappedAddressRange, 96, 80);

struct TransportConfiguration {
    struct Http {
        std::string bind_address;
        std::uint16_t port;
    };
    using DefaultSignalingInterface = std::variant<std::monostate, Http>;

    static const std::size_t MAX_STUN_RELAY_SERVERS = 16;
    StunRelayServer stun_relay_servers[16];
    int stun_relay_server_count;
    static const std::size_t MAX_KNOWN_MAPPED_ADDRESS_RANGES = 16;
    MappedAddressRange known_mapped_address_ranges[16];
    int known_mapped_address_range_count;
    DefaultSignalingInterface default_signaling_channel;
    int negotiation_timeout_in_seconds;
    std::uint16_t min_udp_port;
    std::uint16_t max_udp_port;
    bool global_udp_port;
};
BEDROCK_STATIC_ASSERT_SIZE(TransportConfiguration, 3152, 2504);

class INetherNetTransportInterfaceCallbacks {
public:
    virtual ~INetherNetTransportInterfaceCallbacks();
};

class ISignalingInterface {
public:
    virtual ~ISignalingInterface();
};

using Subscription = Bedrock::PubSub::Subscription;

class INetherNetTransportInterface {
public:
    virtual ~INetherNetTransportInterface();
    virtual bool SendPacket(NetworkID, RAWNETWORKID, const std::string &, ESendType) = 0;
    virtual bool IsPacketAvailable(NetworkID, RAWNETWORKID, unsigned int *) = 0;
    virtual bool ReadPacket(NetworkID, RAWNETWORKID, void *, unsigned int, unsigned int *) = 0;
    virtual bool OpenSessionWithUser(NetworkID, std::shared_ptr<IIdentityAssertionGenerator>) = 0;
    virtual void SetIdentityGenerator(std::shared_ptr<IIdentityAssertionGenerator>) = 0;
    virtual bool CloseSessionWithUser(NetworkID, RAWNETWORKID) = 0;
    virtual bool GetSessionState(NetworkID, RAWNETWORKID, SessionState *) = 0;
    virtual void SetSignalingInterface(const std::shared_ptr<ISignalingInterface> &) = 0;
    virtual void SetRelayConfig(const std::vector<StunRelayServer> &) = 0;
    virtual bool IsBroadcastDiscoveryEnabled() = 0;
    virtual void EnableBroadcastDiscovery() = 0;
    virtual void DisableBroadcastDiscovery() = 0;
    virtual void AddLanHost(NetworkID, const std::string &, int) = 0;
    virtual void RemoveLanHost(NetworkID) = 0;
    virtual void EnableLANSignaling() = 0;
    virtual void DisableLANSignaling() = 0;
    virtual void EnableTrickleIce() = 0;
    virtual void DisableTrickleIce() = 0;
    virtual Subscription RegisterEventHandler(ISignalingEventHandler *) = 0;
    virtual Subscription RegisterEventHandler(ILanEventHandler *) = 0;
};

}  // namespace NetherNet
