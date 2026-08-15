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

#include "bedrock/network/network_block_position.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/cerealize/core/serialization_mode.h"
#include "bedrock/world/level/dimension/dimension_type.h"

enum class SpawnPositionType : int {
    PlayerRespawn = 0,
    WorldSpawn = 1,
};

struct SetSpawnPositionPacketPayload {
    NetworkBlockPosition pos;              // +0
    SpawnPositionType spawn_pos_type;      // +12
    DimensionType dimension_type;          // +16
    NetworkBlockPosition spawn_block_pos;  // +20
};
BEDROCK_STATIC_ASSERT_SIZE(SetSpawnPositionPacketPayload, 32, 32);

class SetSpawnPositionPacket : public Packet {
public:
    SetSpawnPositionPacketPayload payload;                                              // +48 Windows, +44 Linux
    SerializationMode serialization_mode{SerializationMode::SideBySide_LogOnMismatch};  // +80 Windows, +76 Linux
};
BEDROCK_STATIC_ASSERT_SIZE(SetSpawnPositionPacket, 88, 80);
