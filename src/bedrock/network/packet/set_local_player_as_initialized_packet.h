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
#include "bedrock/world/actor/actor_runtime_id.h"

struct SetLocalPlayerAsInitializedPacketPayload {
    ActorRuntimeID player_id;  // +0
};
BEDROCK_STATIC_ASSERT_SIZE(SetLocalPlayerAsInitializedPacketPayload, 8, 8);

class SetLocalPlayerAsInitializedPacket : public Packet {
public:
    SetLocalPlayerAsInitializedPacket();
    SetLocalPlayerAsInitializedPacket(SetLocalPlayerAsInitializedPacketPayload);

    SetLocalPlayerAsInitializedPacketPayload payload;  // +48
    SerializationMode serialization_mode;              // +56
};
BEDROCK_STATIC_ASSERT_SIZE(SetLocalPlayerAsInitializedPacket, 64, 64);
