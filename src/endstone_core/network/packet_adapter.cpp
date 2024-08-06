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

#include "endstone/detail/network/packet_adapter.h"

#include <endstone/detail/network/packet_codec.h>
#include <magic_enum/magic_enum.hpp>

namespace endstone::detail {

PacketAdapter::PacketAdapter(endstone::Packet &packet) : packet_(packet) {}

MinecraftPacketIds PacketAdapter::getId() const
{
    return static_cast<MinecraftPacketIds>(packet_.getType());
}

std::string PacketAdapter::getName() const
{
    return std::string(magic_enum::enum_name(packet_.getType())) + "Packet";
}

Bedrock::Result<void> PacketAdapter::checkSize(std::uint64_t size, bool flag) const
{
    if (flag && size > 0xA00000) {
        Bedrock::ErrorInfo error_info;
        error_info.error = std::error_code(115, std::system_category());
        return nonstd::make_unexpected(error_info);
    }
    return {};
}

void PacketAdapter::write(BinaryStream &stream) const
{
    PacketCodec::encode(stream, packet_);
}

Bedrock::Result<void> PacketAdapter::read(ReadOnlyBinaryStream &stream)
{
    return _read(stream);
}

bool PacketAdapter::disallowBatching() const
{
    return false;
}

bool PacketAdapter::isValid() const
{
    return true;
}

Bedrock::Result<void> PacketAdapter::_read(ReadOnlyBinaryStream &)
{
    throw std::runtime_error("Not implemented");
}

}  // namespace endstone::detail
