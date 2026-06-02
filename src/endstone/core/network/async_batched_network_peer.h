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
#include <mutex>
#include <optional>
#include <queue>
#include <string>

#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/network/batched_network_peer.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/network_peer.h"
#include "endstone/core/network/network_thread_pool.h"

class NetworkConnection;

namespace endstone::core {

/**
 * Per-connection async state, owned by a shared_ptr and captured by every posted strand task so the strand
 * extends its lifetime past ~AsyncBatchedNetworkPeer (no use-after-free on teardown).
 */
struct AsyncConnectionState {
    AsyncConnectionState(NetworkThreadPool &pool, std::shared_ptr<NetworkPeer> inner)
        : recv_strand(pool.makeStrand()), send_strand(pool.makeStrand()), inner_peer(std::move(inner))
    {
    }

    NetworkThreadPool::Strand recv_strand;
    NetworkThreadPool::Strand send_strand;
    std::shared_ptr<NetworkPeer> inner_peer;  // the CompressedNetworkPeer (inner chain root)

    std::atomic<bool> closing{false};
    std::atomic<bool> recv_scheduled{false};

    // recv_strand-only: current decompressed/decrypted batch being split into sub-packets
    std::string incoming_buffer;
    std::optional<ReadOnlyBinaryStream> incoming;

    // sub-packets handed from recv_strand (producer) to the main thread (consumer)
    std::mutex inbound_mutex;
    std::queue<std::string> inbound;
};

/**
 * ABI-compatible replacement for BDS's BatchedNetworkPeer (issue #356). Spliced into the peer chain in place
 * of the original Batched layer; offloads per-connection decrypt/decompress/compress/encrypt to an asio strand
 * once the connection authenticates (the inherited mAsyncEnabled flag, set by BDS's enableAsyncFlush).
 *
 * Before activation it batches synchronously on the main thread (same observable behavior as BDS). Packet events
 * (PacketSendEvent/PacketReceiveEvent) and packet patching always fire on the main thread.
 */
class AsyncBatchedNetworkPeer : public BatchedNetworkPeer {
public:
    AsyncBatchedNetworkPeer(std::shared_ptr<NetworkPeer> compressed_peer, NetworkThreadPool &pool);
    ~AsyncBatchedNetworkPeer() override;

    /**
     * Splice an AsyncBatchedNetworkPeer into `connection` in place of its BatchedNetworkPeer (keeping PacketTrace
     * and the inner chain). Runs on the main thread at connection setup, before any strand task is posted. No-op if
     * the connection has no Batched layer (local / encryption-disabled fast path) or the chain shape is unexpected.
     */
    static void splice(NetworkConnection &connection, NetworkThreadPool &pool);

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

    // Send-side: fire PacketSendEvent + patch outbound packets; returns the (possibly modified) bytes to batch,
    // or std::nullopt if a plugin cancelled the packet.
    std::optional<std::string> handleSendEvent(const std::string &data);
    // Receive-side: fire PacketReceiveEvent; returns the (possibly modified) bytes, or std::nullopt if cancelled.
    std::optional<std::string> handleReceiveEvent(const std::string &data);

    void flushSync();                          // main-thread synchronous batch send (pre-activation)
    [[nodiscard]] Compressibility computeCompressibility() const;

    // Runs on recv_strand: drain inner_peer->_receivePacket(), split batches, enqueue sub-packets. Static so the
    // posted task captures only the shared state, not `this`.
    static void recvLoop(std::shared_ptr<AsyncConnectionState> state);
    // Split one length-prefixed sub-packet out of `state->incoming` into `out`; pulls the next batch when empty.
    static DataStatus splitNext(AsyncConnectionState &state, std::string &out);

    NetworkThreadPool &pool_;
    std::shared_ptr<AsyncConnectionState> state_;

    BinaryStream outgoing_;          // main-thread send-batch accumulator
    std::size_t compressible_bytes_ = 0;
    bool activated_ = false;         // main-thread latch: true after first observed mAsyncEnabled
};

}  // namespace endstone::core
