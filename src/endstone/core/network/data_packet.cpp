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

#include "endstone/core/network/data_packet.h"

namespace endstone::core {
DataPacket::DataPacket(int packet_id, std::string_view payload) : packet_id_(packet_id), payload_(payload) {}

MinecraftPacketIds DataPacket::getId() const
{
    return static_cast<MinecraftPacketIds>(packet_id_);
}

std::string_view DataPacket::getName() const
{
    return "DataPacket";
}

void DataPacket::write(BinaryStream &stream) const
{
    stream.writeRawBytes(payload_);
}

int DataPacket::getPacketId() const
{
    return packet_id_;
}

std::string_view DataPacket::getPayload() const
{
    return payload_;
}

Bedrock::Result<void> DataPacket::_read(ReadOnlyBinaryStream &stream)
{
    payload_ = stream.getView().substr(stream.getReadPointer());
    return {};
}

}  // namespace endstone::core
