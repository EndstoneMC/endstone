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

// Faithful reimplementation of BDS's BatchedNetworkPeer (size 344, mAsyncEnabled at +0x150): it batches outgoing
// packets and splits incoming batches exactly as BDS does. endstone::core::AsyncBatchedNetworkPeer subclasses it and
// overrides only the send/receive paths to run the inner-chain codec on a worker thread. Deriving from this also lets
// BDS's enableAsyncFlush write mAsyncEnabled through a BatchedNetworkPeer* onto our inherited field, and lets
// NetworkConnection::batched_peer (a weak_ptr<BatchedNetworkPeer>) hold our spliced object.
class BatchedNetworkPeer : public NetworkPeer {
public:
    ~BatchedNetworkPeer() override = default;

    void sendPacket(const std::string &data, Reliability reliability, Compressibility compressible) override;
    [[nodiscard]] NetworkStatus getNetworkStatus() const override;
    void update() override;
    void flush(std::function<void()> &&callback) override;
    [[nodiscard]] bool isLocal() const override;
    [[nodiscard]] bool isEncrypted() const override;
    [[nodiscard]] bool isLan() const override;

protected:
    BatchedNetworkPeer() = default;
    DataStatus _receivePacket(std::string &out_data, const PacketRecvTimepointPtr &timepoint_ptr) override;
    [[nodiscard]] Compressibility getCompressibleState(std::size_t data_to_send) const;  // Endstone: protected, was private

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
    SPSCQueue<DataCallback> send_queue_;
    std::atomic_bool task_running_{false};
    std::atomic<std::uint64_t> queued_packets_{0};
    std::uint64_t sent_packets_{0};
    bool async_enabled_{false};
};
static_assert(sizeof(BatchedNetworkPeer) == 344,
              "BatchedNetworkPeer must match BDS layout so mAsyncEnabled stays at offset 0x150");
