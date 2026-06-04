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

#include "endstone/core/network/buffered_raknet_peer.h"

#include <utility>

namespace endstone::core {

BufferedRakNetPeer::BufferedRakNetPeer(std::shared_ptr<NetworkPeer> raknet_peer)
{
    peer_ = std::move(raknet_peer);
}

void BufferedRakNetPeer::sendPacket(const std::string &data, Reliability reliability, Compressibility compressible)
{
    peer_->sendPacket(data, reliability, compressible);
}

NetworkPeer::NetworkStatus BufferedRakNetPeer::getNetworkStatus() const
{
    return peer_->getNetworkStatus();
}

void BufferedRakNetPeer::update()
{
    auto timepoint = std::make_shared<PacketRecvTimepoint>();
    for (;;) {
        ReceivedPacket packet;
        if (peer_->_receivePacket(packet.data, timepoint) != DataStatus::HasData) {
            break;
        }
        packet.timepoint = *timepoint;
        recv_queue_.enqueue(std::move(packet));
    }
    peer_->update();
}

NetworkPeer::DataStatus BufferedRakNetPeer::_receivePacket(std::string &out_data,
                                                           const PacketRecvTimepointPtr &timepoint_ptr)
{
    ReceivedPacket packet;
    if (!recv_queue_.try_dequeue(packet)) {
        return DataStatus::NoData;
    }
    out_data = std::move(packet.data);
    if (timepoint_ptr) {
        *timepoint_ptr = packet.timepoint;
    }
    return DataStatus::HasData;
}

}  // namespace endstone::core
