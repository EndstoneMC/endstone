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

#include "bedrock/core/result.h"
#include "bedrock/deps/raknet/packet_priority.h"
#include "bedrock/forward.h"
#include "bedrock/network/compressibility.h"
#include "bedrock/network/minecraft_packet_ids.h"
#include "bedrock/network/network_peer.h"
#include "bedrock/network/sub_client_id.h"

class Packet {
public:
    virtual ~Packet() = default;
    // [[nodiscard]] virtual MinecraftPacketIds getId() const = 0;
    // [[nodiscard]] virtual std::string getName() const = 0;
    // [[nodiscard]] virtual Bedrock::Result<void> checkSize(std::uint64_t, bool) const = 0;
    // virtual void write(BinaryStream &) const = 0;
    // [[nodiscard]] virtual Bedrock::Result<void> read(ReadOnlyBinaryStream &) = 0;
    // [[nodiscard]] virtual bool disallowBatching() const = 0;
    // [[nodiscard]] virtual bool isValid() const = 0;

private:
    // [[nodiscard]] virtual Bedrock::Result<void> _read(ReadOnlyBinaryStream &) = 0;

    PacketPriority priority_{PacketPriority::MEDIUM_PRIORITY};                         // + 8
    NetworkPeer::Reliability reliability_{NetworkPeer::Reliability::ReliableOrdered};  // + 12
    SubClientId sub_client_id_{SubClientId::PrimaryClient};                            // + 16
    bool is_handled_{false};                                                           // + 17
    NetworkPeer::PacketRecvTimepoint recv_timepoint_;                                  // + 24
    void *handler_{nullptr};                                                           // + 32
    Compressibility compressibility_{Compressibility::Compressible};                   // + 40
};
BEDROCK_STATIC_ASSERT_SIZE(Packet, 48, 48);
