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
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "bedrock/core/threading/spsc_queue.h"
#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/forward.h"  // class TaskGroup {}
#include "network_peer.h"

// Reconstruction of BDS's BatchedNetworkPeer (size 344, mAsyncEnabled at +0x150). endstone::core::
// AsyncBatchedNetworkPeer derives from this so BDS's NetworkSystem::enableAsyncFlush, which writes mAsyncEnabled
// directly through a BatchedNetworkPeer* at that offset, lands on our inherited field, and so
// NetworkConnection::batched_peer (a std::weak_ptr<BatchedNetworkPeer>) can hold our spliced object. Endstone never
// runs BDS's batching on these members; the async peer uses its own strand + queue and leaves them default.
class BatchedNetworkPeer : public NetworkPeer {
public:
    ~BatchedNetworkPeer() override = default;

protected:
    BatchedNetworkPeer() = default;

    struct DataCallback {
        std::string data;
        Compressibility compressible;
        std::function<void()> callback;
    };

    BinaryStream outgoing_data_;
    std::size_t compressible_bytes_{0};
    std::string incoming_data_buffer_;
    std::optional<ReadOnlyBinaryStream> incoming_data_;
    std::unique_ptr<TaskGroup> task_group_;
    SPSCQueue<DataCallback, 512> send_queue_;
    std::atomic_bool task_running_{false};
    std::atomic<std::uint64_t> queued_packets_{0};
    std::uint64_t sent_packets_{0};
    bool async_enabled_{false};
};
static_assert(sizeof(BatchedNetworkPeer) == 344,
              "BatchedNetworkPeer must match BDS layout so mAsyncEnabled stays at offset 0x150");
