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

#include "bedrock/network/network_system.h"

#include <memory>
#include <utility>

#include "endstone/core/network/async_batched_network_peer.h"
#include "endstone/core/network/buffered_raknet_peer.h"
#include "endstone/core/network/event_loop_group.h"
#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

// Issue #356: once BDS has built the connection's peer chain, replace its BatchedNetworkPeer with our async one.
bool NetworkSystem::onNewIncomingConnection(const NetworkIdentifier &id, std::shared_ptr<NetworkPeer> &&peer)
{
    auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&NetworkSystem::onNewIncomingConnection, this, id, std::move(peer));
    if (!result) {
        return result;
    }

    auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.isAsyncNetworkEnabled()) {
        return result;  // [network] async = false -> leave BDS's synchronous peer chain untouched
    }

    auto *connection = _getConnectionFromId(id);
    if (!connection) {
        return result;
    }
    auto batched = connection->batched_peer.lock();
    if (!batched || !batched->peer_) {
        throw std::runtime_error("Invalid connection state: expected a BatchedNetworkPeer with an inner peer");
    }

    // Wrap the chain's RakNet leaf (the childless peer) so its read buffer is only ever touched on the main thread; the
    // wrapper is transparent to AsyncBatchedNetworkPeer, which just sees the inner chain root.
    auto *leaf = &batched->peer_;
    while ((*leaf)->peer_) {
        leaf = &(*leaf)->peer_;
    }
    *leaf = std::make_shared<endstone::core::BufferedRakNetPeer>(*leaf);

    // Replace the BatchedNetworkPeer with our async one wherever the chain holds it (PacketTrace -> ... -> batched).
    auto *slot = &connection->peer;
    while (*slot && *slot != batched) {
        slot = &(*slot)->peer_;
    }
    if (*slot != batched) {
        throw std::runtime_error("Expected BatchedNetworkPeer in the new connection's peer chain, but none found");
    }

    auto async_batched = std::make_shared<endstone::core::AsyncBatchedNetworkPeer>(id, batched->peer_,
                                                                                   server.getEventLoopGroup().next());
    *slot = async_batched;
    connection->batched_peer = async_batched;
    return result;
}
