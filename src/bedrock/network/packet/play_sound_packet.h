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

#include "bedrock/audio/server_sound_handle.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/network/network_block_position.h"
#include "bedrock/network/packet.h"

struct PlaySoundPacketPayload {
    PlaySoundPacketPayload();
    PlaySoundPacketPayload(std::string name, const Vec3 &pos, float volume, float pitch,
                           std::optional<ServerSoundHandle> server_sound_handle);

    std::string name;
    NetworkBlockPosition pos;
    float volume;
    float pitch;
    std::optional<ServerSoundHandle> server_sound_handle;
};

class PlaySoundPacket : public Packet {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;

    PlaySoundPacket();
    PlaySoundPacket(PlaySoundPacketPayload payload);

    PlaySoundPacketPayload payload;
    SerializationMode serialization_mode;
};
