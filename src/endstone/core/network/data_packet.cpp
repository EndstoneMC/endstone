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

endstone::core::DataPacket::DataPacket(int packet_id, std::string_view payload)
    : packet_id_(packet_id), payload_(payload)
{
}

MinecraftPacketIds endstone::core::DataPacket::getId() const
{
    return static_cast<MinecraftPacketIds>(packet_id_);
}

std::string endstone::core::DataPacket::getName() const
{
    return "DataPacket";
}

Bedrock::Result<void> endstone::core::DataPacket::checkSize(std::uint64_t packet_size, bool is_receiver_server) const
{
    if (is_receiver_server && packet_size > 0xA00000) {
        return BEDROCK_NEW_ERROR(std::errc::message_size);
    }
    return {};
}

void endstone::core::DataPacket::write(BinaryStream &stream) const
{
    BinaryStream bs(payload_, false);
    stream.writeStream(bs);
}

Bedrock::Result<void> endstone::core::DataPacket::read(ReadOnlyBinaryStream &stream)
{
    if (auto result = _read(stream); !result.ignoreError()) {
        return BEDROCK_RETHROW(result);
    }
    return {};
}

bool endstone::core::DataPacket::disallowBatching() const
{
    return false;
}

bool endstone::core::DataPacket::isValid() const
{
    return true;
}

Bedrock::Result<void> endstone::core::DataPacket::_read(ReadOnlyBinaryStream &stream)
{
    payload_ = stream.getView().substr(stream.getReadPointer());
    return {};
}
