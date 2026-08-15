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
#include "bedrock/network/packet/cerealize/core/serialization_mode.h"

struct StopSoundPacketPayload {
    std::string name;        // +0
    bool stop_all;           // +32
    bool stop_music_legacy;  // +33
};
BEDROCK_STATIC_ASSERT_SIZE(StopSoundPacketPayload, 40, 32);

class StopSoundPacket : public Packet {
public:
    StopSoundPacketPayload payload;                                                     // +48
    SerializationMode serialization_mode{SerializationMode::SideBySide_LogOnMismatch};  // +88
};
BEDROCK_STATIC_ASSERT_SIZE(StopSoundPacket, 96, 88);
