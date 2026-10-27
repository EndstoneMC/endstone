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
#include <optional>
#include <utility>

#include "bedrock/network/batched_network_peer.h"
#include "bedrock/network/compressed_network_peer.h"
#include "bedrock/network/network_connection.h"
#include "endstone/core/network/batched_network_peer.h"
#include "endstone/core/network/compressed_network_peer.h"
#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

bool NetworkSystem::onNewIncomingConnection(const NetworkIdentifier &id, std::shared_ptr<NetworkPeer> &&peer,
                                            std::optional<WebKey> web_key)
{
    if (!ENDSTONE_HOOK_CALL_ORIGINAL(&NetworkSystem::onNewIncomingConnection, this, id, std::move(peer),
                                     std::move(web_key))) {
        return false;
    }

    // BDS appends the new connection, while a lookup by id can find a closing one with the same id
    auto &connection = *connections_.back();
    const auto compressed = connection.compressed_peer.lock();
    const auto batched = connection.batched_peer.lock();
    if (!compressed || !batched) {
        return true;
    }

    const auto *top = connection.peer.get();
    if (batched->peer_ != compressed || (top != batched.get() && top->peer_ != batched)) {
        endstone::core::EndstoneServer::getInstance().getLogger().error("Unable to splice the peer chain of {}.",
                                                                        id.getAddress());
        return true;
    }

    auto new_compressed =
        std::make_shared<endstone::core::EndstoneCompressedNetworkPeer>(compressed->peer_, *compressed);
    batched->peer_ = new_compressed;
    auto new_batched =
        std::make_shared<endstone::core::EndstoneBatchedNetworkPeer>(connection.id, new_compressed, batched);
    connection.compressed_peer = new_compressed;
    connection.batched_peer = new_batched;
    // Leaves out PacketTraceNetworkPeer, which nothing on the server reads
    connection.peer = std::move(new_batched);
    return true;
}
