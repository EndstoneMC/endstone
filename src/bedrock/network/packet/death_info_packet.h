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
#include "bedrock/network/packet/cerealize/core/packet_serialization_helper.h"
#include "bedrock/network/packet/serialize/serialized_packet.h"

struct DeathInfoPacketInfo {
    static constexpr auto PACKET_NAME = "DeathInfoPacket";
    static constexpr auto PACKET_ID = MinecraftPacketIds::DeathInfo;
    static constexpr SerializationMode DEFAULT_PACKET_SERIALIZATION_MODE = SerializationMode::SideBySide_LogOnMismatch;
    static constexpr Compressibility COMPRESSIBILITY = Compressibility::Compressible;
};

struct DeathInfoPacketPayload {
    using DeathCauseMessageType =
        TypeWrapper<std::pair<std::string, std::vector<std::string>>, struct TagDeathCauseMessageType>;
    DeathInfoPacketPayload();
    DeathInfoPacketPayload(const DeathCauseMessageType &);

    DeathCauseMessageType death_cause_message;
};

class DeathInfoPacket : public SerializedPayloadPacket<DeathInfoPacketInfo, DeathInfoPacketPayload> {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;
};
