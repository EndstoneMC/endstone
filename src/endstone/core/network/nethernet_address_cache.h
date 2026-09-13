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

#include <deque>
#include <map>
#include <mutex>
#include <string>
#include <string_view>

#include "bedrock/deps/nethernet/network_id.h"
#include "bedrock/deps/webrtc/candidate.h"
#include "endstone/util/socket_address.h"

namespace endstone::core {

/**
 * @brief Remembers the remote address of a NetherNet peer until its player is constructed.
 *
 * NetherNet hands the address and the NetworkID to two different layers, so the two are joined on the
 * remote ICE ufrag. Both writers run on NetherNet threads; readers run on the server thread.
 *
 * Lookups do not consume: the login IP-ban check, the player constructor and every packet event all
 * ask for the same address. Entries fall out by insertion order once the bound is reached.
 */
class NetherNetAddressCache {
public:
    [[nodiscard]] static NetherNetAddressCache &getInstance();

    void addSession(const NetherNet::NetworkID &id, std::string_view sdp);
    void addAddress(const webrtc::Candidate &candidate, const SocketAddress &address);
    [[nodiscard]] SocketAddress get(const NetherNet::NetworkID &id);

private:
    // Signalling is reachable by anyone who can open the port, so both maps are bounded.
    static constexpr std::size_t MAX_PENDING_SESSIONS = 256;

    std::mutex mutex_;
    std::map<std::string, NetherNet::NetworkID> by_ufrag_;
    std::map<NetherNet::NetworkID, SocketAddress> by_id_;
    std::deque<std::string> ufrag_order_;
    std::deque<NetherNet::NetworkID> id_order_;
};

}  // namespace endstone::core
