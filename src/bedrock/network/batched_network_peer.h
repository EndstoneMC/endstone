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
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/core/threading/spsc_queue.h"
#include "bedrock/core/threading/task_group.h"
#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/network/network_peer.h"

class BatchedNetworkPeer : public NetworkPeer {
private:
    struct DataCallback {
        std::string data;
        Compressibility compressible;
        std::function<void()> callback;
    };

    BinaryStream outgoing_data_;
    std::size_t compressible_bytes_;
    std::string incoming_data_buffer_;
    std::optional<ReadOnlyBinaryStream> incoming_data_;
    std::unique_ptr<TaskGroup> task_group_;
    SPSCQueue<DataCallback, 512> send_queue_;
    std::atomic_bool task_running_;
    std::atomic<std::uint64_t> queued_packets_;
    std::uint64_t sent_packets_;
    bool async_enabled_;

    friend class endstone::core::EndstoneBatchedNetworkPeer;  // Endstone
};
BEDROCK_STATIC_ASSERT_SIZE(BatchedNetworkPeer, 344, 320);
