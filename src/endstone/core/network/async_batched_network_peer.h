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
#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "bedrock/core/threading/spsc_queue.h"
#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/network/batched_network_peer.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/network_peer.h"
#include "endstone/core/network/event_loop_group.h"

class NetworkConnection;

namespace endstone::core {

/**
 * ABI-compatible replacement for BDS's BatchedNetworkPeer (issue #356). Spliced into the peer chain in place of the
 * original Batched layer; offloads per-connection decrypt/decompress/compress/encrypt to an asio strand once the
 * connection authenticates (the inherited mAsyncEnabled flag, set by BDS's enableAsyncFlush).
 *
 * Threading: one strand per connection owns all inner-chain codec (recv, send and update), serialized like a Netty
 * channel event loop; the main thread owns batching, event dispatch and the dequeue. The lock-free SPSC hands
 * sub-packets from the strand (producer) to the main thread (consumer). Posted strand tasks capture
 * shared_from_this(), so the peer cannot be destroyed while a task is in flight (no use-after-free on teardown).
 *
 * Before activation it batches synchronously on the main thread (same observable behavior as BDS). Packet events
 * (PacketSendEvent/PacketReceiveEvent) and packet patching always fire on the main thread.
 */
class AsyncBatchedNetworkPeer : public BatchedNetworkPeer,
                                public std::enable_shared_from_this<AsyncBatchedNetworkPeer> {
public:
    AsyncBatchedNetworkPeer(std::shared_ptr<NetworkPeer> compressed_peer, EventLoopGroup::EventLoop event_loop);
    ~AsyncBatchedNetworkPeer() override;

    /**
     * Splice an AsyncBatchedNetworkPeer into `connection` in place of its BatchedNetworkPeer (keeping PacketTrace
     * and the inner chain). Runs on the main thread at connection setup, before any strand task is posted. No-op if
     * the connection has no Batched layer (local / encryption-disabled fast path) or the chain shape is unexpected.
     */
    static void splice(NetworkConnection &connection, EventLoopGroup &group);

    void sendPacket(const std::string &data, Reliability reliability, Compressibility compressible) override;
    [[nodiscard]] NetworkStatus getNetworkStatus() const override;
    void update() override;
    void flush(std::function<void()> &&callback) override;
    [[nodiscard]] bool isLocal() const override;
    [[nodiscard]] bool isEncrypted() const override;
    [[nodiscard]] bool isLan() const override;

protected:
    DataStatus _receivePacket(std::string &out_data, const PacketRecvTimepointPtr &timepoint_ptr) override;

private:
    [[nodiscard]] const NetworkIdentifier &getId() const;

    // Fire PacketSendEvent + patch outbound packets; returns the (possibly modified) bytes to batch, or nullopt if a
    // plugin cancelled the packet. Main thread only.
    std::optional<std::string> handleSendEvent(const std::string &data);
    // Fire PacketReceiveEvent; returns the (possibly modified) bytes, or nullopt if cancelled. Main thread only.
    std::optional<std::string> handleReceiveEvent(const std::string &data);

    void flushSync();  // main-thread synchronous batch send (pre-activation)
    [[nodiscard]] Compressibility computeCompressibility() const;

    void recvLoop();                                  // on strand_: drain + split + enqueue
    DataStatus splitNext(std::string &out);           // pull/split one sub-packet from the inner chain

    EventLoopGroup::EventLoop event_loop_;            // per-connection event loop (both directions)

    SPSCQueue<std::string> inbound_{1024};            // strand (producer) -> main thread (consumer)
    std::atomic<bool> closing_{false};
    std::atomic<bool> recv_scheduled_{false};

    std::string incoming_buffer_;                     // strand_-only: current batch being split
    std::optional<ReadOnlyBinaryStream> incoming_;    // strand_-only

    BinaryStream outgoing_;                           // main-only: send-batch accumulator
    std::size_t compressible_bytes_ = 0;              // main-only
    bool activated_ = false;                          // main-only: latched after first observed mAsyncEnabled
};

}  // namespace endstone::core
