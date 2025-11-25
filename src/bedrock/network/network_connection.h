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

#include <array>
#include <bitset>
#include <queue>

#include "bedrock/bedrock.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/network_peer.h"

class NetworkConnection {
public:
    enum class Type : int {
        Remote = 0,
        Local = 1,
    };
    NetworkConnection() = delete;

    [[nodiscard]] bool isChannelPaused(uint32_t channel) const;
    void setChannelPaused(uint32_t channel, bool paused);
    [[nodiscard]] ENDSTONE_HOOK NetworkPeer::DataStatus receivePacket(
        std::string &receive_buffer, const NetworkPeer::PacketRecvTimepointPtr &timepoint_ptr);
    void update();
    void disconnect();
    [[nodiscard]] bool shouldCloseConnection() const;

    NetworkIdentifier id;
    Type type;
    std::weak_ptr<class NetworkPacketRecorder> network_packet_recorder;
    std::weak_ptr<class EncryptedNetworkPeer> encrypted_peer;
    std::weak_ptr<class CompressedNetworkPeer> compressed_peer;
    std::weak_ptr<class BatchedNetworkPeer> batched_peer;
    std::weak_ptr<class LatencyNetworkPeer> latency_peer;
    std::weak_ptr<class PacketTraceNetworkPeer> packet_trace_peer;
    std::shared_ptr<NetworkPeer> peer;
    std::chrono::steady_clock::time_point last_packet_time;
    std::chrono::steady_clock::time_point closed_time;

protected:
    struct PausedPacket {
        std::string data;
        NetworkPeer::PacketRecvTimepoint timepoint;
    };
    std::bitset<2UL> paused_channels_;
    std::queue<PausedPacket> resumed_packets_;
    std::array<std::vector<PausedPacket>, 2UL> paused_packets_;

private:
    bool disconnected_;
    bool should_close_connection_;  // +377
    bool encryption_disabled_;
};
