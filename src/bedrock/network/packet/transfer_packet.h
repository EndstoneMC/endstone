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

#include <string>

#include "bedrock/network/packet.h"

struct TransferPacketInfo {
    static constexpr auto PACKET_NAME = "TransferPacket";
    static constexpr auto PACKET_ID = MinecraftPacketIds::Transfer;
    static constexpr auto DEFAULT_PACKET_SERIALIZATION_MODE = SerializationMode::SideBySide_LogOnMismatch;
    static constexpr auto COMPRESSIBILITY = Compressibility::Compressible;
};

struct TransferPacketPayload {
    TransferPacketPayload();
    TransferPacketPayload(const std::string &, int);
    TransferPacketPayload(const NetherNet::NetworkID &);
    TransferPacketPayload(bool);

    std::string destination;
    uint16_t destination_port;
    bool reload_world;
};

class TransferPacket : public SerializedPayloadPacket<TransferPacketInfo, TransferPacketPayload> {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;
};
