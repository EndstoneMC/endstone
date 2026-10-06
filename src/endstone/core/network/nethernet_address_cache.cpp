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
#include "endstone/core/network/nethernet_address_cache.h"

#include <string>

namespace endstone::core {

namespace {
constexpr std::string_view UFRAG_ATTRIBUTE = "a=ice-ufrag:";
}  // namespace

NetherNetAddressCache &NetherNetAddressCache::getInstance()
{
    static NetherNetAddressCache instance;
    return instance;
}

void NetherNetAddressCache::addSession(const NetherNet::NetworkID &id, const std::string_view sdp)
{
    const auto begin = sdp.find(UFRAG_ATTRIBUTE);
    if (begin == std::string_view::npos) {
        return;
    }
    const auto value = sdp.substr(begin + UFRAG_ATTRIBUTE.size());
    const auto ufrag = value.substr(0, value.find_first_of("\r\n"));
    if (ufrag.empty()) {
        return;
    }

    const std::lock_guard lock(mutex_);
    if (by_ufrag_.insert_or_assign(std::string(ufrag), id).second) {
        ufrag_order_.emplace_back(ufrag);
    }
    while (ufrag_order_.size() > MAX_PENDING_SESSIONS) {
        by_ufrag_.erase(ufrag_order_.front());
        ufrag_order_.pop_front();
    }
}

void NetherNetAddressCache::addAddress(const webrtc::Candidate &candidate, const SocketAddress &address)
{
    const std::lock_guard lock(mutex_);
    const auto it = by_ufrag_.find(candidate.username_);
    if (it == by_ufrag_.end()) {
        return;
    }
    if (by_id_.insert_or_assign(it->second, address).second) {
        id_order_.emplace_back(it->second);
    }
    while (id_order_.size() > MAX_PENDING_SESSIONS) {
        by_id_.erase(id_order_.front());
        id_order_.pop_front();
    }
}

SocketAddress NetherNetAddressCache::get(const NetherNet::NetworkID &id)
{
    const std::lock_guard lock(mutex_);
    const auto it = by_id_.find(id);
    if (it == by_id_.end()) {
        return {};
    }
    return it->second;
}

}  // namespace endstone::core
