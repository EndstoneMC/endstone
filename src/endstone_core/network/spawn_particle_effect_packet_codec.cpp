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
#include "endstone/network/spawn_particle_effect_packet.h"

namespace endstone::detail {
template <>
void PacketCodec::encode(BinaryStream &stream, SpawnParticleEffectPacket &packet)
{
    stream.writeUnsignedChar(packet.dimension_id);
    stream.writeVarInt64(packet.actor_id);
    stream.writeFloat(packet.position.getX());
    stream.writeFloat(packet.position.getY());
    stream.writeFloat(packet.position.getZ());
    stream.writeString(packet.effect_name);
    stream.writeBool(packet.molang_variables_json.has_value());
    if (packet.molang_variables_json.has_value()) {
        stream.writeString(packet.molang_variables_json.value());
    }
}

}  // namespace endstone::detail
