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
#include "endstone/core/network/network_thread_pool.h"
#include "endstone/runtime/hook.h"

// Issue #356: after BDS builds the connection (the chain is constructed inside NetworkConnection's ctor), splice
// an AsyncBatchedNetworkPeer in place of the BatchedNetworkPeer so per-connection decrypt/decompress/compress/
// encrypt can move to a strand once the client authenticates.
bool NetworkSystem::onNewIncomingConnection(const NetworkIdentifier &id, std::shared_ptr<NetworkPeer> &&peer)
{
    auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&NetworkSystem::onNewIncomingConnection, this, id, std::move(peer));
    if (!result) {
        return result;
    }

    auto &pool = endstone::core::NetworkThreadPool::getInstance();
    if (pool.isRunning()) {
        if (auto *connection = _getConnectionFromId(id)) {
            endstone::core::AsyncBatchedNetworkPeer::splice(*connection, pool);
        }
    }
    return result;
}
