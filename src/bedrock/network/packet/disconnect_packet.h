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

#include <optional>
#include <string>
#include <variant>

#include "bedrock/network/disconnection_request_info.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/cerealize/core/packet_serialization_helper.h"
#include "bedrock/network/packet/serialize/serialized_packet.h"

struct DisconnectPacketMessages {
    std::string message;
    std::string filtered_message;
};

namespace cereal {
struct NullType {};
}  // namespace cereal

using VariantDisconnectPacketMessages = std::variant<DisconnectPacketMessages, cereal::NullType>;

struct DisconnectPacketPayload {
    DisconnectPacketPayload();
    DisconnectPacketPayload(Connection::DisconnectFailReason reason, std::optional<std::string> message,
                            std::optional<std::string> filtered_message);
    Connection::DisconnectFailReason reason;
    VariantDisconnectPacketMessages messages;
};

class DisconnectPacket : public Packet {
public:
    DisconnectPacketPayload payload;
    SerializationMode serialization_mode = SerializationMode::CerealOnly;
    DisconnectPacket();
    DisconnectPacket(DisconnectPacketPayload payload);
};
