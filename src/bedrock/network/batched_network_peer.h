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

#include "network_peer.h"

// Endstone: layout-faithful reconstruction of BDS's BatchedNetworkPeer (size 344, mAsyncEnabled @ +0x150).
//
// endstone::core::AsyncBatchedNetworkPeer derives from this so that:
//   1. BDS's NetworkSystem::enableAsyncFlush, which writes `mAsyncEnabled = 1` directly through a
//      BatchedNetworkPeer* at offset +0x150, lands the write on our inherited field (no corruption); and
//   2. NetworkConnection::batched_peer (a std::weak_ptr<BatchedNetworkPeer>) can hold our spliced object.
//
// We never construct a standalone BatchedNetworkPeer nor touch the reserved members. The heavy BDS members
// (mOutgoingData, mIncomingData, mTaskGroup, mSendQueue, ...) are opaque storage we own but ignore; the async
// peer uses its own asio strand + queue. The base is left abstract on purpose (NetworkPeer's pure virtuals are
// overridden by AsyncBatchedNetworkPeer), so no BDS ctor/dtor/vtable needs to be linked.
class BatchedNetworkPeer : public NetworkPeer {
public:
    ~BatchedNetworkPeer() override = default;

protected:
    BatchedNetworkPeer() = default;

    std::byte reserved_[0x150 - sizeof(NetworkPeer)];  // BDS: mOutgoingData .. mSentPackets (unused by Endstone)
    bool mAsyncEnabled;                                // BDS: +0x150, set by enableAsyncFlush after auth
};
static_assert(sizeof(BatchedNetworkPeer) == 344,
              "BatchedNetworkPeer must match BDS layout so mAsyncEnabled stays at offset 0x150");
