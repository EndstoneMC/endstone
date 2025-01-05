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

#include "bedrock/core/utility/binary_stream.h"
#include "endstone/detail/network/packet_codec.h"
#include "endstone/network/play_sound_packet.h"

namespace endstone::detail {
template <>
void PacketCodec::encode(BinaryStream &stream, PlaySoundPacket &packet)
{
    stream.writeString(packet.name)
    stream.writeVarInt(packet.position.x)
    stream.writeUnsignedVarInt(packet.position.y)
    stream.writeVarInt(packet.position.z)
    stream.writeFloat(packet.volume);
    stream.writeFloat(packet.pitch);
}

}  // namespace endstone::detail
