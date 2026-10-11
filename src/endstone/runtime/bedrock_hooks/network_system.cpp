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

#include "bedrock/network/compressed_network_peer.h"
#include "bedrock/network/network_connection.h"
#include "endstone/core/network/compressed_network_peer.h"
#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

bool NetworkSystem::onNewIncomingConnection(const NetworkIdentifier &id, std::shared_ptr<NetworkPeer> &&peer)
{
    if (!ENDSTONE_HOOK_CALL_ORIGINAL(&NetworkSystem::onNewIncomingConnection, this, id, std::move(peer))) {
        return false;
    }

    // BDS appends the new connection, while a lookup by id can find a closing one with the same id
    auto &connection = *connections_.back();
    const auto compressed = connection.compressed_peer.lock();
    if (!compressed) {
        return true;
    }

    auto *slot = &connection.peer;
    while (*slot && *slot != compressed) {
        slot = &(*slot)->peer_;
    }
    if (!*slot) {
        endstone::core::EndstoneServer::getInstance().getLogger().error("Unable to splice the peer chain of {}.",
                                                                        id.getAddress());
        return true;
    }

    auto replacement = std::make_shared<endstone::core::EndstoneCompressedNetworkPeer>(compressed->peer_, *compressed);
    *slot = replacement;
    connection.compressed_peer = replacement;
    return true;
}
