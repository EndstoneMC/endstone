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

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/cerealize/core/serialization_mode.h"
#include "bedrock/world/level/dimension/dimension_definition_group.h"

struct DimensionDataPacketPayload {
    DimensionDefinitionGroup dimension_definition_group;
};
BEDROCK_STATIC_ASSERT_SIZE(DimensionDataPacketPayload, 16, 24);

class DimensionDataPacket : public Packet {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;
    DimensionDataPacketPayload payload;
    SerializationMode serialization_mode;
};
BEDROCK_STATIC_ASSERT_SIZE(DimensionDataPacket, 72, 80);
