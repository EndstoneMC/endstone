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
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/deps/webrtc/socket_address.h"

namespace endstone::core {
class NetherNetAddressCache;
}

namespace webrtc {

enum class IceCandidateType : int {
    kHost = 0,
    kSrflx = 1,
    kPrflx = 2,
    kRelay = 3,
};

enum AdapterType : unsigned int {
    ADAPTER_TYPE_UNKNOWN = 0,
    ADAPTER_TYPE_ETHERNET = 1,
    ADAPTER_TYPE_WIFI = 2,
    ADAPTER_TYPE_CELLULAR = 4,
    ADAPTER_TYPE_VPN = 8,
    ADAPTER_TYPE_LOOPBACK = 16,
    ADAPTER_TYPE_ANY = 32,
    ADAPTER_TYPE_CELLULAR_2G = 64,
    ADAPTER_TYPE_CELLULAR_3G = 128,
    ADAPTER_TYPE_CELLULAR_4G = 256,
    ADAPTER_TYPE_CELLULAR_5G = 512,
};

class Candidate {
private:
    std::string id_;
    int component_;
    std::string protocol_;
    std::string relay_protocol_;
    SocketAddress address_;
    std::uint32_t priority_;
    std::string username_;
    std::string password_;
    IceCandidateType type_;
    std::string network_name_;
    AdapterType network_type_;
    AdapterType underlying_type_for_vpn_;
    std::uint32_t generation_;
    std::string foundation_;
    SocketAddress related_address_;
    std::string tcptype_;
    std::string transport_name_;
    std::uint16_t network_id_;
    std::uint16_t network_cost_;
    std::string url_;

    friend class endstone::core::EndstoneSocketAddress;
    friend class endstone::core::NetherNetAddressCache;
};
BEDROCK_STATIC_ASSERT_SIZE(Candidate, 528, 432);

class CandidatePairInterface {
public:
    virtual ~CandidatePairInterface() = default;
    [[nodiscard]] virtual const Candidate &local_candidate() const = 0;
    [[nodiscard]] virtual const Candidate &remote_candidate() const = 0;
};

}  // namespace webrtc
