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
#include "bedrock/network/packet/types/world/actor/ai/util/boss_event_data.h"
#include "bedrock/safety/redactable_string.h"
#include "bedrock/world/actor/actor_unique_id.h"

struct BossEventPacketPayload {
    ActorUniqueID boss_id;                   // +0
    BossEventUpdateType event_type;          // +8
    Bedrock::Safety::RedactableString name;  // +16
    float health_percent;                    // +88
    BossBarColor color;                      // +92
    BossBarOverlay overlay;                  // +93
};
BEDROCK_STATIC_ASSERT_SIZE(BossEventPacketPayload, 96, 80);

class BossEventPacket : public Packet {
public:
    BossEventPacketPayload payload;                                       // +48
    SerializationMode serialization_mode{SerializationMode::CerealOnly};  // +144
};
BEDROCK_STATIC_ASSERT_SIZE(BossEventPacket, 152, 136);
