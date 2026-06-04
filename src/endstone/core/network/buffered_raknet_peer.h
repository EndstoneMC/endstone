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

#include <memory>
#include <string>

#include "bedrock/core/threading/spsc_queue.h"
#include "bedrock/network/network_peer.h"

namespace endstone::core {

/**
 * Sits at the bottom of an async connection's peer chain, wrapping the RakNetNetworkPeer (issue #356). BDS keeps each
 * connection's received packets in an unsynchronized per-peer buffer that only the main thread may touch. update() runs
 * on the main thread as part of the per-tick update chain and drains that buffer into a single-producer/single-consumer
 * queue; the strand-side codec then pulls raw packets back out via _receivePacket(), so decrypt and decompress run off
 * the main thread without ever racing RakNet's buffer. Everything else forwards straight to the wrapped peer.
 */
class BufferedRakNetPeer : public NetworkPeer {
public:
    explicit BufferedRakNetPeer(std::shared_ptr<NetworkPeer> raknet_peer);

    void sendPacket(const std::string &data, Reliability reliability, Compressibility compressible) override;
    [[nodiscard]] NetworkStatus getNetworkStatus() const override;
    void update() override;

protected:
    DataStatus _receivePacket(std::string &out_data, const PacketRecvTimepointPtr &timepoint_ptr) override;

private:
    struct ReceivedPacket {
        std::string data;
        PacketRecvTimepoint timepoint;
    };

    SPSCQueue<ReceivedPacket> recv_queue_;
};

}  // namespace endstone::core
