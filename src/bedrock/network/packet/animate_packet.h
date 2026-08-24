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

#include <cstdint>
#include <optional>

#include "bedrock/bedrock.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/cerealize/core/serialization_mode.h"
#include "bedrock/world/actor/actor_runtime_id.h"
#include "bedrock/world/actor/actor_swing_source.h"

struct AnimatePacketPayload {
    enum class Action : std::uint8_t {
        NoAction = 0,
        Swing = 1,
        WakeUp = 3,
        CriticalHit = 4,
        MagicCriticalHit = 5,
    };

    ActorRuntimeID runtime_id;                     // +0
    Action action;                                 // +8
    float data;                                    // +12
    std::optional<ActorSwingSource> swing_source;  // +16
};
BEDROCK_STATIC_ASSERT_SIZE(AnimatePacketPayload, 24, 24);

class AnimatePacket : public Packet {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;

    AnimatePacketPayload payload;                                         // +48
    SerializationMode serialization_mode{SerializationMode::CerealOnly};  // +72
};
BEDROCK_STATIC_ASSERT_SIZE(AnimatePacket, 80, 80);
