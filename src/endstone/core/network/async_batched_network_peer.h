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
#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "bedrock/core/threading/spsc_queue.h"
#include "bedrock/network/batched_network_peer.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/network_peer.h"
#include "endstone/core/network/event_loop_group.h"

namespace endstone::core {

/**
 * ABI-compatible stand-in for BDS's BatchedNetworkPeer (issue #356), spliced into the peer chain in its place. Once
 * the connection authenticates (BDS sets the inherited mAsyncEnabled), the inner-chain codec moves onto one asio
 * strand -- the connection's event loop -- while the main thread keeps batching, event dispatch and the queue drain.
 * Strand tasks capture shared_from_this(), so teardown never races in-flight work. Before activation it batches
 * synchronously, matching BDS's observable behavior.
 */
class AsyncBatchedNetworkPeer : public BatchedNetworkPeer,
                                public std::enable_shared_from_this<AsyncBatchedNetworkPeer> {
public:
    AsyncBatchedNetworkPeer(NetworkIdentifier id, std::shared_ptr<NetworkPeer> peer,
                            EventLoopGroup::EventLoop event_loop);

    void sendPacket(const std::string &data, Reliability reliability, Compressibility compressible) override;
    void update() override;
    void flush(std::function<void()> &&callback) override;

protected:
    DataStatus _receivePacket(std::string &out_data, const PacketRecvTimepointPtr &timepoint_ptr) override;

private:
    [[nodiscard]] const NetworkIdentifier &getId() const;

    // PacketSendEvent / PacketReceiveEvent + packet patching. Main thread only; nullopt means a plugin cancelled.
    std::optional<std::string> handleSendEvent(const std::string &data);
    std::optional<std::string> handleReceiveEvent(const std::string &data);

    void recvLoop();

    NetworkIdentifier id_;
    EventLoopGroup::EventLoop event_loop_;
    SPSCQueue<std::string> recv_queue_;
    std::atomic<bool> recv_scheduled_{false};
    bool activated_ = false;
};

}  // namespace endstone::core
